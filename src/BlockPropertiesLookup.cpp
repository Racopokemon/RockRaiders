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
    default:
        return 8*8-1;
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

bool isBlockWalkable(Block b) {
    BlockType t = b.getBlockType();
    return t == BlockType::BUILDING || t == BlockType::GROUND || t == BlockType::PLATE || t == BlockType::RUBBLE;
}
