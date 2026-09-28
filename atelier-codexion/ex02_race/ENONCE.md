# ex02 — Race condition et premier mutex

**Objectif :** Voir une race condition de ses yeux, puis la corriger avec un mutex.

**Notions :** pthread_mutex_t, lock / unlock, section critique, -fsanitize=thread

## Énoncé

Dans `race.c`, deux threads incrémentent le même compteur 1 000 000 de fois chacun. Le résultat attendu est 2 000 000.

- Lance-le 5 fois et note les résultats.
- Explique pourquoi `counter++` n'est pas une opération « atomique ».
- Ajoute un `pthread_mutex_t` **dans la structure** `t_data` : `init`, `lock`/`unlock` autour de `counter++`, `destroy` à la fin.
- Compile la version buguée avec `-fsanitize=thread` et lis le rapport.
- **Bonus :** mets le `lock` une seule fois autour de toute la boucle. Le résultat est juste, mais qu'as-tu perdu ?

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread race.c -o race && ./race
cc -g -fsanitize=thread -pthread race.c -o race_tsan && ./race_tsan
```

## Questions

1. Décompose `counter++` en instructions machine. Où deux threads peuvent-ils se marcher dessus ?
2. Pourquoi garder la section critique la plus courte possible ?

## Dans Codexion

Le sujet impose de protéger **l'état de chaque dongle par un mutex**. Même logique pour toute donnée lue par un thread et écrite par un autre : dernier début de compilation, nombre de compilations, drapeau d'arrêt.

## Indices

- `counter++` = lire la valeur, ajouter 1, réécrire. Si A et B lisent tous les deux 41 avant d'écrire, les deux écrivent 42 : une incrémentation est perdue.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
