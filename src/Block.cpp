#include "Block.h"
//Pretty bad style I guess to put this one function into the cpp, for a struct no cpp would be better ... but then I get problems with circular referencing of both imports above 


BuildingType Block::getBuildingType() {
    return BuildingType(metaA);
}

void Block::setBuildingType(BuildingType type) {
    metaA = type;
}

bool Block::getVisibleAtStart() {
    return metaB != 0;
}

int Block::getWorkersAtStart() {
    return metaA;
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

bool Block::isHoldingPickups() {
    return getBlockHoldingPickups(*this);
}

bool Block::isStorageBuilding() {
    return getBlockStorageBuilding(*this);
}