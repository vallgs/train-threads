/* ex09 — Correction : un tas binaire minimum (priority queue)
 * Pas de thread ici : on construit la structure de données seule, et on la teste.
 * Compile : cc -Wall -Wextra -Werror heap.c -o heap
 *
 * Tas stocké dans un tableau : pour l'indice i,
 *   parent = (i - 1) / 2, enfant gauche = 2i + 1, enfant droit = 2i + 2.
 * Invariant : chaque parent est "plus prioritaire" que ses enfants.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct s_req
{
	long			key;	/* FIFO : heure d'arrivée ; EDF : deadline */
	unsigned long	seq;	/* numéro d'arrivée, départage les égalités */
	int				id;
}	t_req;

typedef struct s_heap
{
	t_req	*data;
	int		size;
	int		cap;
}	t_heap;

/* a passe avant b ? Clé d'abord, puis ordre d'arrivée (tie-breaker). */
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

int	heap_init(t_heap *h, int cap)
{
	h->data = malloc(sizeof(t_req) * cap);
	h->size = 0;
	h->cap = cap;
	return (h->data != NULL);
}

int	heap_push(t_heap *h, t_req r)
{
	int	i;

	if (h->size == h->cap)
		return (0);
	i = h->size++;
	h->data[i] = r;
	while (i > 0 && before(h->data[i], h->data[(i - 1) / 2]))
	{
		swap(&h->data[i], &h->data[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
	return (1);
}

t_req	heap_pop(t_heap *h)
{
	t_req	top;
	int		i;
	int		best;

	top = h->data[0];
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
			break ;
		swap(&h->data[i], &h->data[best]);
		i = best;
	}
	return (top);
}

int	main(void)
{
	t_heap			h;
	t_req			prev;
	t_req			cur;
	unsigned long	seq;
	unsigned int	x;
	int				ok;

	if (!heap_init(&h, 64))
		return (1);
	x = 42;
	seq = 0;
	while (seq < 40)
	{
		x = x * 1103515245 + 12345;
		/* clés entre 0 et 9 : beaucoup d'égalités, pour tester le tie-breaker */
		heap_push(&h, (t_req){.key = (x >> 16) % 10, .seq = seq,
			.id = (int)(seq % 5) + 1});
		seq++;
	}
	ok = 1;
	prev = heap_pop(&h);
	printf("(key=%ld seq=%lu) ", prev.key, prev.seq);
	while (h.size > 0)
	{
		cur = heap_pop(&h);
		printf("(key=%ld seq=%lu) ", cur.key, cur.seq);
		if (before(cur, prev))
			ok = 0;
		prev = cur;
	}
	printf("\n%s\n", ok ? "OK : sortie triée par (key, seq)" : "ERREUR d'ordre");
	free(h.data);
	return (!ok);
}
