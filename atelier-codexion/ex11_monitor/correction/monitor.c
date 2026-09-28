/* ex11 — Correction : le thread monitor (détection de burnout)
 * 4 workers "compilent" à intervalles réguliers. Le worker 4 est trop lent.
 * Un monitor surveille tout le monde et arrête la simulation :
 *   - dès qu'un worker n'a pas recompilé depuis plus de `timeout` ms ;
 *   - ou dès que tous ont compilé au moins `required` fois.
 * Compile : cc -Wall -Wextra -Werror -pthread monitor.c -o monitor
 * Usage   : ./monitor 250 5   (le worker 4 doit burn out)
 *           ./monitor 400 5   (personne ne burn out, arrêt "tous ont fini")
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define N 4

typedef struct s_sim	t_sim;

typedef struct s_worker
{
	pthread_t		thread;
	int				id;
	long			period;
	long			last_compile;
	int				compiles;
	pthread_mutex_t	lock;	/* protège last_compile et compiles */
	t_sim			*sim;
}	t_worker;

struct s_sim
{
	pthread_mutex_t	lock;	/* protège stop ET l'affichage */
	int				stop;
	long			start;
	long			timeout;
	int				required;
	t_worker		w[N];
};

static long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

static int	is_stopped(t_sim *s)
{
	int	ret;

	pthread_mutex_lock(&s->lock);
	ret = s->stop;
	pthread_mutex_unlock(&s->lock);
	return (ret);
}

static void	say(t_sim *s, int id, const char *msg)
{
	pthread_mutex_lock(&s->lock);
	if (!s->stop)
		printf("%ld %d %s\n", now_ms() - s->start, id, msg);
	pthread_mutex_unlock(&s->lock);
}

/* Sommeil qui se réveille souvent pour voir si la simulation est finie :
 * sans ça, un worker endormi 300 ms retarderait la fin du programme. */
static void	sleep_or_stop(t_sim *s, long ms)
{
	long	target;

	target = now_ms() + ms;
	while (now_ms() < target && !is_stopped(s))
		usleep(500);
}

static void	*worker(void *arg)
{
	t_worker	*w;

	w = arg;
	while (!is_stopped(w->sim))
	{
		pthread_mutex_lock(&w->lock);
		w->last_compile = now_ms();
		w->compiles++;
		pthread_mutex_unlock(&w->lock);
		say(w->sim, w->id, "is compiling");
		sleep_or_stop(w->sim, w->period);
	}
	return (NULL);
}

/* Retourne 1 si la simulation doit s'arrêter */
static int	check_all(t_sim *s)
{
	int		i;
	int		all_done;
	long	last;
	int		count;

	all_done = 1;
	i = -1;
	while (++i < N)
	{
		pthread_mutex_lock(&s->w[i].lock);
		last = s->w[i].last_compile;
		count = s->w[i].compiles;
		pthread_mutex_unlock(&s->w[i].lock);
		if (now_ms() - last > s->timeout)
		{
			pthread_mutex_lock(&s->lock);
			printf("%ld %d burned out (retard de détection : %ld ms)\n",
				now_ms() - s->start, s->w[i].id, now_ms() - (last + s->timeout));
			s->stop = 1;
			pthread_mutex_unlock(&s->lock);
			return (1);
		}
		if (count < s->required)
			all_done = 0;
	}
	if (!all_done)
		return (0);
	pthread_mutex_lock(&s->lock);
	s->stop = 1;
	printf("%ld tout le monde a compilé %d fois\n",
		now_ms() - s->start, s->required);
	pthread_mutex_unlock(&s->lock);
	return (1);
}

static void	*monitor(void *arg)
{
	t_sim	*s;

	s = arg;
	while (!check_all(s))
		usleep(1000);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_sim		s;
	pthread_t	mon;
	int			i;

	if (argc != 3 || atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0)
		return (printf("usage: %s timeout_ms required\n", argv[0]), 1);
	s.timeout = atoi(argv[1]);
	s.required = atoi(argv[2]);
	s.stop = 0;
	pthread_mutex_init(&s.lock, NULL);
	s.start = now_ms();
	i = -1;
	while (++i < N)
	{
		s.w[i].id = i + 1;
		s.w[i].period = 50 + i * 20;
		if (i == N - 1)
			s.w[i].period = 300;
		s.w[i].last_compile = s.start;
		s.w[i].compiles = 0;
		s.w[i].sim = &s;
		pthread_mutex_init(&s.w[i].lock, NULL);
	}
	i = -1;
	while (++i < N)
		pthread_create(&s.w[i].thread, NULL, worker, &s.w[i]);
	pthread_create(&mon, NULL, monitor, &s);
	pthread_join(mon, NULL);
	i = -1;
	while (++i < N)
		pthread_join(s.w[i].thread, NULL);
	i = -1;
	while (++i < N)
		pthread_mutex_destroy(&s.w[i].lock);
	pthread_mutex_destroy(&s.lock);
	return (0);
}
