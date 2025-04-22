#ifndef OBJET_HPP
#define OBJET_HPP
// Ce sont les instances des objets présents dans kes grilles E, B, C
class Objet {
public:
    char symbole;
    int grille;
    bool ramasse;

    // Constructeur
    Objet(char symbole, int grille) {
        this->symbole = symbole;
        this->grille  = grille;
        this->ramasse = false;
    }
};

#endif
