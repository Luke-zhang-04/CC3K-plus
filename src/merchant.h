#ifndef MERCHANT_H
#define MERCHANT_H

#include "enemy.h"

class Board;

class Merchant: public Enemy {
    protected:
        EnemyUpdateAction act(int x, int y) override;

    public:
        Merchant(Board&);
};

#endif
