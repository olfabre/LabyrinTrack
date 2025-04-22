#include "Labyrinthe.hpp"
#include <fstream> // pour lire un fichier de grilles
#include <iostream> // pour afficher sur le terminal
#include <algorithm> // pour utiliser la fonction find dans les vecteurs

using namespace std;

// Constructeur
Labyrinthe::Labyrinthe()
{
    // Initialisation variables prochainObjet et grilleActuelle à 0
    prochainObjet = 0;
    grilleActuelle = 0;

    // Ajouter les objets à ramasser dans le labyrinthe

    // Ajouter une épée  E dans la grille N°0
    objets.push_back(Objet('E', 0));

    // Ajouter un bouclier B dans la grille N°1
    objets.push_back(Objet('B', 1));

    // Ajouter une clé C dans la grille N°3
    objets.push_back(Objet('C', 3));
}


// chargerDepuisFichier
// Lit le fichier texte et construit les différentes grilles
void Labyrinthe::chargerDepuisFichier(const string& nomFichier) {

    ifstream fichier(nomFichier);

    // si erreur
    if (!fichier) {
        throw runtime_error("Impossible le lire le fichier " + nomFichier);
    }

    string ligne; // va contenir chaque ligne du fichier.
    vector<string> grille;        // stocke les lignes d’un labyrinthe ce que j'appelle la grille
    vector<string> grilleApresExplosion; // stocke la grille après exploqion qui ets une grille spéciale (grille N°1 après explosion de la bombe T)
    int grilleCourante = 0;       // compte combien de grilles on a lu
    bool premierMur = true;       // c'st pour ignorer la toute première ligne de séparation ##########

    // lecture ligne par ligne
    while (getline(fichier, ligne)) {

        // on ignonre les lignes vides
        if (ligne.empty())
            continue;

        // on détecte la lignea de séparation ####
        if (ligne.find("####################") != string::npos) {
            if (premierMur) {
                premierMur = false; // ignore la première séparation.
            } else if (!grille.empty()) { // Sinon, on ajoute la grille qu’on a finie à labyrinthes.
                labyrinthes.push_back(grille);
                grille.clear();                // on vide la grille pour préparer la suivante
                grilleCourante++;
            }
        } else {
            // Gestion de la grille après explosion (en double sur le fichier)

            // si tabulation alors c’est une ligne spéciale (2 grilles côte à côte).
            size_t posTab = ligne.find('\t');
            if (grilleCourante == 1 && posTab != string::npos) {

                // On coupe la ligne en deux
                // on prends les 20 premiers caractères pour la grille normale.
                grille.push_back(ligne.substr(0, 20));
                // on prends les 20 caractères après la tabulation pour la grille après explosion.
                grilleApresExplosion.push_back(ligne.substr(posTab + 1, 20));
            } else {
                // sinon, on prend les 20 premiers caractères pour grille normale sans grille côte à côte
                grille.push_back(ligne.substr(0, 20));
            }
        }
    }

    // on ajoute la grille après explosion T si elle existe
    if (!grilleApresExplosion.empty()) {
        labyrinthes.push_back(grilleApresExplosion);
    }

    // on ajoute la dernière grille
    if (!grille.empty()) {
        labyrinthes.push_back(grille);
    }

    // Échanger la grille 3 (index 2) et la grille 4 (index 3)
    if (labyrinthes.size() >= 4) {
        swap(labyrinthes[2], labyrinthes[3]);
    }


    // le but est de trouver et affecter les points de daprt et d'arrivée
    // pour chaque grille
    for (size_t numeroGrille = 0; numeroGrille < labyrinthes.size(); ++numeroGrille) {
        //pour chaque ligne
        for (size_t ligneGrille = 0; ligneGrille < labyrinthes[numeroGrille].size(); ++ligneGrille) {
            // pour chaque colonne
            for (size_t colonneGrille = 0; colonneGrille < labyrinthes[numeroGrille][ligneGrille].size(); ++colonneGrille) {

                // on recup le caractere de la case
                char caratereCase = labyrinthes[numeroGrille][ligneGrille][colonneGrille];
                if (numeroGrille == 0) { // Grille 1
                    if (caratereCase == 'D') depart = Position(colonneGrille, ligneGrille, numeroGrille);
                    else if (caratereCase == '1') arrivee = Position(colonneGrille, ligneGrille, numeroGrille);
                } else if (numeroGrille == 1) { // grille 2
                    if (caratereCase == '1') depart = Position(colonneGrille, ligneGrille, numeroGrille);
                    else if (caratereCase == 'T') arrivee = Position(colonneGrille, ligneGrille, numeroGrille);
                } else if (numeroGrille == 2) { // grille 3
                    if (caratereCase == 'T') depart = Position(colonneGrille,ligneGrille, numeroGrille);
                    else if (caratereCase == '2') arrivee = Position(colonneGrille, ligneGrille, numeroGrille);
                } else if (numeroGrille == 3) { // grille 4
                    if (caratereCase == '2') depart = Position(colonneGrille, ligneGrille, numeroGrille);
                    else if (caratereCase == 'A') arrivee = Position(colonneGrille, ligneGrille, numeroGrille);
                }
            }
        }
    }
}

