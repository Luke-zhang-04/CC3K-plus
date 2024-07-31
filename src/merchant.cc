#include "merchant.h"

#include "board.h"
#include "constants.h"
#include "enemy.h"
#include "treasure.h"

#include <cstddef>
#include <iostream>

// does not drop gold... why would you kill it! -- for the merchant's hoard... I guess
Merchant::Merchant(Board& board): Enemy{EnemyType::Merchant, board} {
    gold = 0;
    giveTreasure(new Treasure{TreasureType::MerchantHoard});
}

EnemyUpdateAction Merchant::act(std::size_t x, std::size_t y) {
    if (board.merchantHostile()) {
        return Enemy::act(x, y); // If merchants are hostile, behave like normal enemy
    }

    return Enemy::walk(x, y);
}

std::pair<int, int> Merchant::beAttacked(int attackPower) {
    // first make merchants hostile, then be attacked as normal
    board.aggravateMerchants();
    return Enemy::beAttacked(attackPower);
}
