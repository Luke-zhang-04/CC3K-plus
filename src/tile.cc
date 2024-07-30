#include "tile.h"

#include "constants.h"
#include "enemy.h"
#include "interactible.h"
#include "player.h"
#include "treasure.h"

#include <ostream>
#include <utility>

Tile::~Tile() {
    delete enemy;
    delete item;
    delete treasure;
}

char Tile::getCharacter() const {
    if (player != nullptr)
        return player->getCharacter();
    if (enemy != nullptr)
        return enemy->getCharacter();
    if (item != nullptr)
        return item->getCharacter();
    if (treasure != nullptr)
        return treasure->getCharacter();
    return mapTile;
}

bool Tile::empty() const {
    return player == nullptr && enemy == nullptr && item == nullptr && treasure == nullptr;
}
