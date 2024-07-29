#ifndef DRAGON_H
#define DRAGON_H
#include "enemy.h"

class DragonProtected;

class Dragon: public Enemy {
        friend class Game;

    private:
        DragonProtected* protects = nullptr;

    public:
        Dragon(Board&);
        char getCharacter() override;
        int beAttacked(int attackPower) override;
        EnemyUpdateAction update(int x, int y) override;
};

#endif // DRAGON_H
