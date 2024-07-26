#ifndef COMPASS_H
#define COMPASS_H

#include "board.h"
#include "retrievable.h"

class Compass: public Retrievable {
    Board& board;

    public:
        Compass(Board& board);
        char getCharacter() override;
        bool onRetrieve(Player* player) override;
};

#endif
