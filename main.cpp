// Projet LabyrinTrack
// Algorithme de Backtracking Séquentielle
// Olivier Fabre
#include <iostream>
#include <string>
#include <chrono>
#include "Labyrinthe.hpp"

using namespace std;

int main()
{
    try {

        // on crée un objet de typpe Labyrinthe
        Labyrinthe labyrinthe;

        // On charge notre fichier grilles
        labyrinthe.chargerDepuisFichier("labyrinthe.txt");

        // Affichage des grilles prsent sur le fichier
        // Numérotés de bas en haut de 1 à 3. Le 4ème grille est la grille parallèle après explosion de la bombe
        cout << "\n\nAffichage des labyrinthes initiaux:" << endl;

        // Boucle pour afficher les grilles
        for (int i = 0; i < 4; ++i) {
            labyrinthe.setGrilleActuelle(i);
            labyrinthe.afficher();
        }


        cout << "\n\nAffichage des labyrinthes résolus:" << endl;
        labyrinthe.setGrilleActuelle(0);

        // Début du chrono
        auto debut = chrono::high_resolution_clock::now();

        // action: resoudre le labyrinthe
        bool resolution = labyrinthe.resoudre();

        // Fin du chrono
        auto fin = chrono::high_resolution_clock::now();

        // Calcul du temps écoulé
        auto duree = chrono::duration_cast<chrono::milliseconds>(fin - debut);



        if (resolution) {
            cout << "On a bien trouvé une solution pour chaque grille!" << endl;
        } else {
            cout << "On n'a pas trouvé de solution pour chaque grille!" << endl;
        }



        // On affiche le temps d'exécution
        cout << "Le temps d'exécution pour résoudre avec mon algorithme de backtracking séquentielle: "
             << duree.count() << " millisecondes" << endl;




        // gestion erreur pour debug
    } catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
        return 1;
    }

    return 0;
}
