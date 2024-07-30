#ifndef DRAGON_H
#define DRAGON_H
#include "enemy.h"

#include <cstddef>
#include <utility>

class Dragon: public Enemy {
        // dragon is a supernatural entity with elevated privileges
        friend class Game;

    private:
        Retrievable* protects = nullptr;

    protected:
        EnemyUpdateAction act(std::size_t x, std::size_t y) override;

    public:
        Dragon(Board&);
        char getCharacter() override;
        std::pair<int, int> beAttacked(int attackPower) override;
};

#endif // DRAGON_H
