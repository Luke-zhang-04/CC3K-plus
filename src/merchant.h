#ifndef MERCHANT_H
#define MERCHANT_H

#include "enemy.h"

#include <cstddef>

class Board;

class Merchant: public Enemy {
    protected:
        EnemyUpdateAction act(std::size_t x, std::size_t y) override;

    public:
        Merchant(Board&);
};

#endif
