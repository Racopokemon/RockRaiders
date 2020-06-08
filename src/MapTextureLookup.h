//! \file MapTextureLookup.h
//Just a mapping from blocks (some blocks might have several textures) to their textures,
//its hardcoded in here, but at least in an extra file for it. 
#ifndef MAPTEXTURELOOKUP_H
#define MAPTEXTURELOOKUP_H

#include <SFML/graphics.hpp>
#include "Block.h"

#define TEXTURE_ROWS 8//! Our overall texture has lines of 8 textures, so returning only a single index number is enough.

//(Simply call the getTextureRect function to get the right cutout.)
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
        return 8+6; //2nd row: 8. + 6 steps of rubble
    case PLATE:
        return 16;
    case BUILDING:
        return 16 + 2;    
    default:
        return 8*8-1;
    }
}

//! Returns the texture coordinates for the requested block. 
//For this we need the block (surprise) and the size of one texture in the texture. 
sf::IntRect getTextureRect(Block b, int singleTextureSize) {
    int n = getTextureNumber(b);
    int x = n % TEXTURE_ROWS;
    int y = n / TEXTURE_ROWS;
    return sf::IntRect(x*singleTextureSize, y*singleTextureSize, singleTextureSize, singleTextureSize);
}
#endif