// Attention: Fonction juste affiche le labyrinthe mais ne le modifie pas!
void Labyrinthe::afficher() const
{
    // Bordures haut et bas
    string bordureHaut = "+" + string(20, '-') + "+";
    string bordureBas = "+" + string(20, '-') + "+";

    cout << CYAN << "Affichage Grille " << grilleActuelle + 1 << RESET << endl;

    cout << BLEU << bordureHaut << RESET << endl;

    // on parcourir chaque ligne du labyrinthe en cours
    for (int i = 0; i < labyrinthes[grilleActuelle].size(); i++)
    {
        // Récupérer la ligne actuelle
        string ligne = labyrinthes[grilleActuelle][i];

        // Commencer la ligne avec une bordure gauche (| en bleu)
        string ligneModifiee = BLEU + "|" + RESET;

        // Parcourir chaque caractère de la ligne
        for (int j = 0; j < ligne.size(); j++)
        {
            char caratereCase = ligne[j]; // recupere le caractère à la position j

            // Si c'est un mur
            if (caratereCase == '#')
            {
                ligneModifiee += BLEU;  // Met en bleu
                ligneModifiee += caratereCase;     // Ajoute le caractère
                ligneModifiee += RESET; // Remet la couleur normale
            }

                // Si c'est un chemin
            else if (caratereCase == '*')
            {
                ligneModifiee += JAUNE;
                ligneModifiee += caratereCase;
                ligneModifiee += RESET;
            }

                // Si c'est une épée
            else if (caratereCase == 'E')
            {
                ligneModifiee += VERT;
                ligneModifiee += caratereCase;
                ligneModifiee += RESET;
            }


                // Si c'est un bouclier
            else if (caratereCase == 'B')
            {
                ligneModifiee += CYAN;
                ligneModifiee += caratereCase;
                ligneModifiee += RESET;
            }

                // Si c'est une clé
            else if (caratereCase == 'C')
            {
                ligneModifiee += MAGENTA;
                ligneModifiee += caratereCase;
                ligneModifiee += RESET;
            }

                // Si c'est un monstre
            else if (caratereCase == 'M')
            {
                ligneModifiee += ROUGE;
                ligneModifiee += caratereCase;
                ligneModifiee += RESET;
            }

                // Si c'est une bombe ou explosion
            else if (caratereCase == 'T')
            {
                ligneModifiee += ROUGE;
                ligneModifiee += caratereCase;
                ligneModifiee += RESET;
            }

                // Sinon, c'est un espace ou autre chose
            else
            {
                ligneModifiee += caratereCase; // j' ajoute le caractère sans couleur
            }
        }

        // terminer la ligne avec une bordure droite (| en bleu)
        ligneModifiee += BLEU + "|" + RESET;

        // Afficher la ligne modifiée
        cout << ligneModifiee << endl;
    }

    // bordure du bas
    cout << BLEU << bordureBas << RESET << endl;
    cout << endl;
}


