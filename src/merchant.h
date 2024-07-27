#ifndef MERCHANT_H
#define MERCHANT_H

#include "board.h"
#include "enemy.h"

class Merchant: public Enemy {
    public:
        Merchant(Board&);

        CharacterUpdateAction update(int x, int y) override;
};

#endif
