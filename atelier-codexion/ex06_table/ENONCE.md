# ex06 — La table ronde de dongles

**Objectif :** Passer de 2 verrous à N verrous disposés en cercle, comme dans Codexion.

**Notions :** N ressources en cercle, ordre par indice, cas N = 1

## Énoncé

Écris `table.c` : `./table N K`. Il y a N coders et N mutex (les dongles). Le coder `i` (0 à N-1) a le dongle `i` à gauche et `(i + 1) % N` à droite. Chaque coder fait K fois :

- prendre un dongle, afficher `has taken a dongle`, `usleep(1000)` ;
- prendre l'autre, afficher `has taken a dongle` puis `is compiling`, `usleep(2000)` ;
- relâcher les deux, afficher `is debugging`, `usleep(1000)`.

**Étape 1 :** version naïve (gauche puis droite). Vérifie que `./table 5 20` se bloque.
**Étape 2 :** corrige-la. Teste avec 2, 5 et 200 coders.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread table.c -o table
./table 5 20 | tail -3
./table 200 5 | tail -1
```

## Questions

1. Avec N = 1, gauche et droite sont le même dongle. Que se passe-t-il si un thread verrouille deux fois le même mutex ?
2. Propose une autre stratégie que « le plus petit indice d'abord » (indice : pairs et impairs).

## Dans Codexion

C'est la disposition exacte du sujet (le coder 1 est voisin du coder N). Attention : dans Codexion, un dongle ne sera pas qu'un mutex tenu pendant toute la compilation. Il aura un état (pris, cooldown, file d'attente) protégé par un mutex. C'est l'objet des exercices suivants.

## Indices

- Fais prendre à chacun le dongle d'**indice le plus petit** en premier. Regarde ce que ça change pour le dernier coder seulement.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
