#include "compass.h"
#include "board.h"
#include "player.h"

Compass::Compass(Board& board): board(board) {}

char Compass::getCharacter() {
    return 'C';
}

bool Compass::onRetrieve(Player* player) {
    board.showStairs();
    return true;
}
