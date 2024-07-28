#include "player.h"

#include "constants.h"

#include <algorithm>

Player::Player(Board& board, int maxHealth, int attack, int defence):
    Character{board, maxHealth, attack, defense} {}

char Player::getCharacter() {
    return Symbols::Player;
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
