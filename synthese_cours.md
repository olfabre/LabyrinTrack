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



######  🎓 Pourquoi utiliser ça ?

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



📌 Attention : ici les threads partagent `c`, donc si plusieurs threads écrivent au **même endroit**, il faut **protéger** la mémoire (mutex, atomic… qu’on verra plus tard).



#### Les fonctions **lambda** avec threads

Une **lambda** c’est une **fonction anonyme** (sans nom) que tu définis **à la volée**.



exemple simple

```cpp
auto action = []() {
    cout << "Thread avec lambda" << endl;
};

thread t(action);
t.join();
```



exemple fonction lambda avec paramètres

```cpp
auto action = [](int x, int y) {
    cout << "Somme = " << x + y << endl;
};

thread t(action, 3, 4); // Affiche Somme = 7
t.join();
```



exemple avec capture de vairiables

```cpp
int x = 2, y = 3;
auto action = [x, y]() {
    cout << "Produit = " << x * y << endl;
};
thread t(action);
t.join();
```



exemple avec capture par référence (modifie la vraie variable)

```cpp
int somme = 0;
auto action = [&somme]() {
    for (int i = 0; i < 5; i++) somme += i;
};
thread t(action);
t.join();
cout << "Somme = " << somme << endl; // Somme = 10
```



exemple lambda dans une classe

```cpp
class Toto {
private:
    int somme = 0;

public:
    void fonction() {
        auto action = [this]() {
            for (int i = 0; i < 10; i++) somme += i;
        };
        thread t(action);
        t.join();
    }
};
```





#### detach()

💥 `detach` : le thread continue **en arrière-plan** tout seul.



```cpp
void f() {
    cout << "Je suis détaché..." << endl;
}

int main() {
    thread t(f);
    t.detach(); // Le thread devient indépendant
}
```

🧠 Mais attention ! Si ton programme principal **finit trop vite**, le thread détaché **n’aura pas le temps de s’exécuter** !



#### sleep_for

 Faire une pause

```cpp
#include <chrono>
#include <thread>

this_thread::sleep_for(chrono::seconds(3)); // Attend 3 secondes
```



Exemple avec lambda et pause

```cpp
for (int i = 0; i < 5; i++) {
    thread t([i] {
        if (i == 0)
            this_thread::sleep_for(chrono::seconds(5));
        cout << "Thread #" << i << endl;
    });
    t.join(); // Important sinon le thread sera tué tout de suite
}
```




#### Synchronisation



### 💣 Pourquoi ?

Tous les threads partagent la même mémoire → ils peuvent **écraser** les données.



🧤 `mutex` (verrou) 

exemple 

```cpp
#include <mutex>

mutex m;
int compteur = 0;

void increment() {
    for (int i = 0; i < 1000; i++) {
        m.lock();
        compteur++;
        m.unlock();
    }
}
```

⚠️ Si tu oublies le `unlock()`, ça peut **bloquer à jamais** d'autres threads.





✅ `lock_guard` (verrou automatique)

exemple 

```cpp
void increment() {
    for (int i = 0; i < 1000; i++) {
        lock_guard<mutex> verrou(m);
        compteur++;
    }
}
```

➡️ **Plus sûr**, car le mutex est libéré **automatiquement** à la fin du bloc `{}`.



💎 `atomic`

exemple

```cpp
#include <atomic>

atomic<int> compteur(0);

void increment() {
    for (int i = 0; i < 1000; i++) {
        compteur++;
    }
}
```

➡️ **Pas besoin de mutex !** Mais ça ne marche que pour des types simples (`int`, `bool`, etc.).





#### Problèmes classiques rencontrés



🌀 **Race Condition**

Deux threads écrivent en **même temps** → résultat **imprévisible**.

```cpp
int total = 500;

void add(int x) {
    total += x;
}

int main() {
    thread t1(add, 50);
    thread t2(add, 50);
    t1.join();
    t2.join();
    cout << "Total = " << total << endl;
}
```

💥 Résultat : parfois 600, parfois 550, etc. => il faut un `mutex` pour protéger.



🧨 **Deadlock**

Deux threads attendent **mutuellement** une ressource => **blocage total**.

