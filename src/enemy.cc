#include "enemy.h"

#include "board.h"
#include "constants.h"

#include <vector>

Enemy::Enemy(EnemyType enemyType, Board& board): Character{0, 0, 0}, character{0}, board{board} {
    switch (enemyType) {
        case EnemyType::Vampire:
            maxHealth = 50;
            attack = 25;
            defense = 25;
            character = Symbol::Vampire;
            break;
        case EnemyType::Werewolf:
            maxHealth = 120;
            attack = 30;
            defense = 5;
            character = Symbol::Werewolf;
            break;
        case EnemyType::Troll:
            maxHealth = 120;
            attack = 25;
            defense = 15;
            character = Symbol::Troll;
            break;
        case EnemyType::Goblin:
            maxHealth = 70;
            attack = 5;
            defense = 10;
            character = Symbol::Goblin;
            break;
        case EnemyType::Merchant:
            maxHealth = 30;
            attack = 70;
            defense = 5;
            character = Symbol::Merchant;
            break;
        case EnemyType::Dragon:
            maxHealth = 150;
            attack = 20;
            defense = 20;
            character = Symbol::Dragon;
            break;
        case EnemyType::Phoenix:
            maxHealth = 50;
            attack = 35;
            defense = 20;
            character = Symbol::Phoenix;
            break;
    }

    health = maxHealth;
}

char Enemy::getCharacter() {
    return character;
}

EnemyUpdateAction Enemy::walk(int x, int y) {
    // collect a list of the locations it can move to
    auto area = board.getArea(x, y);
    std::vector<EnemyUpdateAction> movable;
    for (int ix = x - 1; ix <= x + 1; ix++) {
        for (int iy = y - 1; iy <= y + 1; iy++) {
            // don't check the current tile
            if (ix == x && iy == y)
                continue;

            // if it's movable, add it to the array
            if (area[iy][ix] && area[iy][ix]->movable()) {
                movable.emplace_back(
                    // Store this location as a movement direction
                    static_cast<EnemyUpdateAction>((ix + 1 - x) + (iy + 1 - y) * 3)
                );
            }
        }
    }

    // pick a random location it can move to
    return movable[rand() % movable.size()];
}

// return whether the player is adjacent to the enemy
bool Enemy::isPlayerNearby(int x, int y) {
    auto player = board.getPlayerLoc();
    return std::abs(player.first - x) <= 1 && std::abs(player.second - y) <= 1;
}

// attack if possible, otherwise move
EnemyUpdateAction Enemy::update(int x, int y) {
    if (isPlayerNearby(x, y))
        return EnemyUpdateAction::Attack;
    else
        return walk(x, y);
}

// most enemies are worth 1
int Enemy::goldValue() {
    return gold;
}

void Enemy::giveTreasure(Retrievable* toDrop) {
    drops = toDrop;
}

int Enemy::beAttacked(int attackPower) {
    // TODO: stuff
    return Character::beAttacked(attackPower);
}

Retrievable* Enemy::dropTreasure() {
    Retrievable* treasure = drops;
    drops = nullptr;
    return treasure;
}
