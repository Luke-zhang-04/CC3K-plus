#include "enemy.h"

#include "board.h"
#include "constants.h"
#include "random.h"
#include "retrievable.h"

#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

using std::size_t;

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

Enemy::~Enemy() {
    delete drops;
}

char Enemy::getCharacter() {
    return character;
}

EnemyUpdateAction Enemy::walk(size_t x, size_t y) {
    // collect a list of the locations it can move to
    auto area = board.getArea(x, y);
    std::vector<EnemyUpdateAction> movable;
    movable.reserve(8);
    for (int dx = 0; dx <= 2; ++dx) {
        for (int dy = 0; dy <= 2; ++dy) {
            if (dx == 1 && dy == 1)
                continue; // if both are 0, move on, don't let them not move

            // don't move onto the stairs
            auto stairs = board.getStairLoc();
            if (x + dx - 1 == stairs.first && y + dy - 1 == stairs.second)
                continue;

            if (area[dy][dx] != nullptr && canMove(area[dy][dx])) {
                // std::cout << area[1][1]->getCharacter() << " can move to: " <<
                // area[dy][dx]->getCharacter() << (int) static_cast<EnemyUpdateAction>(dx + dy *
                // 3) << std::endl;
                movable.emplace_back(
                    // Store this location as a movement direction
                    static_cast<EnemyUpdateAction>(dx + dy * 3)
                );
            }
        }
    }

    // if no valid directions were found, don't move
    if (movable.empty())
        return EnemyUpdateAction::NoAction;

    // pick a random location it can move to
    return movable[randInt(0, movable.size() - 1)];
}

// return whether the player is adjacent to the enemy
bool Enemy::isPlayerNearby(size_t x, size_t y) {
    auto player = board.getPlayerLoc();
    return std::abs((long long) player.first - (long long) x) <= 1 &&
           std::abs((long long) player.second - (long long) y) <= 1;
}

EnemyUpdateAction Enemy::act(size_t x, size_t y) {
    if (isPlayerNearby(x, y))
        return EnemyUpdateAction::Attack;
    else
        return walk(x, y);
}

// attack if possible, otherwise move
EnemyUpdateAction Enemy::update(size_t x, size_t y, size_t frame) {
    // if it's at least as old as the frame, don't do anything
    if (age >= frame)
        return EnemyUpdateAction::NoAction;
    age += 1; // gets older. happy birthday!

    return act(x, y);
}

// most enemies are worth 1
unsigned int Enemy::goldValue() {
    return gold;
}

void Enemy::giveTreasure(Retrievable* toDrop) {
    drops = toDrop;
}

Retrievable* Enemy::dropTreasure() {
    Retrievable* treasure = drops;
    drops = nullptr;
    return treasure;
}