// on verifie si une position est valide
bool Labyrinthe::estValide(const Position &position) const
{
    // Vérifie si le numreo de la grille est correct
    if (position.grille < 0) {
        return false;
    }
    if (position.grille >= labyrinthes.size()) {
        return false;
    }

    // Vérifie si la position en hauteur (ligne) est correcte
    if (position.y < 0) {
        return false;
    }
    if (position.y >= labyrinthes[position.grille].size()) {
        return false;
    }

    // Vérifie si la position en largeur (colonne) est correcte
    if (position.x < 0) {
        return false;
    }
    if (position.x >= labyrinthes[position.grille][position.y].size()) {
        return false;
    }

    // Si tout est bon, c est valide
    return true;
}





// on verifie si une position est obstacle
bool Labyrinthe::estObstacle(const Position &position) const
{
    // On vérifie en 1er si la position valide
    if (estValide(position) == false)
    {
        return true; // Si invalide c'est un obstacle
    }

    // On récupère le caractère à cette position
    char caratereCase;
    caratereCase = labyrinthes[position.grille][position.y][position.x];

    // Si mur # alors c'est un obstacle
    if (caratereCase == '#')
    {
        return true;
    }
    else
    {
        return false;
    }
}





bool Labyrinthe::estObjet(const Position &position) const
{
    // on vérifie en 1er si la position valide
    if (estValide(position) == false)
    {
        return false;
    }

    // On récupère le caractère à cette position
    char caratereCase;
    caratereCase = labyrinthes[position.grille][position.y][position.x];

    // Si le caractère est E, B, C 'B' alors c'est un objet
    if (caratereCase == 'E')
    {
        return true;
    }
    else if (caratereCase == 'B')
    {
        return true;
    }
    else if (caratereCase == 'C')
    {
        return true;
    }
    else
    {
        return false; // Sinon pas un objet
    }
}





// On verifie su c'est un passage d'une grille à une autre
bool Labyrinthe::estPassage(const Position &position) const
{
    // Vérifier si position valide
    if (estValide(position) == false)
    {
        return false;
    }

    // on récupére le caractère à cette position
    char caratereCase;
    caratereCase = labyrinthes[position.grille][position.y][position.x];

    // passage vers grille suivante
    if (caratereCase == '1')
    {
        return true;
    }
        // passage
    else if (caratereCase == '2')
    {
        return true;
    }
    else
    {
        return false; // Sinon, pas un passage
    }
}

// On vérifie si c'est une bombe TNT
bool Labyrinthe::estExplosion(const Position &position) const
{
    // si la position valide
    if (estValide(position) == false)
    {
        return false;
    }

    // On récupère le caractère à cette position dans la grille
    char caratereCase;
    caratereCase = labyrinthes[position.grille][position.y][position.x];

    // Si le caractère est T, alors c'est une explosion
    if (caratereCase == 'T')
    {
        return true;
    }
    else
    {
        return false;
    }
}



// On verofoe si cest un monstre
bool Labyrinthe::estMonstre(const Position &position) const
{
    // si position valide
    if (estValide(position) == false)
    {
        return false;
    }

    // on récupére le caractère à cette position
    char caratereCase;
    caratereCase = labyrinthes[position.grille][position.y][position.x];

    // Vérifier si le caractère est 'M' (monstre)
    if (caratereCase == 'M')
    {
        return true; // c'est un monstre
    }
    else
    {
        return false;
    }
}




