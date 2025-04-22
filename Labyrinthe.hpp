#ifndef LABYRINTHE_HPP
#define LABYRINTHE_HPP

// vector<ElementTypé> me permets de stocker des éléments d'un type
// Je peux ajouter (push_back) ou supprimer (pop_back) et accéder à mes éléments facilement.
// C'est un stack ou pile (Last-In First-Out).s

#include <vector> // tableau dynamique
#include <string> // chaine caractere
#include "Position.hpp" // strcuture corrodonée & grille
#include "Objet.hpp" // Les objets trouvés dans les grilles à ramasser
#include "Couleurs.hpp"   // Lez couleurs d'affichage


using namespace std;



// Représentation d'une grille labyrinthe avec seulement les signatures des donctions.
// Les implementations seront dans Labyrinthe.cpp
class Labyrinthe {

public:
    // Constructeur
    Labyrinthe();

    // on charge le labyrinthe depuis le fichier sous format texte
    void chargerDepuisFichier(const string& nomFichier);

    // on résout le labyrinthe en trouvant un chemin
    bool resoudre();

    // On affiche la grille actuelle sur el terminal
    void afficher() const;

    // On modifie la grille en cours
    void setGrilleActuelle(int grille) {
        grilleActuelle = grille;
    }

private:
    // On stocke plusieurs grilles
    vector<vector<string>> labyrinthes;

    // Les positions de départ et d'arrivée dans la grille encours
    Position depart;
    Position arrivee;

    // Chemin trouvé pour aller de départ à arrivée
    vector<Position> chemin;

    // on liste des objets à ramasser
    vector<Objet> objets;

    // Indice de l'objet qu'on doit ramasser pour les avoir dans l'ordre
    int prochainObjet;

    // Numéro de la grille en cours
    int grilleActuelle;

    // on vérifie si une position est bonne (dans le périmetre dy labyrinthe en cours)
    bool estValide(const Position &pos) const;

    // On vérifie si c'est la case est un mur
    bool estObstacle(const Position &pos) const;

    // On cérifie si la case est un objet
    bool estObjet(const Position &pos) const;

    // On vérifie si la case est un monstre 'M'
    bool estMonstre(const Position &pos) const;

    // On vérifie si la case est un passage (1 ou 2)
    bool estPassage(const Position &pos) const;

    // On vérifie si la case est une bombe TNT 'T'
    bool estExplosion(const Position &pos) const;

    // On vérifie si on peut ramasser l'objet maintnant
    bool peutRamasserObjet(const Position& pos) const;

    // On récupère le caractère dans la grille à une position précise
    char getCase(const Position &pos) const;

    // On moodifie le caractère dans la grille à une position  précise
    void setCase(const Position &pos, char c);

    // Algorithme de backtracking (cherche un chemin)
    bool backtracking(const Position &pos, vector<Position> &cheminActuel);

    // on marque le chemin trouvé dans la grille en mettant '*' dans la case
    void marquerChemin();

    // on vérifie si tous les objets sont ramassés
    bool tousObjetsRamasses() const;
};



#endif