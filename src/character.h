#ifndef CHARACTER_H
#define CHARACTER_H

class Board;

class Character {
    protected:
        int maxHealth, health, defense, attack;

    public:
        Character(int maxHealth, int defense, int attack);

        /** Character to be displayed on board */
        virtual char getCharacter() = 0;

        /** Attacking power */
        int getPower();

        /** Update its own health and returns it, could be negative if it dies */
        virtual int beAttacked(int attackPower);
};

#endif
