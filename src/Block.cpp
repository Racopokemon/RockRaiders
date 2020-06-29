#include "Block.h"
//Pretty bad style I guess to put this one function into the cpp, for a struct no cpp would be better ... but then I get problems with circular referencing of both imports above 


BuildingType Block::getBuildingType() {
    return BuildingType(metaA);
}

void Block::setBuildingType(BuildingType type) {
    metaA = type;
}

//!True if this is some kind of ground block that entities can walk on
bool Block::isWalkable() {
    return isBlockWalkable(*this);
}

int Block::getMovementSpeed() {
    return getBlockMovementSpeed(*this);
}

float Block::getMovementTimePriority() {
    return getBlockMovementTimePriority(*this);
}

bool Block::isBreakableWall() {
    return isBlockBreakableWall(*this);
}

bool Block::isGeneralWall() {
    return isBlockGeneralWall(*this);
}

int Block::getWallStrength() {
    return getBlockWallStrength(*this);
}

bool Block::isAbsorbingPickups() {
    return getBlockAbsorbingPickups(*this);
}