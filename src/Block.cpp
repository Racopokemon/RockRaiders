#include "Block.h"
//Pretty bad style I guess to put this one function into the cpp, for a struct no cpp would be better ... but then I get problems with circular referencing of both imports above 

//!True if this is some kind of ground block that entities can walk on
bool Block::isWalkable() {
    return isBlockWalkable(*this);
}

int Block::getMovementSpeed() {
    return getBlockMovementSpeed(*this);
}