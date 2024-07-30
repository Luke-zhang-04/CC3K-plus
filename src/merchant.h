#ifndef MERCHANT_H
#define MERCHANT_H

#include "enemy.h"

#include <cstddef>

using std::size_t;

class Board;

class Merchant: public Enemy {
    protected:
        EnemyUpdateAction act(size_t x, size_t y) override;

    public:
        Merchant(Board&);
};

#endif
