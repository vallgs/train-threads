# ex09 — Un tas binaire minimum

**Objectif :** Implémenter à la main la file de priorité qu'exige le sujet.

**Notions :** priority queue, tas dans un tableau, tie-breaker

## Énoncé

Pas de thread ici. Écris un tas minimum de `t_req { long key; unsigned long seq; int id; }` :

- `heap_init`, `heap_push`, `heap_pop`, et le sommet est `data[0]`.
- Comparaison `before(a, b)` : la plus petite `key` passe d'abord, et à égalité le plus petit `seq` (l'ordre d'arrivée).

Test : pousse 40 éléments avec des clés pseudo-aléatoires entre 0 et 9 (beaucoup d'égalités), dépile tout, vérifie que la sortie est triée par `(key, seq)`.

## Pour tester

```sh
cc -Wall -Wextra -Werror heap.c -o heap && ./heap | tail -1
```

## Questions

1. Quelle est la complexité de `push` et de `pop` ?
2. Comment représenter FIFO avec la même structure ? Et EDF ?
3. Pourquoi le sujet insiste-t-il sur un départage des égalités ?

## Dans Codexion

« **You must implement a priority queue (heap)** for FIFO/EDF scheduling. » Aucune file toute faite n'est autorisée.

## Indices

- Pour l'indice `i` : parent `(i - 1) / 2`, enfants `2i + 1` et `2i + 2`.
- push : on ajoute en bas et on fait remonter. pop : on met le dernier en haut et on fait descendre vers le plus prioritaire des deux enfants.
- `rand` n'est pas autorisé dans Codexion. Un générateur maison suffit : `x = x * 1103515245 + 12345`.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
