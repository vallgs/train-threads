# ex04 — Un logger thread-safe

**Objectif :** Afficher des lignes jamais mélangées, dans l'ordre, et plus rien après « burned out ».

**Notions :** mutex d'affichage, ordre des logs, arrêt propre

## Énoncé

Dans `logger.c`, chaque thread construit sa ligne en trois `printf`. Lance-le : les lignes se mélangent.

Écris un logger à partir d'une structure `t_log { pthread_mutex_t lock; long start; int stopped; }` :

- `int log_state(t_log *log, int id, const char *msg)` : affiche `<ts> <id> <msg>` si la simulation n'est pas arrêtée. Retourne 1 si elle l'est.
- `void log_final(t_log *log, int id, const char *msg)` : affiche le dernier message **et** arrête la simulation.

Test : 4 threads envoient 300 messages (avec `usleep(100)` entre deux). Après 10 ms, `main` appelle `log_final(&log, 3, "burned out")`. La dernière ligne affichée doit être celle-là.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread logger_fixed.c -o logger_fixed
./logger_fixed | tail -3
```

## Questions

1. Pourquoi prendre le timestamp **après** avoir verrouillé le mutex, et pas avant ?
2. Pourquoi afficher « burned out » et passer `stopped` à 1 dans la **même** section critique ?

## Dans Codexion

Le sujet exige que **les logs soient sérialisés par un mutex** et qu'aucun message ne soit mélangé. En pratique, les correcteurs vérifient aussi qu'aucune ligne n'apparaît après « burned out ».

## Indices

- Imagine : A lit l'heure (10 ms), puis est mis en pause. B lit l'heure (11 ms) et affiche. A reprend et affiche 10. Les timestamps reculent.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
