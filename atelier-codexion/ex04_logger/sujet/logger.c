/* ex04 — Sujet : logs mélangés
 * Chaque thread affiche ses lignes en TROIS appels séparés (comme si tu
 * construisais la ligne morceau par morceau). Lance le programme :
 * les lignes se mélangent. Ta mission : écrire un logger thread-safe.
 * Compile : cc -Wall -Wextra -Werror -pthread logger.c -o logger
 */
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_coder
{
	pthread_t	thread;
	int			id;
	long		start;
}	t_coder;

static long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
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
		printf("%ld", get_time_ms() - c->start);
		printf(" %d", c->id);
		printf(" %s\n", states[i % 3]);
		i++;
	}
	return (NULL);
}

int	main(void)
{
	t_coder	coders[4];
	long	start;
	int		i;

	start = get_time_ms();
	i = -1;
	while (++i < 4)
	{
		coders[i].id = i + 1;
		coders[i].start = start;
		pthread_create(&coders[i].thread, NULL, routine, &coders[i]);
	}
	i = -1;
	while (++i < 4)
		pthread_join(coders[i].thread, NULL);
	return (0);
}
