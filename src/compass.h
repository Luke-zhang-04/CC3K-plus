#ifndef COMPASS_H
#define COMPASS_H

#include "retrievable.h"

class Board;

class Compass: public Retrievable {
    private:
        Board& board;

    protected:
        bool retrieved(Player* player) override;

    public:
        Compass(Board& board);
        char getCharacter() override;
};

#endif
