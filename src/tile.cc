#include "tile.h"

char Tile::getCharacter() {
    if (player != nullptr) return player->getCharacter();
    // if (enemy != nullptr) return enemy->getCharacter();
    if (item != nullptr) return item->getCharacter();
    // if (treasure != nullptr) return treasure->getCharacter();
    return mapTile;
}

Tile::~Tile() {
    // delete enemy;
    delete item;
    // delete treasure;
}
