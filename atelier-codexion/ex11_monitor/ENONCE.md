# ex11 — Le thread monitor

**Objectif :** Détecter un burnout en moins de 10 ms et arrêter proprement tous les threads.

**Notions :** surveillance, drapeau d'arrêt, sommeil interruptible, précision < 10 ms

## Énoncé

4 workers « compilent » régulièrement (toutes les 50, 70, 90 et 300 ms). À chaque compilation, ils mettent à jour `last_compile` et `compiles` sous leur propre mutex. Un thread **monitor** vérifie toutes les millisecondes :

- si un worker n'a pas recompilé depuis plus de `timeout` ms : afficher `X burned out` avec le retard de détection, et arrêter la simulation ;
- si tous ont compilé au moins `required` fois : arrêter la simulation.

Les workers dorment avec un `sleep_or_stop` qui vérifie régulièrement le drapeau d'arrêt. À la fin, tout est joint, détruit et libéré.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread monitor.c -o monitor
./monitor 250 5 | tail -2   # le worker 4 burn out
./monitor 400 5 | tail -2   # tout le monde finit
valgrind --leak-check=full ./monitor 250 3
```

## Questions

1. Pourquoi un coder ne peut-il pas détecter lui-même son propre burnout ?
2. Pourquoi le monitor dort-il 1 ms entre deux tours, et pas 0 ou 50 ms ?
3. Que se passe-t-il si les workers font un simple `usleep(300000)` ?

## Dans Codexion

« **A separate monitor thread must detect burnout precisely and stop the simulation.** » Et les deux conditions d'arrêt du sujet sont exactement celles-ci : un burnout, ou tout le monde a atteint `number_of_compiles_required`.

## Indices

- Pense à un coder bloqué dans une file d'attente de dongle : il ne s'exécute pas, il ne peut rien vérifier.

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
