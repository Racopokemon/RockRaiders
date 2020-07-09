#ifndef ICON_LOOKUP_H
#define ICON_LOOKUP_H

#define ICON_LOOKUP_TILE_COUNT 10

#define ICON_X 124
#define ICON_BACK 63
#define ICON_DRILL 9
#define ICON_TICK 144
#define ICON_1 40
#define ICON_2 30
#define ICON_3 20
#define ICON_LEAVE 64
#define ICON_HOME 173
#define ICON_START 32

#include <SFML/Graphics.hpp>

class IconLookup {
    public : 
        static sf::IntRect getIconTileFromIndex(int i, int textureWidth);
};

#endif