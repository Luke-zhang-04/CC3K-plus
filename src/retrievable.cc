#include "retrievable.h"

Retrievable::Retrievable(Enemy* e): protectedBy{e} {}
Retrievable::Retrievable(): protectedBy{nullptr} {}

bool Retrievable::onRetrieve(Player* player) {
    // don't do anything if it's protected
    if (protectedBy != nullptr)
        return false;

    // otherwise try to retrieve it
    return retrieved(player);
}

bool Retrievable::isProtected() {
    return protectedBy != nullptr;
}

// can't be re-protected. Could be changed if there's ever an adjacency protection we need, but
// this works for now
void Retrievable::protectorDies() {
    protectedBy = nullptr;
}

void Retrievable::setProtector(Enemy* e) {
    if (protectedBy == nullptr)
        protectedBy = e;
}
