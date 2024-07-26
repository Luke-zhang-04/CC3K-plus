#if !defined(TILE_H)
#    define TILE_H
#include <ostream>

#include "retrievable.h"
#include "player.h"
#include "enemy.h"
#include "interactible.h"

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

#endif // TILE_H
