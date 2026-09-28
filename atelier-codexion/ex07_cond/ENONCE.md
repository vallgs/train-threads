# ex07 — Variables de condition

**Objectif :** Attendre un événement sans boucler dans le vide, avec ou sans délai maximum.

**Notions :** pthread_cond_wait, broadcast / signal, pthread_cond_timedwait, struct timespec

## Énoncé

**Partie A, le coup de départ.** Une structure `t_gate { mutex, cond, int open, long start }`. 5 threads attendent que `open` passe à 1. Le `main` dort 300 ms, puis ouvre la porte et réveille tout le monde. Chaque thread affiche l'heure de son départ : tous doivent partir vers 300 ms.

**Partie B, attente limitée.** Écris `int wait_open_for(t_gate *g, long timeout_ms)` avec `pthread_cond_timedwait`. Si personne n'ouvre, la fonction doit rendre 0 après environ `timeout_ms`.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread cond.c -o cond && ./cond
```

## Questions

1. Pourquoi entourer `pthread_cond_wait` d'un `while` et pas d'un `if` ?
2. Pourquoi faut-il détenir le mutex au moment d'appeler `pthread_cond_wait` ?
3. Quelle différence entre `signal` et `broadcast` ? Lequel utiliser ici ?
4. `pthread_cond_timedwait` prend-il une durée ou une date ?

## Dans Codexion

Le sujet suggère une condition pour **gérer les files d'attente** des dongles. `timedwait` sera utile pour attendre la fin d'un cooldown, puisque personne ne « signale » qu'un cooldown est terminé.

## Indices

- `timedwait` attend une **date absolue** : maintenant + délai, convertie en `struct timespec` (`tv_sec`, `tv_nsec`).
- Il renvoie `ETIMEDOUT` (défini dans `errno.h`) quand la date est dépassée.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