```cpp
mutex m1, m2;

void t1() {
    m1.lock();
    m2.lock();
    // travail
    m2.unlock();
    m1.unlock();
}

void t2() {
    m2.lock();
    m1.lock();
    // travail
    m1.unlock();
    m2.unlock();
}
```

➡️ Solution : toujours **verrouiller dans le même ordre**.

## 🧨 Deadlock : c’est quoi exactement ?

Imagine deux **personnes** qui veulent chacune utiliser **deux fourchettes** pour manger 🍴🍴.

- **Personne A** prend la **fourchette 1** et attend la **fourchette 2**.
- **Personne B** prend la **fourchette 2** et attend la **fourchette 1**.

💥 Résultat : **elles se bloquent à jamais**. Personne ne lâche sa fourchette. Personne ne peut manger. => **Deadlock** (blocage total).

## 🧠 En programmation, c’est pareil :

Tu as deux **threads** (tâches en parallèle), et deux **ressources** protégées par des **verrous** (`mutex`).

### 🔐 Les `mutex`

- Ce sont des **verrous**, pour éviter que deux threads écrivent en même temps dans une même mémoire.
- `lock()` => je verrouille
- `unlock()` => je déverrouille

exemple de blockage avec deadlock

```cpp
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m1, m2;

void t1() {
    m1.lock();          // 🔒 thread t1 verrouille m1
    this_thread::sleep_for(chrono::milliseconds(100));
    m2.lock();          // 🧨 attend m2 qui est peut-être déjà pris !
    cout << "t1 travaille" << endl;
    m2.unlock();
    m1.unlock();
}

void t2() {
    m2.lock();          // 🔒 thread t2 verrouille m2
    this_thread::sleep_for(chrono::milliseconds(100));
    m1.lock();          // 🧨 attend m1 qui est peut-être déjà pris !
    cout << "t2 travaille" << endl;
    m1.unlock();
    m2.unlock();
}

int main() {
    thread a(t1);
    thread b(t2);
    a.join();
    b.join();
    return 0;
}
```

🧨 Ce qu’il se passe ici :

| Temps | Thread t1            | Thread t2            |
| ----- | -------------------- | -------------------- |
| 0 ms  | `m1.lock()`          | `m2.lock()`          |
| 100ms | essaie `m2.lock()` ❌ | essaie `m1.lock()` ❌ |

🔁 Les deux attendent indéfiniment = **DEADLOCK**.



✅ Solution : verrouiller dans **le même ordre**

Au lieu de :

```cpp
t1 → m1 puis m2  
t2 → m2 puis m1 ❌ (ordre différent !)
```

On **force** les deux threads à toujours faire dans le même ordre, par exemple :

```cpp
void t1() {
    m1.lock();
    m2.lock();
    // ...
    m2.unlock();
    m1.unlock();
}

void t2() {
    m1.lock();  // 👈 même ordre que t1
    m2.lock();
    // ...
    m2.unlock();
    m1.unlock();
}
```

💡 Résultat : pas de deadlock car personne ne reste bloqué en attendant quelque chose que l'autre possède **dans un ordre inverse**.



🧪 Encore mieux : utiliser `std::lock`

En C++11, tu peux faire :

```cpp
std::lock(m1, m2);
```

Cela verrouille **plusieurs mutex en toute sécurité**, sans deadlock possible.

🔒 Exemple sûr avec `std::lock`

```cpp
void t1() {
    std::lock(m1, m2);
    lock_guard<mutex> lg1(m1, std::adopt_lock);
    lock_guard<mutex> lg2(m2, std::adopt_lock);
    // travail
}
```

✅ `std::adopt_lock` dit : “les mutex sont déjà verrouillés, pas besoin de les verrouiller encore.”





#### 🧍‍♂️ **Starvation**

Un thread **attend trop longtemps** une ressource que les autres accaparent.

Pas d’exemple direct ici, mais ça peut arriver si on priorise toujours les mêmes threads.





### Patrons de conception



#### 🪓 1. **Fork-Join**

- **Fork** : le programme se divise en plusieurs threads.
- **Join** : on attend que les threads aient fini avant de continuer.

exemple

