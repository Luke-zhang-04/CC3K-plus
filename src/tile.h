#ifndef TILE_H
#define TILE_H

#include "constants.h"

#include <cstddef>
#include <ostream>

using std::size_t;

class Retrievable;
class Interactible;
class Enemy;
class Player;

/* clang-format off */
struct Tile {
    friend std::ostream& operator<<(std::ostream&, Tile&);

    char mapTile = Symbol::Blank;
    Retrievable* treasure = nullptr;
    Interactible* item = nullptr;
    Enemy* enemy = nullptr;
    Player* player = nullptr;

    /** Chamber ID, should only be non-zero if `mapTile` is a floor tile or stair */
    size_t chamberId = 0;

    // return the character of the highest level item on this tile
    char getCharacter() const;

    bool empty() const;

    // owns treasure, interactible and enemy
    ~Tile();
};
/* clang-format on */

std::ostream& operator<<(std::ostream&, const Tile&);

#endif
