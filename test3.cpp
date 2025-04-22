// backtracking_sequentiel.cpp
// Auteur : [Ton prénom et nom]
// Date : [Date]
// Résolution séquentielle d'un labyrinthe par backtracking avec gestion des labyrinthes multiples et génération d'un rapport texte

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Position {
    int x, y;
};

struct Contexte {
    int niveau;
    bool couronne;
    bool bouclier;
    bool epee;
};

vector<vector<string>> labyrinthes(4);
vector<vector<string>> parcours(4);
int hauteur, largeur;

bool estValide(int x, int y, vector<string>& lab) {
    return x >= 0 && x < hauteur && y >= 0 && y < largeur && lab[x][y] != '#' && lab[x][y] != 'M' && lab[x][y] != '*';
}

Position trouverCorrespondance(char marque, int niveauCible) {
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            if (labyrinthes[niveauCible][i][j] == marque) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

bool explorer(int x, int y, Contexte ctx) {
    vector<string>& lab = labyrinthes[ctx.niveau];
    vector<string>& chemin = parcours[ctx.niveau];

    if (!estValide(x, y, lab)) return false;

    char temp = lab[x][y];
    bool modifC = false, modifB = false, modifE = false;

    if (temp == 'C' && !ctx.couronne) { ctx.couronne = true; modifC = true; }
    if (temp == 'B' && !ctx.bouclier) { ctx.bouclier = true; modifB = true; }
    if (temp == 'E' && !ctx.epee)    { ctx.epee = true;    modifE = true; }

    if (temp == 'A' && ctx.couronne && ctx.bouclier && ctx.epee) {
        chemin[x][y] = '*';
        return true;
    }

    lab[x][y] = '*';
    chemin[x][y] = '*';

    // Exploration classique (autour de la case)
    bool trouve = false;
    if (explorer(x+1, y, ctx) || explorer(x-1, y, ctx) ||
        explorer(x, y+1, ctx) || explorer(x, y-1, ctx)) {
        return true;
    }

    // Transitions supplémentaires sans bloquer l'exploration locale
    if (temp == '1' && ctx.niveau == 0) {
        Position p = trouverCorrespondance('1', 1);
        if (p.x != -1 && p.y != -1) {
            Contexte ctxNext = ctx; ctxNext.niveau = 1;
            if (explorer(p.x, p.y, ctxNext)) return true;
        }
    }
    else if (temp == 'T' && ctx.niveau == 1) {
        Position p = trouverCorrespondance('T', 2);
        if (p.x != -1 && p.y != -1) {
            Contexte ctxNext = ctx; ctxNext.niveau = 2;
            if (explorer(p.x, p.y, ctxNext)) return true;
        }
    }
    else if (temp == '2' && ctx.niveau == 2) {
        Position p = trouverCorrespondance('2', 3);
        if (p.x != -1 && p.y != -1) {
            Contexte ctxNext = ctx; ctxNext.niveau = 3;
            if (explorer(p.x, p.y, ctxNext)) return true;
        }
    }

    lab[x][y] = temp;
    chemin[x][y] = temp;

    if (modifC) ctx.couronne = false;
    if (modifB) ctx.bouclier = false;
    if (modifE) ctx.epee = false;

    return false;
}

int main() {
    ifstream fichier("labyrinthe.txt");
    if (!fichier) {
        cerr << "Erreur ouverture fichier !" << endl;
        return 1;
    }

    string ligne;
    int idx = 0;
    while (getline(fichier, ligne)) {
        if (ligne.empty()) continue;
        if (ligne.find('\t') != string::npos) {
            labyrinthes[1].push_back(ligne.substr(0, ligne.find('\t')));
            labyrinthes[2].push_back(ligne.substr(ligne.find('\t')+1));
        } else {
            labyrinthes[idx].push_back(ligne);
            if (labyrinthes[idx].size() == 20) idx++;
        }
    }

    for (int i = 0; i < 4; ++i) parcours[i] = labyrinthes[i];
    hauteur = labyrinthes[0].size();
    largeur = labyrinthes[0][0].size();

    Position depart{-1, -1};
    for (int i = 0; i < hauteur; ++i)
        for (int j = 0; j < largeur; ++j)
            if (labyrinthes[0][i][j] == 'D') depart = {i, j};

    if (depart.x == -1 || depart.y == -1) {
        cerr << "Position de départ non trouvée !" << endl;
        return 1;
    }

    Contexte ctx = {0, false, false, false};
    bool trouve = explorer(depart.x, depart.y, ctx);

    ofstream rapport("rapport_labyrinthe.txt");
    rapport << "===== Rapport de Résolution du Labyrinthe =====\n\n";
    rapport << "Chemin trouvé : " << (trouve ? "OUI" : "NON") << "\n";
    rapport << "Objets requis collectés : ";
    rapport << ((ctx.couronne && ctx.bouclier && ctx.epee) ? "OUI" : "NON") << "\n\n";

    if (trouve) {
        for (int niv = 0; niv < 4; ++niv) {
            rapport << "======= Labyrinthe " << niv+1 << " =======\n";
            for (auto& ligne : parcours[niv]) rapport << ligne << "\n";
            rapport << "\n";
        }
    } else {
        rapport << "Aucun chemin valide trouvé.\n";
    }
    rapport.close();

    if (trouve) {
        for (int niv = 0; niv < 4; ++niv) {
            cout << "\n======= Labyrinthe " << niv+1 << " =======\n";
            for (auto& ligne : parcours[niv]) cout << ligne << endl;
        }
        cout << "\nUn rapport a été généré : rapport_labyrinthe.txt" << endl;
    } else {
        cout << "Aucun chemin valide trouvé !\nUn rapport a été généré : rapport_labyrinthe.txt" << endl;
    }

    return 0;
}