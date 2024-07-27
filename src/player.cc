#include "player.h"

#include <algorithm>

Player::Player(Board& board, int maxHealth, int attack, int defence):
    attackMod{0}, defenseMod{0}, suited{false}, gold{0},
    Character{board, maxHealth, attack, defense} {}

char Player::getCharacter() {
    return '@';
}

void Player::usePotion(int healthBuff, int attackBuff, int defenseBuff) {
    attackMod += attackBuff;
    defenseMod += defenseBuff;
    health = std::max(health + healthBuff, maxHealth);
}

void Player::pickupSuit() {
    suited = true;
}

void Player::pickupGold(int amt) {
    gold += amt;
}
