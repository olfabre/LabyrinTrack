#include "Labyrinthe.hpp"

int main() {
    Labyrinthe lab;

    if (!lab.chargerDepuisFichier("labyrinthe.txt")) {
        return -1;
    }

    lab.afficher();

    std::cout << "Départ : (" << lab.depart.x << ", " << lab.depart.y << ")" << std::endl;
    std::cout << "Arrivée : (" << lab.arrivee.x << ", " << lab.arrivee.y << ")" << std::endl;

    return 0;
}
