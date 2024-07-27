#ifndef TILE_H
#define TILE_H
#include "enemy.h"
#include "interactible.h"
#include "player.h"
#include "retrievable.h"

#include <ostream>

struct Tile {
    friend std::ostream& operator>>(std::ostream&, Tile&);

    char mapTile;
    Retrievable* treasure;
    Interactible* item;
    Enemy* enemy;
    Player* player;

    // return the character of the highest level item on this tile
    char getCharacter() const;

    // whether or not a character can move into this tile
    bool movable() const;

    // owns treasure, interactible and enemy
    ~Tile();
};

#endif
