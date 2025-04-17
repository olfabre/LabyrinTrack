# Cours sur le parallèlisme (Master ILSEN)

## Aide mémoire pour mon évaluation



Imagine un **restaurant** :

- Le **processus**, c’est **tout le restaurant** : la cuisine, les serveurs, les plats, les clients…
- Les **threads**, ce sont les **serveurs** : ils font tous partie du même resto, partagent les mêmes plats, mais chacun peut aller **servir une table différente en même temps**





#### Notion de processus

#### 🌱 Définition

Un **processus**, c’est **un programme en cours d’exécution**.

Par exemple, quand tu lances Firefox ou ton terminal, chacun devient un processus.

#### 📦 Caractéristiques d’un processus

- **Isolé des autres** :
  - En **temps** → comme s’il avait son propre CPU.
  - En **espace mémoire** → il ne voit pas les données des autres processus.

#### 📑 Il possède :

- Un **PID** (Process ID).
- Un **contexte mémoire** :
  - `.text` = instructions du programme.
  - `.data` = variables globales.
  - `.heap` = mémoire allouée dynamiquement (via `new`).
  - `.stack` = pile d’exécution (appels de fonctions, variables locales, etc.).



#### Notion de thread

#### 🌱 Définition

Un **thread**, c’est une tâche **légère** à l’intérieur d’un processus. C’est comme un **sous-programme** qui peut s’exécuter en **parallèle** avec les autres. Un **thread** est un **petit programme** à l’intérieur d’un **gros programme (le processus)**. C’est comme une **fonction** qui tourne **en parallèle** avec d’autres.

#### 🎯 Pourquoi utiliser des threads ?

- Pour **utiliser plusieurs cœurs** du CPU en même temps.
- Pour écrire du code **plus rapide et réactif** (ex. : interface graphique + calculs en fond).

#### 🧠 Caractéristiques

- **Partage la mémoire** avec les autres threads du même processus.
- Chaque thread a sa **propre pile (stack)** pour ses variables locales.
- Ils peuvent s’exécuter indépendamment.



exemple de base

```cpp
#include <iostream>
#include <thread>

void direBonjour() {
    std::cout << "Bonjour depuis un thread !" << std::endl;
}

int main() {
    std::thread t1(direBonjour); // Création du thread
    t1.join(); // On attend que le thread se termine
    return 0;
}

```

🧾 Ce que fait ce code :

- Il crée un thread qui exécute la fonction `direBonjour()`.
- `join()` permet de dire : "attend que le thread ait fini avant de quitter le programme."



##### 🧠 La **pile**, le **tas**, les **variables globales**

###### 📦 1. La **pile (stack)**

C’est là où sont stockées :

- Les **variables locales** à une fonction
- Les **arguments** de fonction
- Le **chemin d’exécution** (pour savoir où on est)

📌 Chaque **thread a sa propre pile**. Donc les variables locales ne se mélangent pas !



###### 🗃️ 2. Le **tas (heap)**

C’est la **mémoire dynamique**. Par exemple quand tu fais :

```
int* p = new int(5);
```

⛲ Là, tu demandes un **int** sur le tas (heap), et tous les threads peuvent y accéder s’ils partagent le pointeur.

📌 Le **tas est partagé** entre les threads : donc attention aux conflits !



###### 🌐 3. Les **variables globales**

Elles sont partagées **par tous les threads**.

```
int compteur = 0;

void incrementer() {
    compteur++;
}
```

📌 Si plusieurs threads modifient `compteur` **en même temps**, il y a des **problèmes** → c’est ce qu’on appelle une **condition de course (race condition)** 😱



##### ⚙️ Utiliser plusieurs **cœurs CPU** grâce aux threads

Quand tu crées plusieurs threads, le système d’exploitation peut les faire tourner :

- Soit sur le **même cœur**, un par un (tour par tour)
- Soit sur **des cœurs différents en parallèle**, si ton CPU en a plusieurs

### ➤ Exemple :

Si tu as un **CPU à 4 cœurs**, tu peux lancer 4 threads qui s’exécutent **vraiment en même temps** 🧠🧠.

🧪 Exemple C++11 avec plusieurs threads :



```cpp
#include <iostream>
#include <thread>

void travail(int id) {
    std::cout << "Thread " << id << " travaille...\n";
}

int main() {
    std::thread t1(travail, 1);
    std::thread t2(travail, 2);

    t1.join();
    t2.join();

    return 0;
}

```

##### 🎓 Bonnes pratiques pour débuter

 ✅ Utilise des **fonctions simples** dans chaque thread
 ✅ Fais attention au **partage de mémoire** (tas et variables globales)
 ✅ Utilise des **verrous (mutex)** quand plusieurs threads écrivent dans la même donnée
 ✅ **Toujours appeler `.join()`** pour attendre la fin du thread (sinon bug !)

