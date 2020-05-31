#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>
/*!
 * Every entry in the map is one of the here declared blockTypes. 
 */
enum BlockType {
    MASSIVE, HARD, NORMAL_A, NORMAL_B, NORMAL_C, GROUND, RUBBLE, PLATE, BUILDING
};

/*!
 * A block represents a single entry in the game's map. 
 * Depending on its BlockType it may be an empty cave part or an unpassable wall.
 * Internally we store two metadata values for each block, this doesn't matter on the outside though, 
 * if you know this is a rubble block and you are interested in the rubble amount, just call the 
 * so named function and we read the right meta out for you automatically. 
 * 
 * Blocks also know their texture coordinates for rendering. 
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
        BlockType setBlockType(const BlockType type) {
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
        int getRubbleAmount() const {
            return metaA;
        }
        //!From 0 to 5
        void setRubbleAmount(int const amount) {
            metaA = amount; 
        }
        //! Makes only sense for rubble of couse
        void setOreAmount(int const amount) {
            metaB = amount; 
        }

};

#endif