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

#include <SFML/Graphics.hpp>

sf::FloatRect getIconTileFromIndex(int i, int textureWidth) {
    int tileSize = textureWidth / ICON_LOOKUP_TILE_COUNT;
    return sf::FloatRect((i % ICON_LOOKUP_TILE_COUNT) + 0.5f, (i / ICON_LOOKUP_TILE_COUNT) + 0.5f, tileSize - 1, tileSize - 1);
}


#endif