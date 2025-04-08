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
        Labyrinthe.hpp
        Position.hpp
        # Ajoute ici d'autres fichiers sources si nécessaire
        )

# Exécutable final
add_executable(labyrinthe ${SOURCES})
