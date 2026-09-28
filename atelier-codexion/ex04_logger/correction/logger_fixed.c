/* ex04 — Correction : logger thread-safe avec arrêt
 * Compile : cc -Wall -Wextra -Werror -pthread logger_fixed.c -o logger_fixed
 * Vérif   : ./logger_fixed | tail -3   -> la dernière ligne doit être "burned out"
 */
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_log
{
	pthread_mutex_t	lock;
	long			start;
	int				stopped;
}	t_log;

typedef struct s_coder
{
	pthread_t	thread;
	int			id;
	t_log		*log;
}	t_coder;

static long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

/* Affiche une ligne complète SI la simulation n'est pas arrêtée.
 * Le timestamp est pris APRÈS avoir le verrou : les lignes sortent donc
 * toujours dans l'ordre chronologique. Retourne 1 si la simulation est finie. */
int	log_state(t_log *log, int id, const char *msg)
{
	pthread_mutex_lock(&log->lock);
	if (log->stopped)
	{
		pthread_mutex_unlock(&log->lock);
		return (1);
	}
	printf("%ld %d %s\n", get_time_ms() - log->start, id, msg);
	pthread_mutex_unlock(&log->lock);
	return (0);
}

/* Dernier message : on l'affiche ET on arrête tout, sous le même verrou.
 * Personne ne peut donc glisser une ligne après "burned out". */
void	log_final(t_log *log, int id, const char *msg)
{
	pthread_mutex_lock(&log->lock);
	if (!log->stopped)
	{
		printf("%ld %d %s\n", get_time_ms() - log->start, id, msg);
		log->stopped = 1;
	}
	pthread_mutex_unlock(&log->lock);
}

static void	*routine(void *arg)
{
	t_coder		*c;
	int			i;
	const char	*states[3] = {"is compiling", "is debugging", "is refactoring"};

	c = (t_coder *)arg;
	i = 0;
	while (i < 300)
	{
		if (log_state(c->log, c->id, states[i % 3]))
			break ;
		usleep(100);
		i++;
	}
	return (NULL);
}

int	main(void)
{
	t_coder	coders[4];
	t_log	log;
	int		i;

	pthread_mutex_init(&log.lock, NULL);
	log.stopped = 0;
	log.start = get_time_ms();
	i = -1;
	while (++i < 4)
	{
		coders[i].id = i + 1;
		coders[i].log = &log;
		pthread_create(&coders[i].thread, NULL, routine, &coders[i]);
	}
	usleep(10 * 1000);
	log_final(&log, 3, "burned out");
	i = -1;
	while (++i < 4)
		pthread_join(coders[i].thread, NULL);
	pthread_mutex_destroy(&log.lock);
	return (0);
}
