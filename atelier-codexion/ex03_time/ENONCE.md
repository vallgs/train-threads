# ex03 — Le temps en millisecondes

**Objectif :** Savoir mesurer le temps et dormir sans dériver.

**Notions :** gettimeofday, struct timeval, usleep, timestamp relatif

## Énoncé

Écris `timing.c` avec trois fonctions :

- `long get_time_us(void)` et `long get_time_ms(void)` à partir de `gettimeofday`.
- `void precise_sleep(long ms)` : dort par petits morceaux et regarde l'heure à chaque réveil, jusqu'à atteindre la cible.

Dans le `main`, mesure 50 fois le dépassement (en µs) de `usleep(10000)` et de `precise_sleep(10)`, puis affiche la moyenne. Lance-le une fois au repos, puis pendant que le CPU est chargé (dans un autre terminal : `while true; do ./race; done`).

## Pour tester

```sh
cc -Wall -Wextra -Werror timing.c -o timing && ./timing
```

## Questions

1. Pourquoi le calcul `tv_sec * 1000 + tv_usec / 1000` doit-il se faire en `long` ?
2. Pourquoi les logs affichent-ils un temps **relatif** au début de la simulation ?
3. Au-delà de la précision, quel autre avantage y a-t-il à se réveiller souvent pendant un long sommeil ?

## Dans Codexion

Tous les logs commencent par un timestamp en ms, et le burnout doit être affiché **moins de 10 ms** après avoir eu lieu. Il te faut des fonctions de temps fiables dès le départ.

## Indices

- Sur ma machine de test, la différence au repos est faible (quelques centaines de µs). Elle grandit quand la machine est chargée. Ne sois pas surpris si tes chiffres varient.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
