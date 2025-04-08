## Projet – Programmation parallèle Résolution d’un labyrinthe par backtracking avec collecte d’objets



### Implémentation 

L'implémentation sera réalisé en C++11. On utilisera une compilation qui a été recommandée pour le mode developpement

```bash
g++ -Wall -g3 main.cpp -o labyrinthe

ou plutôt

g++ -std=c++11 -Wall -g3 main.cpp -o labyrinthe
```

pour info, `-Wall` sert à **activer presque tous les avertissements (warnings)** lors de la compilation.



### Création CMakeLists

```txt
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(Labyrinthe)

# Standard C++11 et options de compilation
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Options de compilation similaires à ta commande
add_compile_options(-Wall -Wextra -Werror -g3)

# Fichiers sources (ajoute ici tous les fichiers nécessaires)
set(SOURCES
    main.cpp
    Labyrinthe.cpp
    # Ajoute ici d'autres fichiers sources si nécessaire
)

# Exécutable final
add_executable(labyrinthe ${SOURCES})

```



génération et compilation

```txt
mkdir build

cmake -S . -B build	Génère le projet dans le dossier build
cmake --build build	Compile le projet dans le dossier build
```







### Structure

J'ai créé une classe `Position` qui va représenter une cellule spécifique du labyrinthe (contient les coordonnées x et y, son état et les objets collectés (des booléens indiquant si chaque objet couronne, bouclier, épée est collecté).

J'ai créé une classe `Labyrinthe`qui charge et stocke la grille du labyrinthe depuis un fichier, identifie les points `D`  et `A`, vérifie les déplacements s'il sont valides ou pas...



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









