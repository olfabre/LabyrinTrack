#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

// Codes de couleur ANSI
const string RESET   = "\033[0m";
const string BLANC = "\033[97m";
const string ROUGE   = "\033[31m";
const string VERT    = "\033[32m";
const string JAUNE   = "\033[33m";
const string BLEU    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";


// Directions de déplacement
const vector<pair<int,int>> directions = {
        {-1, 0}, // on monte :   on passe de la ligne i−1, colonne j reste la même
        { 0, 1}, // on va a droite : on passe de la colonne  j+1, ligne i reste la même
        { 1, 0}, // on descend: on passe de la ligne i+1, colonne j reste la même
        { 0,-1} // on av à gauche :  on passe de la colonne j−1, ligne i reste la même
};

// Structure case
struct Case {
    bool estMur    = false; // interdite
    bool estDepart = false; // obligatoire
    bool estArrivee = false; // obligatoire
    bool estObjetAPrendre = false;  // obligatoire
    bool estMine = false;  // interdite
};


// Backtracking en respectant les contraintes
bool resoudreLabyrinthe(const vector<vector<Case>>& labyrinthe, vector<vector<bool>>& isCaseVisitee, int caseActuelle_x, int caseActuelle_y, int caseArrivee_x, int caseArrivee_y, vector<pair<int,int>>& cheminParcouru, bool objetRamasse, bool fautIlRamasserObjet)
{
    int nbrLigne = labyrinthe.size();
    int nbrColonne = labyrinthe[0].size();

    //if (caseActuelle_x < 0 || caseActuelle_x >= nbrLigne || caseActuelle_y < 0 || caseActuelle_y >= nbrColonne) return false;


    const Case& caractereLu = labyrinthe[caseActuelle_x][caseActuelle_y];

    // condition d'arret
    if (caractereLu.estMur || caractereLu.estMine || isCaseVisitee[caseActuelle_x][caseActuelle_y]) {
        return false;
    }

    // case que je visite
    isCaseVisitee[caseActuelle_x][caseActuelle_y] = true;

    // on enregistre dans le vecteur les coordonnée x y
    cheminParcouru.emplace_back(caseActuelle_x, caseActuelle_y);


    if (caractereLu.estObjetAPrendre){
        objetRamasse = true;
    }

    // condition d'arret
    if (caseActuelle_x == caseArrivee_x && caseActuelle_y == caseArrivee_y) {
        if (!fautIlRamasserObjet || objetRamasse) return true;
    }

    // recursive pour parcourir les 4 directions
    for (int i = 0; i < 4; ++i) {



        int nouvelleCoordonnee_x = caseActuelle_x + directions[i].first;
        int nouvelleCoordonnee_y = caseActuelle_y + directions[i].second;

        // appel recurvif avec les nouvevvelles coordonnées
        bool cheminTrouve = resoudreLabyrinthe(labyrinthe,isCaseVisitee,nouvelleCoordonnee_x,nouvelleCoordonnee_y,caseArrivee_x,caseArrivee_y,cheminParcouru,objetRamasse,fautIlRamasserObjet);

        //condition d'arret avec un chemin trouvé
        if (cheminTrouve) {
            return true;
        }

    } // boucle for




    isCaseVisitee[caseActuelle_x][caseActuelle_y] = false;
    cheminParcouru.pop_back();
    return false;
} // fin du backtracking




