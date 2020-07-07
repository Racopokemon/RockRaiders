#include "BlockPropertiesLookup.h"
#include "Block.h"

int getTextureNumber(Block b) {
    switch (b.getBlockType())
    {
    case MASSIVE:
        return 0;
    case HARD:
        return 1;
    case NORMAL_A:
        return 2;
    case NORMAL_B:
        return 3;
    case NORMAL_C:
        return 4;
    case RUBBLE:
        return 8+b.getRubbleAmount();
    case GROUND:
        return 8+RUBBLE_STEPS; //2nd row: 8. + RUBBLE_STEPS steps of rubble
    case PLATE:
        return 16;
    case BUILDING:
        return 16 + 2;    
    case WATER:
        return 4*8+2;
    case LAVA:
        return 4*8;
    default:
        return 8*8-1; //Missing texture picture
    }
}

sf::IntRect getTextureRect(Block b, int singleTextureSize) {
    int n = getTextureNumber(b);
    int x = n % TEXTURE_ROWS;
    int y = n / TEXTURE_ROWS;
    return sf::IntRect(x*singleTextureSize, y*singleTextureSize, singleTextureSize, singleTextureSize);
}

#define SPEED_DEFAULT 100
#define SPEED_RUBBLE 33
#define SPEED_PLATE 170

int getBlockMovementSpeed(Block b) {
    switch (b.getBlockType())
    {
    case PLATE:
    case BUILDING:
        return SPEED_PLATE;
    case RUBBLE:
    {
        float r ((float)b.getRubbleAmount() / (float)(RUBBLE_STEPS+1)); //Linear interpolation between the rubble levels, r=0 ist most rubble, r=1 is least rubble
        return (int)((float)SPEED_RUBBLE * (1-r) + (float)SPEED_DEFAULT * r);
    }
    default:
        return SPEED_DEFAULT;
    }
}

float getBlockMovementTimePriority(Block b) {
    if (b.getBlockType() == BUILDING) {
        return 10000.f / SPEED_RUBBLE;
    } else {
        return 10000.f / (float)getBlockMovementSpeed(b);
    }
}

bool isBlockWalkable(Block b) {
    BlockType t = b.getBlockType();
    return t == BlockType::BUILDING || t == BlockType::GROUND || t == BlockType::PLATE || t == BlockType::RUBBLE;
}

bool isBlockBreakableWall(Block b) {
    BlockType t = b.getBlockType();
    return t == NORMAL_A || t == NORMAL_B || t == NORMAL_C || t == HARD;
}
bool isBlockGeneralWall(Block b) {
    return isBlockBreakableWall(b) || b.getBlockType() == MASSIVE;
}

#define NORMAL_A_STRENGTH 45
#define NORMAL_B_STRENGTH 85
#define NORMAL_C_STRENGTH 150
#define HARD_STRENGTH 1200

int getBlockWallStrength(Block b) {
    switch (b.getBlockType())
    {
    case NORMAL_A:
        return NORMAL_A_STRENGTH;
    case NORMAL_B:
        return NORMAL_B_STRENGTH;
    case NORMAL_C:
        return NORMAL_C_STRENGTH;
    case HARD:
        return HARD_STRENGTH;
    default:
        throw std::runtime_error("Requested the strength of a block that is not a breakable wall");
        return -1;
    }
}

bool getBlockAbsorbingPickups(Block b) {
    switch (b.getBlockType()) 
    {
    case BUILDING:
        return b.getBuildingType() == BASE;
    }
    return false; 
}

bool getBlockHoldingPickups(Block b) {
    //Hey, you discovered a stub! 
    return false; 
}

bool getBlockStorageBuilding(Block b) {
    return b.getBuildingType() == BASE;
}