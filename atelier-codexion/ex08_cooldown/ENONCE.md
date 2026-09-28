# ex08 — Un dongle avec temps de recharge

**Objectif :** Construire une ressource qui reste indisponible un certain temps après avoir été rendue.

**Notions :** état protégé par mutex, cond + timedwait, vérification automatique

## Énoncé

Crée `t_dongle { mutex, cond, int taken, long available_at, long cooldown }` et deux fonctions :

- `void dongle_take(t_dongle *d)` : bloque jusqu'à ce que le dongle soit libre **et** rechargé, puis le marque pris.
- `void dongle_release(t_dongle *d)` : le marque libre, fixe `available_at = maintenant + cooldown`, réveille les threads en attente.

Test : 3 threads utilisent le même dongle 5 fois chacun, le gardent 20 ms, cooldown de 50 ms. Enregistre chaque heure de prise et de rendu, puis vérifie à la fin que l'écart est toujours d'au moins 50 ms.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread cooldown.c -o cooldown && ./cooldown | tail -3
```

## Questions

1. Pourquoi le mutex n'est-il **pas** tenu pendant les 20 ms d'utilisation ?
2. Pourquoi utiliser `timedwait` jusqu'à `available_at` plutôt qu'un simple `cond_wait` ?

## Dans Codexion

« **Dongle cooldown is mandatory** » : après avoir été relâché, un dongle ne peut être repris avant `dongle_cooldown` ms. C'est exactement ce mécanisme.

## Indices

- Deux cas d'attente différents : le dongle est pris (quelqu'un fera un broadcast en le rendant), ou il est libre mais en recharge (personne ne préviendra, mais tu sais jusqu'à quand attendre).

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
