/* ex01 — Correction
 * Chaque thread reçoit l'adresse de SA propre case, qui vit plus longtemps
 * que le thread (elle est dans main, et main attend avec pthread_join).
 */
#include <pthread.h>
#include <stdio.h>

static void	*routine(void *arg)
{
	int	id;

	id = *(int *)arg;
	printf("je suis le thread %d\n", id);
	return (NULL);
}

int	main(void)
{
	pthread_t	threads[4];
	int			ids[4];
	int			i;

	i = 0;
	while (i < 4)
	{
		ids[i] = i + 1;
		pthread_create(&threads[i], NULL, routine, &ids[i]);
		i++;
	}
	i = 0;
	while (i < 4)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	return (0);
}
