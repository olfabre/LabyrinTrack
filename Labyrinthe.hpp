#ifndef LABYRINTHE_HPP
#define LABYRINTHE_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Position.hpp"


class Labyrinthe {
public:
    // Grille du labyrinthe
    std::vector<std::string> grille;

    // Dimensions du labyrinthe
    int largeur;
    int hauteur;

    // Position de départ et d'arrivée
    Position depart{0, 0};
    Position arrivee{0, 0};

    Labyrinthe() : largeur(0), hauteur(0) {}

    // Charge le labyrinthe depuis un fichier texte
    // on lit et on parse le fichier avec un structure de matrice de caractères
    bool chargerDepuisFichier(const std::string &nomFichier) {
        std::ifstream fichier(nomFichier);
        if (!fichier.is_open()) {
            std::cerr << "Impossible d'ouvrir le fichier : " << nomFichier << std::endl;
            return false;
        }

        std::string ligne;
        grille.clear();

        while (getline(fichier, ligne)) {
            grille.push_back(ligne);
        }

        hauteur = grille.size();
        largeur = hauteur > 0 ? grille[0].size() : 0;

        // Identifier les positions de départ (D) et d'arrivée (A)
        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                if (grille[i][j] == 'D') {
                    depart = Position(i, j);
                } else if (grille[i][j] == 'A') {
                    arrivee = Position(i, j);
                }
            }
        }

        fichier.close();
        return true;
    }

    // Vérifie si une position est valide (dans la grille et non un mur)
    bool estValide(int x, int y) const {
        return x >= 0 && x < hauteur &&
               y >= 0 && y < largeur &&
               grille[x][y] != '#';
    }

    // Affiche le labyrinthe (pour debug)
    void afficher() const {
        for (const auto &ligne : grille) {
            std::cout << ligne << std::endl;
        }
    }
};

#endif