/* ex02 — Race condition
 * Deux threads incrémentent le même compteur 1 000 000 de fois chacun.
 * Résultat attendu : 2000000. Lance-le 5 fois. Que vois-tu ?
 * Compile : cc -Wall -Wextra -Werror -pthread race.c -o race
 */
#include <pthread.h>
#include <stdio.h>

#define ITER 1000000

typedef struct s_data
{
	long	counter;
}	t_data;

static void	*routine(void *arg)
{
	t_data	*d;
	int		i;

	d = (t_data *)arg;
	i = 0;
	while (i < ITER)
	{
		d->counter++;
		i++;
	}
	return (NULL);
}

int	main(void)
{
	pthread_t	a;
	pthread_t	b;
	t_data		data;

	data.counter = 0;
	pthread_create(&a, NULL, routine, &data);
	pthread_create(&b, NULL, routine, &data);
	pthread_join(a, NULL);
	pthread_join(b, NULL);
	printf("counter = %ld (attendu %d)\n", data.counter, 2 * ITER);
	return (0);
}
