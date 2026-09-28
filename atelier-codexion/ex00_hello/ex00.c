#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct s_worker
{
	pthread_t		thread;
	int				id;
}t_worker;

void	*routine(void *arg)
{
	t_worker	*w;

	w = (t_worker *)arg;
	printf("coder %d: bonjour !\n", w->id);
	return (NULL);
}

int main(int ac, char **av)
{
	t_worker	*workers;
	int		n;
	int		i;

	if (ac != 2 || atoi(av[1]) <= 0)
		return (printf("usage: %s n (n > 0)\n", av[0]), 1);
	n = atoi(av[1]);
	workers = malloc(sizeof(t_worker) * n);
	if (!workers)
		return (1);
	i = 0;
	while (i < n)
	{
		workers[i].id = i + 1;
		if (pthread_create(&workers[i].thread, NULL, routine, &workers[i]))
			break;
		i++;
	}
	while (--i >= 0)
		pthread_join(workers[i].thread, NULL);
	printf("main: threads fini\n");
	free(workers);
	return (0);
}