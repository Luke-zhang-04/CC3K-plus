#include "merchant.h"

#include "board.h"
#include "enemy.h"

Merchant::Merchant(Board& board): Enemy{EnemyType::Merchant, board} {}

CharacterUpdateAction Merchant::update(int x, int y) {
    if (board.merchantHostile()) {
        return Enemy::update(x, y); // If merchants are hostile, behave like normal enemy
    }

    return Enemy::walk(x, y);
}
