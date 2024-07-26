#include "races.h"
#include "player.h"
#include <Algorithm>

Drawf::Dwarf(Board& board): Player(board, 100, 20, 30) {}
Elf::Elf(Board& board):     Player(board, 140, 30, 10) {}
Human::Human(Board& board): Player(board, 140, 20, 20) {}
Orc::Orc(Board& board):     Player(board, 180, 30, 25) {}

void Elf::usePotion(int healthMod, int attackMod, int defenseMod) {
    Player::usePotion(
        std::abs(healthMod),
        std::abs(attackMod),
        std::abs(defenseMod)
    )
}

void Dwarf::pickupGold(int amt) {
    gold += amt * 2;
}

void Orc::pickupGold(int amt) {
    gold += std::floor(amt / 2);
}
