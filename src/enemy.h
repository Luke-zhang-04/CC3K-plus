#ifndef ENEMY_H
#define ENEMY_H
#include "character.h"

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
        EnemyUpdateAction walk(int x, int y);

        // takes in current location, queries whether there is a player near the location
        bool isPlayerNearby(int x, int y);

        Board& board;

        // gold value when killed
        int gold = 1;

    public:
        Enemy(EnemyType, Board&);

        char getCharacter() override;

        virtual EnemyUpdateAction update(int x, int y);

        int goldValue();

        // override Character's beAttacked so it can drop when killed
        int beAttacked(int) override;

        // give this enemy a retrievable
        void giveTreasure(Retrievable*);

        // take a retrievable from the enemy
        Retrievable* dropTreasure();
};

#endif