int main() {

     // lecture
    ifstream fin("labyrinthe.txt");


    vector<vector<string>> labyrinthes; // labyrinthes[z] 2 dimensions
    vector<string> labyrintheEnCoursDeLecture;
    string ligneEnCoursDeLecture;

    while (getline(fin, ligneEnCoursDeLecture)) {

        if (ligneEnCoursDeLecture.empty())
        {
            if (!labyrintheEnCoursDeLecture.empty()) {
                labyrinthes.push_back(labyrintheEnCoursDeLecture);
                labyrintheEnCoursDeLecture.clear();
            }
        } else {

            labyrintheEnCoursDeLecture.push_back(ligneEnCoursDeLecture);
        }


    } // fin boucle while


    if (!labyrintheEnCoursDeLecture.empty()) {
        labyrinthes.push_back(labyrintheEnCoursDeLecture);
    }

    fin.close(); // ferme le flux


    // démarrage du chrono
    auto chrono_debut = chrono::steady_clock::now();

    for (int numero = 0; numero < labyrinthes.size(); ++numero) {



        vector<string> labyrintheEnCours = labyrinthes[numero];

        int nbrLigneTotal = labyrintheEnCours.size();
        int nbrColonnesTotal = labyrintheEnCours[0].size(); // premiere ligne nbr de colonne = nbr de caractere



        // Conversion en Case coordonnées
        vector<vector<Case>> labyrinthe(nbrLigneTotal, vector<Case>(nbrColonnesTotal));

        // initialise mes poinst de départ et d'arrivée
        int caseDepart_x=0, caseDepart_y=-0, caseArrivee_x=0, caseArrivee_y=0;
        bool fautIlRamasserObjet = false;


        for (int i = 0; i < nbrLigneTotal; ++i)
        {
            for (int j = 0; j < nbrColonnesTotal; ++j)
            {
                char caractereLu = labyrintheEnCours[i][j];

                Case& Case = labyrinthe[i][j];

                if (caractereLu == '#'){
                    Case.estMur = true;
                }
                else if (caractereLu == 'D'){
                    Case.estDepart = true;
                    caseDepart_x=i;
                    caseDepart_y=j;
                }
                else if (caractereLu == '1') {

                      if (numero == 0){
                          Case.estArrivee = true;
                          caseArrivee_x=i;
                          caseArrivee_y=j;
                      }
                      if (numero == 1){
                          Case.estDepart = true;
                          caseDepart_x=i;
                          caseDepart_y=j;
                      }
                      if (numero == 2){
                          // il ne se passe rien car on part de T
                      }

                }
                else if (caractereLu == 'T'){

                      if (numero == 1){
                          Case.estArrivee = true;
                          caseArrivee_x=i;
                          caseArrivee_y=j;
                      }
                      if (numero == 2){
                          Case.estDepart = true;
                          caseDepart_x=i;
                          caseDepart_y=j;
                      }

                }
                else if (caractereLu == '2'){

                      if (numero == 1){
                          // il en se passe rien car on arrive à T
                      }
                      if (numero == 2){
                          Case.estArrivee = true;
                          caseArrivee_x=i;
                          caseArrivee_y=j;
                      }
                      if (numero == 3){
                          Case.estDepart = true;
                          caseDepart_x=i;
                          caseDepart_y=j;
                      }

                }
                else if (caractereLu == 'A'){

                      if (numero == 3){
                          Case.estArrivee = true;
                          caseArrivee_x=i;
                          caseArrivee_y=j;
                      }

                }
                else if (caractereLu == 'M'){
                    Case.estMine = true;
                }
                else if (caractereLu == 'E'){
                    Case.estObjetAPrendre = true;
                    fautIlRamasserObjet = true;

                }
                else if (caractereLu == 'B'){

                    if (numero == 1){
                        Case.estObjetAPrendre = true;
                        fautIlRamasserObjet = true;
                    }
                    if (numero == 2){
                        // rien ne se passe car le cheminParcouru est direct sans rammassage
                        fautIlRamasserObjet = false;

                    }

                }
                else if (caractereLu == 'C'){
                    Case.estObjetAPrendre    = true;
                    fautIlRamasserObjet = true;
                }





            } //fin boucle colonne

        } // fin boucle ligne



        cout << "\nLabyrinthe #" << (numero+1) << " (" << nbrLigneTotal << "x" << nbrColonnesTotal << ")\n";



        // Backtracking
        vector<vector<bool>> isCaseVisitee(nbrLigneTotal, vector<bool>(nbrColonnesTotal, false));

        vector<pair<int,int>> cheminParcouru; // (x,y)

        bool estResolu = resoudreLabyrinthe(labyrinthe, isCaseVisitee, caseDepart_x, caseDepart_y, caseArrivee_x, caseArrivee_y, cheminParcouru, labyrinthe[caseDepart_x][caseDepart_y].estObjetAPrendre, fautIlRamasserObjet);


        if (estResolu == false) {
               cout << "  Aucun chemin trouvé\n";
               continue; // on passe à l'autre labyrinthe
        }

        // affcihage du cheminParcouru trouvé
        for (int i = 0; i < cheminParcouru.size(); ++i) {

            // Récupérer les coordonnéee du point i du cheminParcouru
            pair<int,int> coordonnees = cheminParcouru[i];
            int x = coordonnees.first;   // ligne
            int y = coordonnees.second;  // colonne

            // Vérifier que la case est un espace ' '
            if (labyrintheEnCours[x][y] == ' ') {
                // Remplacer l'espace par une étoile '*'
                labyrintheEnCours[x][y] = '*';
            }
        }


// Affichage coloré ligne par ligne
        for (int i = 0; i < labyrintheEnCours.size(); ++i) {
            const string& ligne = labyrintheEnCours[i];
            // indentation
            cout << "  ";
            for (char c : ligne) {
                // choix de la couleur selon le caractère
                switch (c) {
                    case '#':
                        cout << BLANC << c << RESET;
                        break;
                    case 'M':  // mines en rouge
                        cout << ROUGE << c << RESET;
                        break;
                    case 'D':
                        // départ et arrivée en bleu
                        cout << BLEU << c << RESET;
                        break;
                    case '1':
                        cout << BLEU << c << RESET;
                        break;
                    case '2':
                        cout << BLEU << c << RESET;
                        break;
                    case 'T':
                        cout << BLEU << c << RESET;
                        break;
                    case 'A':
                        cout << BLEU << c << RESET;
                        break;

                    case 'E': case 'B': case 'C':
                        // objets à ramasser en vert
                        cout << VERT    << c << RESET;
                        break;
                    case '*':  // cheminParcouru trouvé en jaune vif (ici magenta pour contraster)
                        cout << CYAN << c << RESET;
                        break;
                    default:
                        // espaces et tout le reste
                        cout << c;
                }
            }
            cout << "\n";
        }






    } // fin de la boucle for (on boucle sur tous les labyrinthes)

    auto chrono_fin = std::chrono::steady_clock::now();
    auto duree_ms = std::chrono::duration_cast<std::chrono::milliseconds>(chrono_fin - chrono_debut).count();

    // affichage du temps total
    cout << "\nTemps total pour résoudre tous les labyrinthes : "
         << duree_ms << " ms\n";                               // <<<<<<<<<<<<<<<<


    return 0; // fin du main
}