#### Processus lourd VS léger

|                            | **Processus (lourd)** | **Thread (léger)**      |
| -------------------------- | --------------------- | ----------------------- |
| **Mémoire**                | Séparée               | Partagée                |
| **Pile**                   | Séparée               | Séparée                 |
| **Registres CPU**          | Séparés               | Séparés                 |
| **Fichiers ouverts**       | Propres               | Partagés                |
| **Changement de contexte** | Lent                  | Rapide                  |
| **Communication**          | Difficile             | Facile (par la mémoire) |

Processus : chacun avec sa mémoire
Threads : partagent le tas et les variables globales



##### **Partage de mémoire entre threads**

###### 📦 Les threads partagent :

- Les **variables globales** (`int compteur`).
- Le **tas** (`new`, `malloc`).

###### 🧑‍🤝‍🧑 Chaque thread a :

- Sa **propre pile (stack)** avec ses variables locales et registres.



```mathematica
Mémoire Partagée (tas, globales)
├── Thread A : sa propre pile
├── Thread B : sa propre pile
├── Thread C : sa propre pile
```



exemple

```cpp
#include <iostream>
#include <thread>
using namespace std;

int compteur = 0;

void f() {
    compteur += 1;
}

int main() {
    thread t1(f);
    thread t2(f);
    t1.join();
    t2.join();
    cout << "Compteur = " << compteur << endl;
    return 0;
}
```

➡️ Ici, `compteur` est **partagé** par les deux threads. Ils peuvent l’**écraser** si on ne protège pas son accès (avec un mutex par exemple, mais ça vient après dans le cours).



Tu peux aussi créer un thread **sans tâche**, et lui en attribuer une plus tard :

```cpp
thread u;
u = thread(g, 20);
u.join();
```



#### Operator()

✅ À quoi ça sert ?

En C++11, quand tu veux créer un `std::thread`, tu peux lui donner :

- une **fonction**
- un **lambda**
- ou… un **objet qui se comporte comme une fonction** → **grâce à `operator()`**

Exemple

```cpp
#include <iostream>
#include <thread>
using namespace std;

class W {
public:
    void operator()() {
        cout << "Opérateur () appelé" << endl;
    }
};

int main() {
    W zz;             // zz est un objet de type W
    thread t(zz);     // on passe zz au thread -> comme une fonction !
    t.join();         // on attend que le thread se termine
    return 0;
}
```

### Ce qu’il se passe :

1. Tu crées un objet `zz` de type `W`.
2. Tu dis : `thread t(zz);`
   - Le thread regarde : "Est-ce que `zz` est **appelable** comme une fonction ?"
   - Oui ! Parce qu’il a `operator()`.
3. Le thread **exécute le contenu** de `operator()()` dans un thread séparé.

💡 C’est comme si tu faisais :

```cpp
void f() {
    cout << "Fonction appelée" << endl;
}

thread t(f); // même idée
```

Mais là, tu utilises une **classe personnalisée qui se comporte comme une fonction**.



## 🎓 Pourquoi utiliser ça ?

- Tu peux **garder des variables dans la classe** si tu veux qu’un thread ait un **état**.
- Très pratique si tu veux créer des **tâches personnalisées et réutilisables**.

exemple



```cpp
class Compteur {
    int debut, fin;
public:
    Compteur(int d, int f) : debut(d), fin(f) {}

    void operator()() {
        for (int i = debut; i <= fin; ++i)
            cout << i << " ";
        cout << endl;
    }
};

int main() {
    Compteur c(1, 5); // Compteur de 1 à 5
    thread t(c);      // Lancement dans un thread
    t.join();
    return 0;
}

🔍 Résultat :
1 2 3 4 5
```

💡 Ici, tu vois qu’on peut **passer des données** à notre objet (`debut`, `fin`) et les utiliser dans `operator()`.

| Élément         | Signification                                              |
| --------------- | ---------------------------------------------------------- |
| `operator()()`  | Permet à une classe de se **comporter comme une fonction** |
| `thread t(zz);` | Crée un thread en lançant `zz.operator()()`                |
| Avantage        | Tu peux garder des **données internes** dans la classe     |



#### Parallèlisme avec 2 threads



exemple



```cpp
vector<int> a = {1, 3, 5, 8};
vector<int> b = {7, 3, 3, -4};
vector<int> c(4);

void add(int start, int end) {
    for (int i = start; i < end; i++) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    thread t1(add, 0, 2); // traite indices 0 et 1
    thread t2(add, 2, 4); // traite indices 2 et 3
    t1.join();
    t2.join();
    return 0;
}

```



