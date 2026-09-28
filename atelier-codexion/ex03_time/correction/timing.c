/* ex03 — Correction : mesurer le temps et dormir précisément
 * Compile : cc -Wall -Wextra -Werror -pthread timing.c -o timing
 * Astuce  : relance-le pendant qu'un autre programme charge le CPU
 *           (ex : ./race dans un autre terminal en boucle) et compare.
 */
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

/* Temps absolu en microsecondes (pour mesurer finement) */
long	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000L + tv.tv_usec);
}

/* Temps absolu en millisecondes (ce qu'on affiche dans les logs) */
long	get_time_ms(void)
{
	return (get_time_us() / 1000);
}

/* Dort `ms` millisecondes par petits morceaux : on se réveille souvent
 * pour regarder l'heure, donc on s'arrête au plus près de la cible.
 * Bonus pour plus tard : c'est aussi l'endroit idéal pour vérifier
 * "la simulation est-elle finie ?" (voir ex11). */
void	precise_sleep(long ms)
{
	long	target;
	long	left;

	target = get_time_us() + ms * 1000;
	left = target - get_time_us();
	while (left > 0)
	{
		if (left > 1000)
			usleep(left / 2);
		else
			usleep(100);
		left = target - get_time_us();
	}
}

int	main(void)
{
	long	start;
	long	t0;
	long	err_usleep;
	long	err_precise;
	int		i;

	start = get_time_ms();
	printf("%ld début\n", get_time_ms() - start);
	err_usleep = 0;
	err_precise = 0;
	i = 0;
	while (i < 50)
	{
		t0 = get_time_us();
		usleep(10 * 1000);
		err_usleep += (get_time_us() - t0) - 10000;
		t0 = get_time_us();
		precise_sleep(10);
		err_precise += (get_time_us() - t0) - 10000;
		i++;
	}
	printf("dépassement moyen pour un sommeil de 10 ms :\n");
	printf("  usleep        : %ld us\n", err_usleep / 50);
	printf("  precise_sleep : %ld us\n", err_precise / 50);
	printf("%ld fin\n", get_time_ms() - start);
	return (0);
}
