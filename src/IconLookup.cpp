#include "IconLookup.h"
sf::IntRect IconLookup::getIconTileFromIndex(int i, int textureWidth) {
    int tileSize = textureWidth / ICON_LOOKUP_TILE_COUNT;
    return sf::IntRect((i % ICON_LOOKUP_TILE_COUNT) * tileSize, (i / ICON_LOOKUP_TILE_COUNT) * tileSize, tileSize, tileSize);
}
