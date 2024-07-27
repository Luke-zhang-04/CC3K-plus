#ifndef DRAGON_H
#define DRAGON_H
#include "dragon_protected.h"
#include "enemy.h"

class Dragon: public Enemy {
    private:
        DragonProtected* protects;

    public:
        char getCharacter();
        CharacterUpdateAction update(int x, int y);
};

#endif // DRAGON_H