// on recupere le caractere à une position d'une grille
char Labyrinthe::getCase(const Position &position) const
{
    // si la position valide
    if (estValide(position) == false)
    {
        return '#'; // Si position invalide, on retourne un mur #
    }

    // on récupérer le caractère à cette position
    char caratereCase;
    caratereCase = labyrinthes[position.grille][position.y][position.x];

    // Retourner le caractère
    return caratereCase;
}








void Labyrinthe::setCase(const Position &position, char c)
{
    // Vérifier  position  valide
    if (estValide(position) == true)
    {
        // Si position valide, on change le caractère à cette position
        labyrinthes[position.grille][position.y][position.x] = c;
    }
    // Sinon, on ne fait rien (car la position est invalide)

}

// marquage du chemin trouvé
void Labyrinthe::marquerChemin()
{
    // Parcour toutes les positions du chemin trouvé
    for (int i = 0; i < chemin.size(); i++)
    {
        // prnends la position actuelle dans le chemin
        Position postion = chemin[i];

        // Vérifie si la position est valide
        if (estValide(postion) == true)
        {
            // Vérifie si la case est vide
            if (labyrinthes[postion.grille][postion.y][postion.x] == ' ')
            {
                // Marque la case avec une *
                labyrinthes[postion.grille][postion.y][postion.x] = '*';
            }
        }
    }
}