```cpp
void travail(int i) {
    cout << "Thread " << i << " travaille" << endl;
}

int main() {
    thread t1(travail, 1);
    thread t2(travail, 2);
    t1.join();
    t2.join();
    cout << "Tous les threads ont terminé" << endl;
}
```



#### 👑 2. **Maître-Esclave**

- Un thread **maître** donne du travail aux **esclaves** (workers).
- Il gère la coordination.



#### ☎️ 3. **Client-Serveur**

- Le **serveur** répond à des requêtes venant de plusieurs **clients**.



#### 🚰 4. **Pipeline**

- Une tâche est découpée en **étapes successives**.
- Chaque thread fait **une étape**, puis passe à la suivante.

Exemple imagé :

> Thread A fait "lire", Thread B fait "traiter", Thread C fait "enregistrer".



#### 🧺 5. **Bassin de tâches / Producteur-Consommateur**

- **Thread Pool** : plusieurs threads prêts à exécuter des tâches dès qu’elles sont disponibles.
- **Producteur-Consommateur** :
  - Le producteur met des données dans un **tampon**.
  - Le consommateur lit les données du tampon.



### Performance & Speedup



## 🏎️ 1. **Pourquoi mesurer les performances ?**

Quand tu écris un programme **parallèle** (avec plusieurs threads, plusieurs cœurs...), l’idée, c’est de le faire **plus rapide** que le même programme en version **séquentielle** (ligne par ligne, un seul cœur CPU).

Mais... **est-ce que ça vaut le coup ?** 🤔
 On a besoin d’un moyen pour **mesurer** le gain de vitesse. C’est là qu’arrive la notion de **speedup** !



📊 2. **Qu’est-ce que le Speedup ?**



🔢 Formule :



```cpp
Sp = Tseq / Tp
```

- `Tseq` = Temps d’exécution en **séquentiel** (sur 1 cœur)
- `Tp` = Temps d’exécution en **parallèle**, avec `p` processeurs
- `Sp` = le **gain de vitesse**

### 

un exemple 

| Version             | Temps       |
| ------------------- | ----------- |
| Séquentiel          | 10 secondes |
| Parallèle (4 cœurs) | 3 secondes  |

Alors :

```cpp
Sp = 10 / 3 ≈ 3.33
```

➡️ Tu vas 3,33 fois plus vite avec 4 cœurs ! 💪



🧠 3. L’idéal théorique

> Si ton code est **parfaitement parallélisable**, alors :

```
Sp ≈ p
```

📌 Exemple : 4 cœurs ⇒ on pourrait espérer **4 fois plus rapide**.

Mais... 😢 dans la vraie vie, **tout ne peut pas être parallélisé**. Et c’est là qu’intervient…



📐 4. La **loi d’Amdahl** (⚠️ super importante)



Elle dit :

```cpp
Speedup ≤ 1 / ((1 - S) + S / P)
```

`S` : pourcentage de ton code **parallélisable** (entre 0 et 1)

`P` : nombre de processeurs (ou threads/cœurs)

`1 - S` : partie du code qui **reste séquentielle**



📌 Exemple : 90 % parallélisable (`S = 0.9`)



| Nombre de cœurs `P` | Calcul du speedup                 | Résultat   |
| ------------------- | --------------------------------- | ---------- |
| 1                   | `1 / ((1 - 0.9) + 0.9/1) = 1`     | 1 (normal) |
| 2                   | `1 / (0.1 + 0.9/2) = 1 / 0.55`    | ≈ 1.82     |
| 4                   | `1 / (0.1 + 0.9/4) = 1 / 0.325`   | ≈ 3.08     |
| 100                 | `1 / (0.1 + 0.9/100) = 1 / 0.109` | ≈ 9.17     |

➡️ Même avec **100 processeurs**, tu ne dépasses pas **9,17x plus vite** ❗

Pourquoi ? Parce que les **10 % restants** te **freinent** toujours !



### 💡 Exemple

Si 90 % de ton code est parallélisable (`S = 0.9`) :

- avec 4 processeurs → speedup max ≈ 3.08
- avec 100 processeurs → speedup max ≈ 9.17

On voit que **ajouter plus de processeurs** n’aide plus passé un certain point ⚠️

