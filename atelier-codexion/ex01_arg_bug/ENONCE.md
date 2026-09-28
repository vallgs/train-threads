# ex01 — Chasse au bug : l'argument partagé

**Objectif :** Comprendre pourquoi on ne passe jamais l'adresse d'une variable de boucle à un thread.

**Notions :** durée de vie, adresse partagée, data race

## Énoncé

Le fichier `arg_bug.c` fourni est censé afficher `je suis le thread 1` à `je suis le thread 4`, dans n'importe quel ordre. Lance-le une dizaine de fois.

- Note ce que tu observes (doublons ? des 0 ? des 5 ?).
- Explique précisément le problème.
- Corrige-le sans variable globale.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread arg_bug.c -o arg_bug
for i in 1 2 3 4 5; do ./arg_bug | sort | tr '\n' ' '; echo; done
```

## Questions

1. Tous les threads reçoivent-ils la même adresse ou des adresses différentes ?
2. Pourquoi voit-on parfois `0` alors que la boucle commence à 1 ?

## Dans Codexion

Chaque coder doit recevoir un pointeur vers **son propre** `t_coder` (dans un tableau alloué), jamais vers une variable de boucle.

## Indices

- Regarde ce que vaut `i` au moment où le thread lit `*(int *)arg`, et non au moment où il est créé.
- Il faut une case mémoire différente par thread, qui vit au moins aussi longtemps que lui.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
