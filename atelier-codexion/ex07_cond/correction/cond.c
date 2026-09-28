/* ex07 — Correction : variables de condition
 * Partie A : "coup de départ" — N threads attendent passivement un signal.
 * Partie B : attente avec délai maximum (pthread_cond_timedwait).
 * Compile : cc -Wall -Wextra -Werror -pthread cond.c -o cond
 */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

typedef struct s_gate
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				open;
	long			start;
}	t_gate;

typedef struct s_runner
{
	pthread_t	thread;
	int			id;
	t_gate		*gate;
}	t_runner;

static long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

/* Convertit une date absolue en ms en struct timespec (ce qu'attend timedwait) */
static struct timespec	ms_to_timespec(long abs_ms)
{
	struct timespec	ts;

	ts.tv_sec = abs_ms / 1000;
	ts.tv_nsec = (abs_ms % 1000) * 1000000L;
	return (ts);
}

static void	*runner(void *arg)
{
	t_runner	*r;

	r = arg;
	pthread_mutex_lock(&r->gate->lock);
	/* TOUJOURS un while : réveils parasites (spurious wakeups) possibles,
	 * et la condition a pu changer entre le signal et notre réveil. */
	while (!r->gate->open)
		pthread_cond_wait(&r->gate->cond, &r->gate->lock);
	pthread_mutex_unlock(&r->gate->lock);
	printf("%ld runner %d est parti\n", now_ms() - r->gate->start, r->id);
	return (NULL);
}

static void	part_a(t_gate *g)
{
	t_runner	runners[5];
	int			i;

	i = -1;
	while (++i < 5)
	{
		runners[i].id = i + 1;
		runners[i].gate = g;
		pthread_create(&runners[i].thread, NULL, runner, &runners[i]);
	}
	usleep(300 * 1000);
	printf("%ld main : PAN !\n", now_ms() - g->start);
	pthread_mutex_lock(&g->lock);
	g->open = 1;
	pthread_cond_broadcast(&g->cond);
	pthread_mutex_unlock(&g->lock);
	i = -1;
	while (++i < 5)
		pthread_join(runners[i].thread, NULL);
}

/* Attend que g->open passe à 1, mais pas plus de timeout_ms.
 * Retourne 1 si ouvert, 0 si délai dépassé. */
static int	wait_open_for(t_gate *g, long timeout_ms)
{
	struct timespec	deadline;
	int				ret;

	deadline = ms_to_timespec(now_ms() + timeout_ms);
	ret = 0;
	pthread_mutex_lock(&g->lock);
	while (!g->open && ret != ETIMEDOUT)
		ret = pthread_cond_timedwait(&g->cond, &g->lock, &deadline);
	ret = g->open;
	pthread_mutex_unlock(&g->lock);
	return (ret);
}

int	main(void)
{
	t_gate	g;
	long	t0;
	int		res;

	pthread_mutex_init(&g.lock, NULL);
	pthread_cond_init(&g.cond, NULL);
	g.open = 0;
	g.start = now_ms();
	part_a(&g);
	g.open = 0;
	t0 = now_ms();
	res = wait_open_for(&g, 200);
	printf("partie B : ouvert=%d après %ld ms (attendu : 0 après ~200 ms)\n",
		res, now_ms() - t0);
	pthread_cond_destroy(&g.cond);
	pthread_mutex_destroy(&g.lock);
	return (0);
}
