/* ex01 — Chasse au bug : l'argument partagé
 * Ce programme est censé afficher "je suis le thread 1" ... "je suis le thread 4"
 * (dans n'importe quel ordre). Lance-le plusieurs fois. Que se passe-t-il ?
 * Compile : cc -Wall -Wextra -Werror -pthread arg_bug.c -o arg_bug
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
	int			i;

	i = 1;
	while (i <= 4)
	{
		pthread_create(&threads[i - 1], NULL, routine, &i);
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