// verifie si c'est un objet à ramasser
bool Labyrinthe::peutRamasserObjet(const Position& position) const
{
    // Vérifie si position contient un objet
    if (estObjet(position) == false)
    {
        return false; // Si ce n'est pas un objet, on ne peut rien ramasser !
    }

    // Récupére le symbole de lo'bjet à cette position
    char symbole;
    symbole = labyrinthes[position.grille][position.y][position.x];

    // on vérifie selon la grille en cours
    if (grilleActuelle == 0)
    {
        // 1ere grille : on peut ramasser l'épée E et si elle n'est pas encore ramassée
        if (symbole == 'E' && objets[0].ramasse == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (grilleActuelle == 1)
    {
        // 2eme grille: on peut ramasser le bouclier B et si on a déjà l'épée E
        // et si le bouclier n'a pas encore été ramassé
        if (symbole == 'B' && objets[0].ramasse == true && objets[1].ramasse == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (grilleActuelle == 3)
    {
        // 4ème  grille : on peut ramasser la clé C si l'épée E et le bouclier B sont déjà ramassés
        // et que la clé n'a pas encore été ramassée
        if (symbole == 'C' && objets[0].ramasse == true && objets[1].ramasse == true && objets[2].ramasse == false)
        {
            return true;
        }
    }

    // Dans les autres grilles, on ne ramasse rien
    return false;

}



// verifie si tous les objets ont été ramassés
bool Labyrinthe::tousObjetsRamasses() const
{
    // on vérifie pour la grille 0
    if (grilleActuelle == 0)
    {
        // épée E soit ramassée
        if (objets[0].ramasse == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

        // on vérifie pour la grille 1
    else if (grilleActuelle == 1)
    {
        // bouclier B soit ramassé
        if (objets[1].ramasse == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

        //  on vérifie pour la grille 2
    else if (grilleActuelle == 2)
    {
        // Pas d'objet à rpasser donc tjrs vraie
        return true;
    }

        // on vérifie pour la grille 4
    else if (grilleActuelle == 3)
    {
        // Besoin que la clé C soit ramassée
        if (objets[2].ramasse == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // Si aucune condition
    return false;
}





// Algo backtracking par une fonction récursive
bool Labyrinthe::backtracking(const Position &position, vector<Position> &cheminActuel)
{
    // on vérifie si on est arrivé à la destination A (fin de la recursive)
    if (position == arrivee)
    {
        // on vérifie si tous les objets sont ramassés ou si on est à la grille 3 (grille speciale après explosion)
        if (tousObjetsRamasses() == true || grilleActuelle == 3)
        {
            // on copie le chemin actuel dans le chemin final
            chemin = cheminActuel;
            return true; // on retourne que la Solution a ete trouvée
        }
    }

    // on verifie si position invalide, un obstacle ou un monstre (sauf dans la grille 3)
    if (estValide(position) == false)
    {
        return false;
    }
    if (estObstacle(position) == true)
    {
        return false;
    }
    if (grilleActuelle != 3 && estMonstre(position) == true)
    {
        return false;
    }

    // On vérifie que cette position a été déjà empruntée pour éviter de tourner en rond
    for (int i = 0; i < cheminActuel.size(); i++)
    {
        if (cheminActuel[i] == position)
        {
            return false; // Position déjà visitée
        }
    }

    // Si on est sur une case avec un objet, on le ramasse
    if (estObjet(position) == true)
    {
        char symbole = getCase(position);

        // on recherche l'objet correspondant
        for (int i = 0; i < objets.size(); i++)
        {
            if (objets[i].symbole == symbole && objets[i].grille == position.grille && objets[i].ramasse == false)
            {
                objets[i].ramasse = true; // on ramase l'objet
                break;
            }
        }
    }

    // on ajoute la position en cours au chemin
    cheminActuel.push_back(position);

    // Recursivité
    // On essayer de se déplace dans les 4 directions dans l'ordre : droite, gauche, bas, haut
    Position droite(position.x + 1, position.y, position.grille);
    Position gauche(position.x - 1, position.y, position.grille);
    Position bas(position.x, position.y + 1, position.grille);
    Position haut(position.x, position.y - 1, position.grille);

    // on prends la  direction droite
    if (backtracking(droite, cheminActuel) == true)
    {
        return true;
    }
    // on prends la direction gauche
    if (backtracking(gauche, cheminActuel) == true)
    {
        return true;
    }
    // on prends la  direction bas
    if (backtracking(bas, cheminActuel) == true)
    {
        return true;
    }
    // Essayer direction haut
    if (backtracking(haut, cheminActuel) == true)
    {
        return true;
    }

    // Si aucune direction ne marche, revenir en arrière.
    // on reprends la position de la pile (premier entré premier sorti)
    cheminActuel.pop_back();

    // Si on avait ramassé un objet ici, on le repose (annule le ramassage)
    if (estObjet(position) == true)
    {
        char symbole = getCase(position);

        for (int i = 0; i < objets.size(); i++)
        {
            if (objets[i].symbole == symbole && objets[i].grille == position.grille && objets[i].ramasse == true)
            {
                objets[i].ramasse = false; // je repose l'objet
                break;
            }
        }
    }

    return false; // Aucun chemin trouvé depuis lz position en cours
}


// Ajoute cette fonction pour le backtracking entre T et 2 dans la grille speciale (grille 3)
bool Labyrinthe::backtrackingGrilleParallele(const Position &position, vector<Position> &cheminActuel)
{
    if (getCase(position) == '2')  // Cas d'arrêt, on arrive à la case 2
    {
        chemin = cheminActuel;  // Sauvegarder le chemin trouvé
        return true;
    }

    if (!estValide(position))  // Position invalide
    {
        return false;
    }

    if (estObstacle(position))  // Mur
    {
        return false;
    }

    // Éviter de repasser par la même case
    for (int i = 0; i < cheminActuel.size(); i++)
    {
        if (cheminActuel[i] == position)
        {
            return false;
        }
    }

    cheminActuel.push_back(position);  // Ajouter la position actuelle au chemin

    // Essayer les 4 directions
    Position droite(position.x + 1, position.y, position.grille);
    Position gauche(position.x - 1, position.y, position.grille);
    Position bas(position.x, position.y + 1, position.grille);
    Position haut(position.x, position.y - 1, position.grille);

    if (backtrackingGrilleParallele(droite, cheminActuel)) return true;
    if (backtrackingGrilleParallele(gauche, cheminActuel)) return true;
    if (backtrackingGrilleParallele(bas, cheminActuel)) return true;
    if (backtrackingGrilleParallele(haut, cheminActuel)) return true;

    cheminActuel.pop_back();  // Revenir en arrière si aucune direction ne marche
    return false;
}




// fonction récursive pour trouver le chemin avec le backtracking
bool Labyrinthe::trouverChemin()
{
    vector<Position> cheminActuel;
    chemin.clear();

    for (size_t y = 0; y < labyrinthes[grilleActuelle].size(); ++y)
    {
        for (size_t x = 0; x < labyrinthes[grilleActuelle][y].size(); ++x)
        {
            if (labyrinthes[grilleActuelle][y][x] == '*')
            {
                labyrinthes[grilleActuelle][y][x] = ' ';
            }
        }
    }

    for (size_t y = 0; y < labyrinthes[grilleActuelle].size(); ++y)
    {
        for (size_t x = 0; x < labyrinthes[grilleActuelle][y].size(); ++x)
        {
            char c = labyrinthes[grilleActuelle][y][x];
            if (grilleActuelle == 0)
            {
                if (c == 'D') depart = Position(x, y, grilleActuelle);
                else if (c == '1') arrivee = Position(x, y, grilleActuelle);
            }
            else if (grilleActuelle == 1)
            {
                if (c == '1') depart = Position(x, y, grilleActuelle);
                else if (c == 'T') arrivee = Position(x, y, grilleActuelle);
            }
            else if (grilleActuelle == 3)
            {
                if (c == '2') depart = Position(x, y, grilleActuelle);
                else if (c == 'A') arrivee = Position(x, y, grilleActuelle);
            }
        }
    }

    cout << "\nGrille en cours: " << grilleActuelle + 1 << endl;
    cout << "Départ: (" << depart.x << ", " << depart.y << ")" << endl;
    cout << "Arrivée: (" << arrivee.x << ", " << arrivee.y << ")" << endl;

    bool solutionTrouvee = backtracking(depart, cheminActuel);

    if (solutionTrouvee)
    {
        marquerChemin();
        afficher();

        if (grilleActuelle == 0)
        {
            grilleActuelle = 1;
            return trouverChemin();
        }
        else if (grilleActuelle == 1)
        {
            //cout << CYAN << "Affichage Grille 2 avant explosion" << RESET << endl;
            //afficher();

            grilleActuelle = 2;

            // Ancienne position de T (dans grille 1)
            // on trouve ancienne position de T dans grille 1
            Position anciennePosT;
            for (int y = 0; y < labyrinthes[1].size(); y++)
            {
                for (int x = 0; x < labyrinthes[1][y].size(); x++)
                {
                    if (labyrinthes[1][y][x] == 'T')
                    {
                        anciennePosT = Position(x, y, 2); // On mémorise pour la grille 2
                    }
                }
            }

            // on trouve position de 2 dans la grille 2
            Position position2;
            for (int y = 0; y < labyrinthes[2].size(); y++)
            {
                for (int x = 0; x < labyrinthes[2][y].size(); x++)
                {
                    if (labyrinthes[2][y][x] == '2')
                    {
                        position2 = Position(x, y, 2);
                    }
                }
            }

            // Backtracking entre anciennePosT et '2'
            vector<Position> cheminParallele;
            backtrackingGrilleParallele(anciennePosT, cheminParallele);

            // on marque le chemin trouvé entre T et 2
            for (int i = 0; i < cheminParallele.size(); i++)
            {
                Position pos = cheminParallele[i];
                if (labyrinthes[2][pos.y][pos.x] == ' ' || labyrinthes[2][pos.y][pos.x] == '2')
                {
                    labyrinthes[2][pos.y][pos.x] = '*';
                }
            }

            cout << CYAN << "Affichage Grille 2 après explosion" << RESET << endl;
            afficher();

            grilleActuelle = 3;
            return trouverChemin();
        }
    }
    else
    {
        cout << "Pas de solution trouvée pour l'étage " << grilleActuelle + 1 << endl;
    }

    return solutionTrouvee;
}