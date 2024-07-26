#include "player.h"
#include <algorithm>

Player::Player(Board& board, int maxHealth, int defence, int attack) :
     attackMod{0}, defenseMod{0}, suited{false}, gold{0}, Character{board, maxHealth, defence, attack} {}

char Player::getCharacter() {
    return '@';
}

void Player::usePotion(int healthBuff, int attackBuff, int defenseBuff) {
    attackMod += attackBuff;
    defenseMod += defenseBuff;
    health = std::max(health + healthBuff, maxHealth);
}
