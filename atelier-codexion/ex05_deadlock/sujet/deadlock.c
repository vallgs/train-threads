/* ex05 — Sujet : provoquer (puis soigner) un deadlock
 * Deux threads ont chacun besoin des DEUX mutex. Lance le programme :
 * il se bloque (Ctrl+C pour sortir). Explique pourquoi, puis corrige-le
 * SANS supprimer aucun des deux verrous.
 * Compile : cc -Wall -Wextra -Werror -pthread deadlock.c -o deadlock
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

static void	*alice(void *arg)
{
	t_ctx	*c;
	int		i;

	c = arg;
	i = -1;
	while (++i < 100)
	{
		pthread_mutex_lock(&c->usb_a);
		usleep(1000);
		pthread_mutex_lock(&c->usb_b);
		c->done++;
		pthread_mutex_unlock(&c->usb_b);
		pthread_mutex_unlock(&c->usb_a);
	}
	return (NULL);
}

static void	*bob(void *arg)
{
	t_ctx	*c;
	int		i;

	c = arg;
	i = -1;
	while (++i < 100)
	{
		pthread_mutex_lock(&c->usb_b);
		usleep(1000);
		pthread_mutex_lock(&c->usb_a);
		c->done++;
		pthread_mutex_unlock(&c->usb_a);
		pthread_mutex_unlock(&c->usb_b);
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
	pthread_create(&t1, NULL, alice, &c);
	pthread_create(&t2, NULL, bob, &c);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("compilations faites : %d / 200\n", c.done);
	pthread_mutex_destroy(&c.usb_a);
	pthread_mutex_destroy(&c.usb_b);
	return (0);
}
