#ifndef COMPASS_H
#define COMPASS_H

#include "retrievable.h"

class Board;

class Compass: public Retrievable {
        Board& board;

    public:
        Compass(Board& board);
        char getCharacter() override;
        bool onRetrieve(Player* player) override;
};

#endif
