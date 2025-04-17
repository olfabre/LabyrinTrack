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