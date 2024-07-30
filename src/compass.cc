#include "compass.h"

#include "board.h"
#include "constants.h"
#include "player.h"

Compass::Compass(Board& board): board{board} {}

char Compass::getCharacter() {
    return Symbol::Compass;
}

bool Compass::retrieved(Player* player) {
    board.showStairs();
    return true;
}
