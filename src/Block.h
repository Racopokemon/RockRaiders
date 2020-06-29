#ifndef BLOCK_H
#define BLOCK_H

//!From 0 to 5 are 6 steps. 
#define RUBBLE_STEPS 6

#include <SFML/Graphics.hpp>
#include "BlockPropertiesLookup.h"


/*!
 * Every entry in the map is one of the here declared blockTypes. 
 */
enum BlockType {
    MASSIVE, HARD, NORMAL_A, NORMAL_B, NORMAL_C, GROUND, RUBBLE, PLATE, BUILDING
};

//!Up to now, there is only the basic base building where workers store all pickups and get spawned
enum BuildingType {
    BASE = 0
};

/*!
 * A block represents a single entry in the game's map. 
 * Depending on its BlockType it may be an empty cave part or an unpassable wall.
 * Internally we store two metadata values for each block, this doesn't matter on the outside though, 
 * if you know this is a rubble block and you are interested in the rubble amount, just call the 
 * so named function and we read the right meta out for you automatically. 
 */
struct Block {
    private :
        BlockType type;
        int metaA, metaB;
    public :
        Block() {};
        Block(BlockType type, int metaA, int metaB) {
            this->type = type;
            this->metaA = metaA;
            this->metaB = metaB;
        }
        Block(BlockType type) : Block(type, 0, 0) {}

        BlockType getBlockType() const {
            return type;
        }

        //! Just for debug reasons this also sets both meta values to -1, should be very obvious then to spot errors
        void setBlockType(const BlockType type) {
            this->type = type;
            metaA = -1;
            metaB = -1;
        }

        void setMetaA(int a) {
            this->metaA = a;
        };
        void setMetaB(int b) {
            this->metaB = b;
        };
        int getCrystalAmount() const {
            return metaA;
        }
        //! Also works for ore in rubble
        int getOreAmount() const {
            return metaB;
        }
        //!From 0 to RUBBLE_STEPS-1. 0 Is full throttle rubble in your face, RUBBLE_STEPS-1 is almost ground. 
        int getRubbleAmount() const {
            return metaA;
        }
        //!From 0 to RUBBLE_STEPS-1. 0 is maximum rubble, RUBBLE_STEPS-1 is closest to ground. 
        void setRubbleAmount(int const amount) {
            metaA = amount; 
        }
        //!Makes only sense for rubble of course
        void setOreAmount(int const amount) {
            metaB = amount; 
        }

        BuildingType getBuildingType();

        void setBuildingType(BuildingType type);

        //!True if this is some kind of ground block that entities can walk on.
        bool isWalkable();
        
        int getMovementSpeed();

        float getMovementTimePriority();

        //!Checks if the given position is a *breakable* wall block.
        bool isBreakableWall();

        //!Checks if the given position is any type of wall block.
        bool isGeneralWall();

        int getWallStrength();

        //!This is a very cheap and dirty function to access the meta values for the improvised editor. 
        void getMetaValuesForEditor(int * a, int * b) {
            *a = metaA;
            *b = metaB;
        }

        bool isAbsorbingPickups();
};

#endif