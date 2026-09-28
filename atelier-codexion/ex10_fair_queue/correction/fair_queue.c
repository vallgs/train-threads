/* ex10 — Correction : une file d'attente équitable (FIFO / EDF)
 * Une ressource unique, une file de priorité (le tas de l'ex09),
 * un mutex + une condition. Le verrou n'est accordé qu'au thread
 * qui est EN TÊTE de la file.
 * Compile : cc -Wall -Wextra -Werror -pthread fair_queue.c -o fair_queue
 * Usage   : ./fair_queue fifo   |   ./fair_queue edf
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 6

/* ---------- tas (copie de l'ex09) ---------- */
typedef struct s_req
{
	long			key;
	unsigned long	seq;
	int				id;
}	t_req;

typedef struct s_heap
{
	t_req	data[N];
	int		size;
}	t_heap;

static int	before(t_req a, t_req b)
{
	if (a.key != b.key)
		return (a.key < b.key);
	return (a.seq < b.seq);
}

static void	swap(t_req *a, t_req *b)
{
	t_req	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	heap_push(t_heap *h, t_req r)
{
	int	i;

	i = h->size++;
	h->data[i] = r;
	while (i > 0 && before(h->data[i], h->data[(i - 1) / 2]))
	{
		swap(&h->data[i], &h->data[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

static void	heap_pop(t_heap *h)
{
	int	i;
	int	best;

	h->data[0] = h->data[--h->size];
	i = 0;
	while (1)
	{
		best = i;
		if (2 * i + 1 < h->size && before(h->data[2 * i + 1], h->data[best]))
			best = 2 * i + 1;
		if (2 * i + 2 < h->size && before(h->data[2 * i + 2], h->data[best]))
			best = 2 * i + 2;
		if (best == i)
			return ;
		swap(&h->data[i], &h->data[best]);
		i = best;
	}
}

/* ---------- la ressource équitable ---------- */
typedef struct s_res
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				taken;
	unsigned long	next_seq;
	t_heap			queue;
	int				order[N];
	int				n_granted;
}	t_res;

typedef struct s_client
{
	pthread_t	thread;
	int			id;
	long		deadline;
	int			edf;
	t_res		*res;
}	t_client;

void	res_acquire(t_res *r, int id, long key)
{
	pthread_mutex_lock(&r->lock);
	heap_push(&r->queue, (t_req){.key = key, .seq = r->next_seq, .id = id});
	r->next_seq++;
	/* J'attends tant que la ressource est prise OU que ce n'est pas mon tour */
	while (r->taken || r->queue.data[0].id != id)
		pthread_cond_wait(&r->cond, &r->lock);
	heap_pop(&r->queue);
	r->taken = 1;
	r->order[r->n_granted++] = id;
	pthread_mutex_unlock(&r->lock);
}

void	res_release(t_res *r)
{
	pthread_mutex_lock(&r->lock);
	r->taken = 0;
	/* broadcast et pas signal : on ne sait pas QUEL thread dort sur la
	 * condition, il faut réveiller celui qui est en tête de file. */
	pthread_cond_broadcast(&r->cond);
	pthread_mutex_unlock(&r->lock);
}

static void	*client(void *arg)
{
	t_client	*c;
	long		key;

	c = arg;
	usleep(c->id * 5000);
	/* FIFO : même clé pour tous, c'est donc `seq` (l'ordre d'arrivée,
	 * attribué sous le verrou) qui décide. EDF : la deadline décide,
	 * et `seq` ne sert qu'à départager deux deadlines égales. */
	key = 0;
	if (c->edf)
		key = c->deadline;
	res_acquire(c->res, c->id, key);
	printf("client %d servi (deadline %ld)\n", c->id, c->deadline);
	usleep(2000);
	res_release(c->res);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_res		r;
	t_client	c[N];
	int			edf;
	int			i;

	if (argc != 2 || (strcmp(argv[1], "fifo") && strcmp(argv[1], "edf")))
		return (printf("usage: %s fifo|edf\n", argv[0]), 1);
	edf = (strcmp(argv[1], "edf") == 0);
	memset(&r, 0, sizeof(r));
	pthread_mutex_init(&r.lock, NULL);
	pthread_cond_init(&r.cond, NULL);
	r.taken = 1; /* main bloque la ressource pendant que tout le monde arrive */
	i = -1;
	while (++i < N)
	{
		c[i] = (t_client){.id = i + 1, .edf = edf, .res = &r,
			.deadline = 1000 - (i + 1) * 10};
		pthread_create(&c[i].thread, NULL, client, &c[i]);
	}
	usleep(100 * 1000);
	res_release(&r);
	i = -1;
	while (++i < N)
		pthread_join(c[i].thread, NULL);
	printf("ordre de service :");
	i = -1;
	while (++i < N)
		printf(" %d", r.order[i]);
	printf("\n(attendu : fifo -> 1 2 3 4 5 6 ; edf -> 6 5 4 3 2 1)\n");
	pthread_cond_destroy(&r.cond);
	pthread_mutex_destroy(&r.lock);
	return (0);
}
