#include "dragon.h"

#include "board.h"
#include "constants.h"
#include "retrievable.h"
#include "tile.h"

#include <cstddef>
#include <utility>

// sets the gold value to 0 and tells the enemy to give Dragon stats
Dragon::Dragon(Board& board): Enemy(EnemyType::Dragon, board) {
    gold = 0;
}

std::pair<int, int> Dragon::beAttacked(int attackPower) {
    std::pair<int, int> attkStats = Enemy::beAttacked(attackPower);

    if (attkStats.first <= 0) {
        protects->protectorDies();
    }

    return attkStats;
}

// never moves, only attacks
EnemyUpdateAction Dragon::act(std::size_t x, std::size_t y) {
    auto area = board.getArea(x, y);
    // first, check if the player is nearby (within range)
    for (int ix = 0; ix <= 2; ++ix) {
        for (int iy = 0; iy <= 2; ++iy) {
            if (area[iy][ix]->player) {
                // then, check if the horde is near the player
                auto pArea = board.getArea(x + ix - 1, y + iy - 1);

                for (int jx = 0; jx <= 2; ++jx) {
                    for (int jy = 0; jy <= 2; ++jy) {
                        // if we find our horde, attack the player
                        // protects is a Treasure*, can easily cast it to Retrievable*
                        if (pArea[jy][jx]->treasure == static_cast<Retrievable*>(protects))
                            return EnemyUpdateAction::Attack;
                    }
                }
            }
        }
    }

    // never moves, so if we didn't find the player near our treasure, don't attack them
    return EnemyUpdateAction::NoAction;
}

char Dragon::getCharacter() {
    return Symbol::Dragon;
}
