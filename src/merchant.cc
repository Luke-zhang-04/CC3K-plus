#include "merchant.h"

#include "board.h"
#include "enemy.h"

// does not drop gold... why would you kill it!
Merchant::Merchant(Board& board): Enemy{EnemyType::Merchant, board} {
    gold = 0;
}

EnemyUpdateAction Merchant::act(int x, int y) {
    if (board.merchantHostile()) {
        return Enemy::act(x, y); // If merchants are hostile, behave like normal enemy
    }

    return Enemy::walk(x, y);
}
