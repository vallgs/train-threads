# ex10 — Une file d'attente équitable

**Objectif :** N'accorder une ressource qu'au thread en tête de file, selon la politique choisie.

**Notions :** tas + mutex + cond, arbitrage FIFO / EDF, attendre son tour

## Énoncé

Une seule ressource, protégée par un mutex et une condition, avec le tas de l'ex09 comme file d'attente :

- `res_acquire(r, id, key)` : ajoute la demande à la file, attend **tant que la ressource est prise ou que ce n'est pas son tour**, puis retire sa demande et prend la ressource.
- `res_release(r)` : libère et réveille les threads en attente.

Scénario : `main` garde la ressource 100 ms. Pendant ce temps, 6 clients arrivent (client `i` à `i × 5` ms) avec une deadline `1000 - i × 10`. Enregistre l'ordre de service.

- `./fair_queue fifo` doit servir `1 2 3 4 5 6`.
- `./fair_queue edf` doit servir `6 5 4 3 2 1`.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread fair_queue.c -o fair_queue
./fair_queue fifo | tail -2
./fair_queue edf | tail -2
```

## Questions

1. Pourquoi `broadcast` et pas `signal` dans `res_release` ?
2. Si la simulation s'arrête alors que des threads attendent dans la file, que se passe-t-il ? Comment les faire sortir ?
3. Dans Codexion, chaque coder a besoin de **deux** dongles, chacun avec sa propre file. Qu'est-ce qui peut mal tourner si un coder garde le premier dongle longtemps en attendant le second ?

## Dans Codexion

« **Fair arbitration is mandatory** » : avec `fifo`, servir dans l'ordre d'arrivée ; avec `edf`, servir la deadline la plus proche. Cet exercice en est le prototype à une seule ressource.

## Indices

- Chaque thread n'a qu'une seule demande en cours : son `id` suffit pour savoir s'il est en tête (`queue.data[0].id == id`).

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
