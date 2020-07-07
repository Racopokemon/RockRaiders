/**! \file CharBlockLookup.h
 * Cheap hard-coded lookup to turn chars from a map file into the right blocks. 
 * Every map also contains the conversion scheme in its header. 
 */

#ifndef CHARBLOCKLOOKUP_H
#define CHARBLOCKLOOKUP_H

#include <iostream>
#include "Block.h"

Block getBlockFromCharacter(char block) {
    switch (block)
    {
    case '#':
        return MASSIVE;
    case 'H':
        return HARD;
    case 'A':
        return NORMAL_A;
    case 'B':
        return NORMAL_B;
    case 'C':
        return NORMAL_C;
    case ' ':
        return GROUND;
    case '.':
        return RUBBLE;
    case '=':
        return PLATE;
    case '+':
        return BUILDING;
    case '~': 
        return WATER;
    case '-':
        return LAVA;
    default:
        std::cout << "WARNING: Char " << block << "does not map to a valid block, replaced with ground\n";
        return GROUND;
    }
    
}

#endif