# Atelier Codexion : threads et mutex

12 exercices progressifs pour préparer le projet Codexion.
Chaque dossier contient `ENONCE.md`, parfois un code fourni dans `sujet/`, et la correction dans `correction/`.

| Exo | Sujet | Brique pour Codexion |
|-----|-------|----------------------|
| ex00 | Premiers threads | un coder = un thread |
| ex01 | Chasse au bug : l'argument partagé | passer ses données à un thread |
| ex02 | Race condition et premier mutex | protéger un état partagé |
| ex03 | Le temps en millisecondes | timestamps, sommeil précis |
| ex04 | Un logger thread-safe | logs sérialisés, rien après « burned out » |
| ex05 | Provoquer puis soigner un deadlock | conditions de Coffman |
| ex06 | La table ronde de dongles | N ressources en cercle, cas N = 1 |
| ex07 | Variables de condition | attente passive, timedwait |
| ex08 | Un dongle avec temps de recharge | cooldown |
| ex09 | Un tas binaire minimum | la priority queue imposée |
| ex10 | Une file d'attente équitable | arbitrage FIFO / EDF |
| ex11 | Le thread monitor | détection de burnout < 10 ms, arrêt propre |

## Compiler

```sh
cc -Wall -Wextra -Werror -pthread fichier.c -o prog
```

## Traquer les bugs

```sh
cc -g -fsanitize=thread -pthread fichier.c -o prog_tsan && ./prog_tsan   # data races
valgrind --tool=helgrind ./prog                                          # races, ordre des verrous
valgrind --leak-check=full ./prog                                        # fuites
```

Toutes les corrections ont été compilées avec les flags ci-dessus et passent `-fsanitize=thread` et valgrind sans erreur.

Rappel : ce sont des briques isolées. Réécris tout toi-même dans ton projet ; en soutenance, tu dois pouvoir expliquer chaque ligne.
