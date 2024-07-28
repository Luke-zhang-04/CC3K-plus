#ifndef TILE_H
#define TILE_H

#include "constants.h"

#include <ostream>

class Retrievable;
class Interactible;
class Enemy;
class Player;

/* clang-format off */
struct Tile {
    friend std::ostream& operator>>(std::ostream&, Tile&);

    char mapTile;
    Retrievable* treasure = nullptr;
    Interactible* item = nullptr;
    Enemy* enemy = nullptr;
    Player* player = nullptr;

    // return the character of the highest level item on this tile
    char getCharacter() const;

    // whether or not a character can move into this tile
    bool movable() const;

    // owns treasure, interactible and enemy
    Tile(
        char mapTile,
        Retrievable* treasure,
        Interactible* item,
        Enemy* enemy,
        Player* player
    );
    Tile(char mapTile = Symbols::Blank);
    ~Tile();
};
/* clang-format on */

#endif
