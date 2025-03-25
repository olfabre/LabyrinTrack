#ifndef POSITION_HPP
#define POSITION_HPP

struct Position {
    int x;
    int y;


    // Objets à collecter (états)
    bool couronne;
    bool bouclier;
    bool epee;

    Position(int _x, int _y, bool c = false, bool b = false, bool e = false)
            : x(_x), y(_y), couronne(c), bouclier(b), epee(e) {}

    // Vérifie si tous les objets sont collectés
    bool tousObjetsCollectes() const {
        return couronne && bouclier && epee;
    }
};

#endif
