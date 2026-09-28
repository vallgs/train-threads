/* ex08 — Correction : une ressource avec temps de recharge (cooldown)
 * Un seul dongle, 3 coders qui le veulent chacun 5 fois.
 * Règle : après un release, personne ne peut le reprendre avant `cooldown` ms.
 * Compile : cc -Wall -Wextra -Werror -pthread cooldown.c -o cooldown
 */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define USES 5
#define HOLD_MS 20
#define COOLDOWN_MS 50

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				taken;
	long			available_at;
	long			cooldown;
	long			log_take[3 * USES];
	long			log_release[3 * USES];
	int				n_log;
}	t_dongle;

typedef struct s_user
{
	pthread_t	thread;
	int			id;
	long		start;
	t_dongle	*d;
}	t_user;

static long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

static struct timespec	ms_to_timespec(long abs_ms)
{
	struct timespec	ts;

	ts.tv_sec = abs_ms / 1000;
	ts.tv_nsec = (abs_ms % 1000) * 1000000L;
	return (ts);
}

/* Bloque jusqu'à ce que le dongle soit libre ET rechargé. */
void	dongle_take(t_dongle *d)
{
	struct timespec	ts;

	pthread_mutex_lock(&d->lock);
	while (d->taken || now_ms() < d->available_at)
	{
		if (d->taken)
			pthread_cond_wait(&d->cond, &d->lock);
		else
		{
			/* Libre mais en recharge : on sait exactement QUAND il sera
			 * dispo, donc on dort jusque-là (ou jusqu'à un signal). */
			ts = ms_to_timespec(d->available_at);
			pthread_cond_timedwait(&d->cond, &d->lock, &ts);
		}
	}
	d->taken = 1;
	d->log_take[d->n_log] = now_ms();
	pthread_mutex_unlock(&d->lock);
}

void	dongle_release(t_dongle *d)
{
	pthread_mutex_lock(&d->lock);
	d->taken = 0;
	d->available_at = now_ms() + d->cooldown;
	d->log_release[d->n_log++] = now_ms();
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->lock);
}

static void	*routine(void *arg)
{
	t_user	*u;
	int		i;

	u = arg;
	i = -1;
	while (++i < USES)
	{
		dongle_take(u->d);
		printf("%ld %d has taken a dongle\n", now_ms() - u->start, u->id);
		usleep(HOLD_MS * 1000);
		dongle_release(u->d);
	}
	return (NULL);
}

static int	check(t_dongle *d)
{
	int	i;
	int	ok;

	ok = 1;
	i = 0;
	while (++i < d->n_log)
	{
		if (d->log_take[i] - d->log_release[i - 1] < d->cooldown)
		{
			printf("ERREUR : repris %ld ms après release\n",
				d->log_take[i] - d->log_release[i - 1]);
			ok = 0;
		}
	}
	return (ok);
}

int	main(void)
{
	t_dongle	d;
	t_user		users[3];
	long		start;
	int			i;

	pthread_mutex_init(&d.lock, NULL);
	pthread_cond_init(&d.cond, NULL);
	d.taken = 0;
	d.available_at = 0;
	d.cooldown = COOLDOWN_MS;
	d.n_log = 0;
	start = now_ms();
	i = -1;
	while (++i < 3)
	{
		users[i] = (t_user){.id = i + 1, .start = start, .d = &d};
		pthread_create(&users[i].thread, NULL, routine, &users[i]);
	}
	i = -1;
	while (++i < 3)
		pthread_join(users[i].thread, NULL);
	if (check(&d))
		printf("OK : cooldown de %d ms toujours respecté (%d prises)\n",
			COOLDOWN_MS, d.n_log);
	pthread_cond_destroy(&d.cond);
	pthread_mutex_destroy(&d.lock);
	return (0);
}
