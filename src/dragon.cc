#include "dragon.h"

#include "constants.h"
#include "dragon_protected.h"

// sets the gold value to 0 and tells the enemy to give Dragon stats
Dragon::Dragon(Board& board): Enemy(EnemyType::Dragon, board) {
    gold = 0;
}

int Dragon::beAttacked(int attackPower) {
    int newHealth = Enemy::beAttacked(attackPower);

    if (newHealth <= 0) {
        protects->dragonDead();
    }

    return newHealth;
}

// never moves, only attacks
EnemyUpdateAction Dragon::update(int x, int y) {
    // TODO: Check adjacent to dragon hoard
    if (false)
        return EnemyUpdateAction::Attack;

    return EnemyUpdateAction::NoAction;
}

char Dragon::getCharacter() {
    return Symbol::Dragon;
}
