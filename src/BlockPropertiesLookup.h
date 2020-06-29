//! \file MapTextureLookup.h
//Just a mapping from blocks (some blocks might have several textures) to their textures, and a mapping of the movement speeds of different block types
//its hardcoded in here, but at least in an extra file for it. 
#ifndef BLOCKPROPERTIES_H
#define BLOCKPROPERTIES_H

#include <SFML/graphics.hpp>

struct Block;

//! Our overall texture has lines of 8 textures, so returning only a single index number is enough.
#define TEXTURE_ROWS 8

//(Simply call the getTextureRect function to get the right cutout.)
int getTextureNumber(Block b);

//! Returns the texture coordinates for the requested block. 
//For this we need the block (surprise) and the size of one texture in the texture. 
sf::IntRect getTextureRect(Block b, int singleTextureSize);

int getBlockMovementSpeed(Block b);
float getBlockMovementTimePriority(Block b);
bool isBlockWalkable(Block b);

bool isBlockBreakableWall(Block b);
bool isBlockGeneralWall(Block b);
//!Number of ticks it takes to break the wall (as normal worker)
int getBlockWallStrength(Block b);

bool getBlockAbsorbingPickups(Block b);

bool getBlockHoldingPickups(Block b);

bool getBlockStorageBuilding(Block b);

#endif