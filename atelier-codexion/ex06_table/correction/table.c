/* ex06 — Correction : la table ronde de dongles
 * N coders, N dongles (mutex). Le coder i (0-based) a le dongle i à gauche
 * et le dongle (i + 1) % N à droite. Chacun compile K fois.
 * Compile : cc -Wall -Wextra -Werror -pthread table.c -o table
 * Usage   : ./table 5 20
 *
 * Anti-deadlock : chacun prend d'abord le dongle d'INDICE LE PLUS PETIT.
 * Le dernier coder (gauche = N-1, droite = 0) prend donc d'abord le 0 :
 * il "tourne dans l'autre sens", ce qui casse le cycle.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_table
{
	int				n;
	int				k;
	long			start;
	pthread_mutex_t	*dongles;
	pthread_mutex_t	print;
}	t_table;

typedef struct s_coder
{
	pthread_t	thread;
	int			id;
	int			first;
	int			second;
	int			compiles;
	t_table		*table;
}	t_coder;

static long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

static void	say(t_coder *c, const char *msg)
{
	pthread_mutex_lock(&c->table->print);
	printf("%ld %d %s\n", now_ms() - c->table->start, c->id, msg);
	pthread_mutex_unlock(&c->table->print);
}

static void	*routine(void *arg)
{
	t_coder	*c;

	c = arg;
	while (c->compiles < c->table->k)
	{
		pthread_mutex_lock(&c->table->dongles[c->first]);
		say(c, "has taken a dongle");
		usleep(1000); /* élargit la fenêtre : sans l'ordre, deadlock assuré */
		pthread_mutex_lock(&c->table->dongles[c->second]);
		say(c, "has taken a dongle");
		say(c, "is compiling");
		usleep(2000);
		c->compiles++;
		pthread_mutex_unlock(&c->table->dongles[c->second]);
		pthread_mutex_unlock(&c->table->dongles[c->first]);
		say(c, "is debugging");
		usleep(1000);
	}
	return (NULL);
}

static void	setup_coder(t_coder *c, int i, t_table *t)
{
	int	left;
	int	right;

	left = i;
	right = (i + 1) % t->n;
	c->id = i + 1;
	c->compiles = 0;
	c->table = t;
	c->first = left;
	c->second = right;
	if (right < left)
	{
		c->first = right;
		c->second = left;
	}
}

int	main(int argc, char **argv)
{
	t_table	t;
	t_coder	*coders;
	int		i;

	if (argc != 3 || atoi(argv[1]) < 2 || atoi(argv[2]) < 1)
		return (printf("usage: %s n(>=2) k(>=1)\n", argv[0]), 1);
	t.n = atoi(argv[1]);
	t.k = atoi(argv[2]);
	t.dongles = malloc(sizeof(pthread_mutex_t) * t.n);
	coders = malloc(sizeof(t_coder) * t.n);
	if (!t.dongles || !coders)
		return (free(t.dongles), free(coders), 1);
	pthread_mutex_init(&t.print, NULL);
	i = -1;
	while (++i < t.n)
		pthread_mutex_init(&t.dongles[i], NULL);
	t.start = now_ms();
	i = -1;
	while (++i < t.n)
	{
		setup_coder(&coders[i], i, &t);
		pthread_create(&coders[i].thread, NULL, routine, &coders[i]);
	}
	i = -1;
	while (++i < t.n)
		pthread_join(coders[i].thread, NULL);
	i = -1;
	while (++i < t.n)
		pthread_mutex_destroy(&t.dongles[i]);
	pthread_mutex_destroy(&t.print);
	printf("OK : %d coders ont compilé %d fois chacun\n", t.n, t.k);
	free(t.dongles);
	free(coders);
	return (0);
}
