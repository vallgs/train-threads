/* ex00 — Premiers threads
 * Compile : cc -Wall -Wextra -Werror -pthread hello.c -o hello
 * Usage   : ./hello 5
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_worker
{
	pthread_t	thread;
	int			id;
}	t_worker;

static void	*routine(void *arg)
{
	t_worker	*w;

	w = (t_worker *)arg;
	printf("Coder %d : bonjour !\n", w->id);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_worker	*workers;
	int			n;
	int			i;

	if (argc != 2 || atoi(argv[1]) <= 0)
		return (printf("usage: %s n (n > 0)\n", argv[0]), 1);
	n = atoi(argv[1]);
	workers = malloc(sizeof(t_worker) * n);
	if (!workers)
		return (1);
	i = 0;
	while (i < n)
	{
		workers[i].id = i + 1;
		if (pthread_create(&workers[i].thread, NULL, routine, &workers[i]))
			break ;
		i++;
	}
	/* On ne joint que les threads effectivement créés */
	while (--i >= 0)
		pthread_join(workers[i].thread, NULL);
	printf("main : tous les threads sont terminés\n");
	free(workers);
	return (0);
}
