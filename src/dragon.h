#ifndef DRAGON_H
#define DRAGON_H
#include "enemy.h"

#include <cstddef>
#include <utility>

using std::size_t;

class DragonProtected;

class Dragon: public Enemy {
        friend class Game;

    private:
        DragonProtected* protects = nullptr;

    protected:
        EnemyUpdateAction act(size_t x, size_t y) override;

    public:
        Dragon(Board&);
        char getCharacter() override;
        std::pair<int, int> beAttacked(int attackPower) override;
};

#endif // DRAGON_H
