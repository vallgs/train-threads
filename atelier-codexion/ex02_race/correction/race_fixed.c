/* ex02 — Correction : le compteur est protégé par un mutex.
 * Le mutex vit dans la même structure que la donnée qu'il protège :
 * c'est l'habitude à prendre (pas de variable globale dans Codexion !).
 */
#include <pthread.h>
#include <stdio.h>

#define ITER 1000000

typedef struct s_data
{
	long			counter;
	pthread_mutex_t	lock;
}	t_data;

static void	*routine(void *arg)
{
	t_data	*d;
	int		i;

	d = (t_data *)arg;
	i = 0;
	while (i < ITER)
	{
		pthread_mutex_lock(&d->lock);
		d->counter++;
		pthread_mutex_unlock(&d->lock);
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
	if (pthread_mutex_init(&data.lock, NULL))
		return (1);
	pthread_create(&a, NULL, routine, &data);
	pthread_create(&b, NULL, routine, &data);
	pthread_join(a, NULL);
	pthread_join(b, NULL);
	printf("counter = %ld (attendu %d)\n", data.counter, 2 * ITER);
	pthread_mutex_destroy(&data.lock);
	return (0);
}
