# ex05 — Provoquer puis soigner un deadlock

**Objectif :** Reconnaître un interblocage et le casser avec un ordre global de verrouillage.

**Notions :** deadlock, conditions de Coffman, ordre de verrouillage

## Énoncé

`deadlock.c` : Alice prend `usb_a` puis `usb_b`, Bob prend `usb_b` puis `usb_a`. Lance-le : il se fige (Ctrl+C pour sortir).

- Dessine sur papier qui détient quoi et qui attend quoi au moment du blocage.
- Cite les **4 conditions de Coffman** et montre que les 4 sont réunies ici.
- Corrige le programme **sans supprimer aucun des deux verrous**. Il doit afficher `200 / 200`.

## Pour tester

```sh
cc -Wall -Wextra -Werror -pthread deadlock.c -o deadlock
timeout 3 ./deadlock; echo "code de sortie : $?  (124 = bloqué)"
```

## Questions

1. Laquelle des 4 conditions ta correction casse-t-elle ?
2. Pourquoi le `usleep(1000)` entre les deux `lock` rend-il le bug quasi systématique ?

## Dans Codexion

Le README doit avoir une section **« Blocking cases handled »** qui parle de prévention des deadlocks et des conditions de Coffman. Cet exercice t'en donne le vocabulaire.

## Indices

- Si tout le monde prend les ressources dans le même ordre, peut-on encore former un cycle d'attente ?

## Correction

Les réponses aux questions sont dans le cahier en ligne ; le code corrigé est dans `correction/`. Essaie d'abord seul !
