#ifndef POSITION_HPP
#define POSITION_HPP
// Représentation d'un position avec cooredonnées (x, y) dans une grille précisée
// Avec une strcuture, on signale qu'il s'agit d'un simple conteneur de données (par defaut tout ets public)
// on prends const Position& pour éviter la copie et gagner en performance
struct Position {
    int x; // position horizontale
    int y; // position verticale
    int grille; // numlero de grille

    // Constructeur
    Position(int x = 0, int y = 0, int grille = 0) {
        this->x = x;
        this->y = y;
        this->grille = grille;
    }


    // on vérifie que 2 positions sont pareils
    bool operator==(const Position& autrePosition) const {
        if (x == autrePosition.x) {
            if (y == autrePosition.y) {
                if (grille == autrePosition.grille) {
                    return true;
                }
            }
        }
        return false;
    }
    // on vérifie que 2 positions sont différentes
    bool operator!=(const Position& autrePosition) const {
        if (x != autrePosition.x) {
            return true;
        }
        if (y != autrePosition.y) {
            return true;
        }
        if (grille != autrePosition.grille) {
            return true;
        }
        return false;
    }
};

#endif // POSITION_HPP