# ex00 — Premiers threads

**Objectif :** Lancer N threads qui se présentent, puis attendre qu'ils aient tous fini.

**Notions :** pthread_create, pthread_join, void *routine(void *)

## Énoncé

Écris `hello.c`, qui prend `N` en argument et crée `N` threads. Chaque thread affiche `Coder X : bonjour !` (X de 1 à N). Le `main` attend tous les threads avec `pthread_join`, puis affiche `main : tous les threads sont terminés`.

- Pas de variable globale.
- Chaque thread reçoit un pointeur vers sa propre structure `t_worker { pthread_t thread; int id; }`.
- Le tableau de `t_worker` est alloué avec `malloc` et libéré à la fin.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread hello.c -o hello
./hello 5
```

## Questions

1. Lance le programme plusieurs fois. L'ordre des lignes change-t-il ? Pourquoi ?
2. Retire les `pthread_join`. Que se passe-t-il, et pourquoi ?
3. Que retourne `pthread_create` en cas d'échec ? Que faut-il faire des threads déjà créés ?

## Dans Codexion

Le sujet exige que **chaque coder soit un thread**. Ta future structure `t_coder` démarrera exactement comme ce `t_worker` : un `pthread_t`, un numéro, puis un pointeur vers les données partagées.

## Indices

- La routine a toujours la signature `void *routine(void *arg)`. Dedans, on caste : `t_worker *w = (t_worker *)arg;`.
- On passe `&workers[i]` comme 4e argument de `pthread_create`.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
