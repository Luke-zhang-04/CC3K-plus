#include "merchant.h"

#include "board.h"
#include "enemy.h"

// does not drop gold... why would you kill it!
Merchant::Merchant(Board& board): Enemy{EnemyType::Merchant, board} {
    gold = 0;
}

EnemyUpdateAction Merchant::update(int x, int y) {
    if (board.merchantHostile()) {
        return Enemy::update(x, y); // If merchants are hostile, behave like normal enemy
    }

    return Enemy::walk(x, y);
}
