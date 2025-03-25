## Projet – Programmation parallèle Résolution d’un labyrinthe par backtracking avec collecte d’objets



### Implémentation 

L'implémentation sera réalisé en C++11. On utilisera une compilation qui a été recommandée pour le mode developpement

```bash
g++ -Wall -g3 main.cpp -o labyrinthe

ou plutôt

g++ -std=c++11 -Wall -g3 main.cpp -o labyrinthe
```



### Structure

J'ai créé une classe `Position` qui va représenter une cellule spécifique du labyrinthe (contient les coordonnées x et y, son état et les objets collectés (des booléens indiquant si chaque objet couronne, bouclier, épée est collecté).

J'ai créé une classe `Labyrinthe`qui charge et stocke la grille du labyrinthe depuis un fichier, identifie les points `D`  et `A`, vérifie les déplacements valides...



### Implémentation du backtracking séquentiel

ici nous allons essayer de coder un programme qui énumérer l'ensemble des  possibilités pour résoudre les différents labyrinthes. 

Je vais donc réaliser une fonctione récursive qui explore tous les chemins possibles du labyrinthe à partir de la position de départ `D`  jusqu'à la position finale `A` tout en collectant de façon obligatoire des objets (`C,B,E`) en revenant en arrière dès que l'on rencontre :

- un mur qui sera représenté par `#`
- une case déjà visitée
- une impasse donc un chemin sans issue















**Info**

On va enumérer l'ensemble des possibilités (backtracking).

Avant d'arriver au point d'arrivée, il doit collecter tous les d'objets a,b,c.

Deux méthodes pour paralleliser le back traking

Mettre le tracé du personnage.

10 points pour le backtracking 

10 points codes correcte, backtracking sequentiels, charte, présentation, classes

Dernière séance: 29 avril 









