#ifndef MERCHANT_H
#define MERCHANT_H

#include "board.h"
#include "enemy.h"

class Merchant: public Enemy {
    public:
        Merchant(Board&);

        EnemyUpdateAction update(int x, int y) override;
};

#endif
