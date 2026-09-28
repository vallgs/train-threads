/* ex05 — Correction : ordre global de verrouillage
 * Les deux threads prennent TOUJOURS usb_a avant usb_b.
 * On casse la condition d'attente circulaire (4e condition de Coffman) :
 * celui qui tient usb_b a forcément déjà usb_a, donc personne n'attend
 * une ressource détenue par quelqu'un qui l'attend lui-même.
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_ctx
{
	pthread_mutex_t	usb_a;
	pthread_mutex_t	usb_b;
	int				done;
}	t_ctx;

static void	take_both(t_ctx *c)
{
	pthread_mutex_lock(&c->usb_a);
	usleep(1000);
	pthread_mutex_lock(&c->usb_b);
}

static void	release_both(t_ctx *c)
{
	pthread_mutex_unlock(&c->usb_b);
	pthread_mutex_unlock(&c->usb_a);
}

static void	*worker(void *arg)
{
	t_ctx	*c;
	int		i;

	c = arg;
	i = -1;
	while (++i < 100)
	{
		take_both(c);
		c->done++;
		release_both(c);
	}
	return (NULL);
}

int	main(void)
{
	pthread_t	t1;
	pthread_t	t2;
	t_ctx		c;

	pthread_mutex_init(&c.usb_a, NULL);
	pthread_mutex_init(&c.usb_b, NULL);
	c.done = 0;
	pthread_create(&t1, NULL, worker, &c);
	pthread_create(&t2, NULL, worker, &c);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("compilations faites : %d / 200\n", c.done);
	pthread_mutex_destroy(&c.usb_a);
	pthread_mutex_destroy(&c.usb_b);
	return (0);
}
