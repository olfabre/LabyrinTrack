#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

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


// mutex global
mutex donnee;

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






// Traite un seul labyrinthe : conversion, backtracking, affichage
void traiterLabyrinthe(int numero, vector<string> labyrintheEnCours)
{
    int nbrLigne = labyrintheEnCours.size();
    int nbrColonne = labyrintheEnCours[0].size();

    vector<vector<Case>> labyrinthe(nbrLigne, vector<Case>(nbrColonne));

    int caseDepart_x=0;
    int caseDepart_y=0;
    int caseArrivee_x=0;
    int caseArrivee_y=0;
    bool fautIlRamasserObjet = false;

    for (int i = 0; i < nbrLigne; ++i) {
        for (int j = 0; j < nbrColonne; ++j) {

            char caractereLu = labyrintheEnCours[i][j];
            Case& caseActuelle = labyrinthe[i][j];

            if (caractereLu == '#'){
                caseActuelle.estMur = true;
            }
            else if (caractereLu == 'D'){
                caseActuelle.estDepart = true;
                caseDepart_x=i;
                caseDepart_y=j;
            }
            else if (caractereLu == '1') {

                if (numero == 0){
                    caseActuelle.estArrivee = true;
                    caseArrivee_x=i;
                    caseArrivee_y=j;
                }
                if (numero == 1){
                    caseActuelle.estDepart = true;
                    caseDepart_x=i;
                    caseDepart_y=j;
                }
                if (numero == 2){
                    // il ne se passe rien car on part de T
                }

            }
            else if (caractereLu == 'T'){

                if (numero == 1){
                    caseActuelle.estArrivee = true;
                    caseArrivee_x=i;
                    caseArrivee_y=j;
                }
                if (numero == 2){
                    caseActuelle.estDepart = true;
                    caseDepart_x=i;
                    caseDepart_y=j;
                }

            }
            else if (caractereLu == '2'){

                if (numero == 1){
                    // il en se passe rien car on arrive à T
                }
                if (numero == 2){
                    caseActuelle.estArrivee = true;
                    caseArrivee_x=i;
                    caseArrivee_y=j;
                }
                if (numero == 3){
                    caseActuelle.estDepart = true;
                    caseDepart_x=i;
                    caseDepart_y=j;
                }

            }
            else if (caractereLu == 'A'){

                if (numero == 3){
                    caseActuelle.estArrivee = true;
                    caseArrivee_x=i;
                    caseArrivee_y=j;
                }

            }
            else if (caractereLu == 'M'){
                caseActuelle.estMine = true;
            }
            else if (caractereLu == 'E'){
                caseActuelle.estObjetAPrendre = true;
                fautIlRamasserObjet = true;

            }
            else if (caractereLu == 'B'){

                if (numero == 1){
                    caseActuelle.estObjetAPrendre = true;
                    fautIlRamasserObjet = true;
                }
                if (numero == 2){
                    // rien ne se passe car le cheminParcouru est direct sans rammassage
                    fautIlRamasserObjet = false;

                }

            }
            else if (caractereLu == 'C'){
                caseActuelle.estObjetAPrendre    = true;
                fautIlRamasserObjet = true;
            }


        } // fin boucle j
    } // fin boucle i



    // Backtracking
    vector<vector<bool>> isCaseVisitee(nbrLigne, vector<bool>(nbrColonne, false));
    vector<pair<int,int>> cheminParcouru;
    bool trouve = resoudreLabyrinthe(labyrinthe, isCaseVisitee, caseDepart_x, caseDepart_y, caseArrivee_x, caseArrivee_y, cheminParcouru,
            labyrinthe[caseDepart_x][caseDepart_y].estObjetAPrendre, fautIlRamasserObjet);

    // Préparer affichage du labyrinthe avec chemin
    vector<string> affichage = labyrintheEnCours;

    if (trouve) {
        // Parcourir chaque point du chemin
        for (int i = 0; i < cheminParcouru.size(); ++i) {


            pair<int,int> point = cheminParcouru[i];
            int ligne   = point.first;   // coordonnée en ligne
            int colonne = point.second;  // coordonnée en colonne


            char caractere = affichage[ligne][colonne];
            if (caractere == ' ') {

                affichage[ligne][colonne] = '*';
            }
        }
    } // fin trouve




    // Affichage protégé par mutex
    {
        lock_guard<mutex> lock(donnee);
        cout << "\nLabyrinthe #" << (numero + 1)
             << " (" << nbrLigne << "x" << nbrColonne << "):\n";
        if (!trouve) {
            cout << "  Aucun chemin trouvé\n";
        }
        for (auto& ligne : affichage) {
            cout << "  ";
            for (char c : ligne) {
                switch (c) {
                    case '#': cout << BLANC << c << RESET; break;
                    case 'M': cout << ROUGE << c << RESET; break;
                    case 'D':
                    case 'A': cout << BLEU  << c << RESET; break;
                    case 'E':
                    case 'B':
                    case 'C': cout << VERT  << c << RESET; break;
                    case '*': cout << CYAN  << c << RESET; break;
                    default:  cout << c;             break;
                }
            }
            cout << "\n";
        }
    }
}




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
    // démarrage du chrono
    auto chrono_debut = chrono::steady_clock::now();

    vector<thread> threads;

    int nombreLabyrinthes = labyrinthes.size();

    for (int i = 0; i < nombreLabyrinthes; ++i) {

        threads.push_back( thread(traiterLabyrinthe, i, labyrinthes[i]));

    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }


    auto chrono_fin = std::chrono::steady_clock::now();
    auto duree_ms = std::chrono::duration_cast<std::chrono::milliseconds>(chrono_fin - chrono_debut).count();

    cout << "\nTemps total : " << duree_ms << " ms\n";
    return 0;








}
