#if !defined(TILE_H)
#    define TILE_H
#include "player.h"
#include "interactible.h"

struct Tile {
    char mapTile;
    // Retrievable* treasure;
    Item* item;
    // Enemy* enemy;
    Player* player;

    // return the character of the highest level item on this tile
    char getCharacter();

    // owns treasure, interactible and enemy
    ~Tile();
};

#endif // TILE_H
