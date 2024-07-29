#ifndef RETRIEVABLE_H
#define RETRIEVABLE_H

class Player;

class Retrievable {
    public:
        virtual char getCharacter() = 0;
        virtual bool onRetrieve(Player* player) = 0;
        virtual ~Retrievable() {}
};

#endif
