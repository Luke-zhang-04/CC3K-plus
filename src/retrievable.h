#if !defined(RETRIEVABLE_H)
#define RETRIEVABLE_H

class Retrievable {
    public:
        char getCharacter() = 0;
        void onRetrieve(Player* player) = 0;
}

#endif // RETRIEVABLE_H
