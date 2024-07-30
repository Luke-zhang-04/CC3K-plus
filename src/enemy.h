#ifndef ENEMY_H
#define ENEMY_H
#include "character.h"

#include <cstddef>

using std::size_t;

class Retrievable;
class Board;

enum class EnemyType { Vampire, Werewolf, Troll, Goblin, Merchant, Dragon, Phoenix };

/*
(x, y)

(-1, -1)  (0, -1)  (1, -1)
(-1,  0)  (0,  0)  (1,  0)
(-1,  1)  (0,  1)  (1,  1)

with x+1 + (y+1)*3

perfectly converts to

NW  N  NE
W  atk  E
SW  S  SE
No action
*/

enum class EnemyUpdateAction {
    NorthWest,
    North,
    NorthEast,
    West,
    Attack,
    East,
    SouthWest,
    South,
    SouthEast,
    NoAction
};

class Enemy: public Character {
    private:
        char character;
        Retrievable* drops = nullptr; // item which drops when killed

    protected:
        EnemyUpdateAction walk(size_t x, size_t y);

        // takes in current location, queries whether there is a player near the location
        bool isPlayerNearby(size_t x, size_t y);

        // theoretically could take in the frame if enemies act differently at different times. not
        // currently requires
        virtual EnemyUpdateAction act(size_t x, size_t y);

        Board& board;

        // gold value when killed
        unsigned int gold = 1;

        size_t age = 0;

    public:
        Enemy(EnemyType, Board&);
        ~Enemy();

        char getCharacter() override;

        EnemyUpdateAction update(size_t x, size_t y, size_t frame);

        unsigned int goldValue();

        // give this enemy a retrievable
        void giveTreasure(Retrievable*);

        // take a retrievable from the enemy
        Retrievable* dropTreasure();
};

#endif
