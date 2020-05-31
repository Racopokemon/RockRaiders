#include "GameMap.h"
#include "MapTextureLookup.h"
#define TEXTURE_TILES_PER_ROW 8 //finally doing them consts like a boss ... ok "boss" ...

bool GameMap::update() {
    return false;
}

void GameMap::draw (sf::RenderTarget &target, float delta) {
    if (renderDataNeedsToBeUpdatedBeforeNextDraw) {
        renderDataNeedsToBeUpdatedBeforeNextDraw = false;
        updateRenderData();
    }
    target.draw(renderDataTextures, &texture);
}

GameMap::GameMap(Block ** m, int width, int height, std::string texture) {
    this->width = width;
    this->height = height; 
    map = m;
    if (!this->texture.loadFromFile(texture)) {
        throw "Could not load texture file " + texture;
    }
    singleTextureSize = this->texture.getSize().x / TEXTURE_TILES_PER_ROW;
    renderDataTextures = sf::VertexArray(sf::Quads, width*height);
}
int GameMap::getHeight() {
    return height;
}
int GameMap::getWidth() {
    return width;
}
GameMap::~GameMap() {
    for (int i = 0; i < width; i++) {
        delete[height] map[i];
    }
    delete[width] map; 
}

void GameMap::updateRenderData() {
    int i = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < width; y++) {
            sf::IntRect textureTile = getTextureRect(map[x][y], singleTextureSize);

            renderDataTextures[i+0].position = sf::Vector2f(x, y);
            renderDataTextures[i+1].position = sf::Vector2f(x, y);
            renderDataTextures[i+2].position = sf::Vector2f(x, y);
            renderDataTextures[i+3].position = sf::Vector2f(x, y);
            //TODO continue this, once I found out how the coordinates are handled in here ...
            //how views can change this
            //and how transforms change this
            //and how I use all this to end up with a map that actually resembles the right view of the text file. 
            //https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php#example-tile-map compare here, they literally did the same thing already
        }
    }
}

/**! Here we assume that you know that this is block actually holds rubble. 
 * Reduces the rubble amount by 1 and turns the block into a normal ground block when all rubble was removed.
 * Returns true if this brings another ore to the light. 
 */
/*
bool removeRubble() {
    --metaA;
    bool removeRubble = false;
    if (metaA < metaB) {
        //Starting with 5/5 it should never be possible to create more than one ore at the same time from digging, 
        //we completely work with this assertion. 
        metaB = metaA;
        removeRubble = true;
    }
    
    if (metaA < 0) {
        metaA = 0;
        metaB = 0;
        type = GROUND;
    }
    return removeRubble;
}*/

/**! We expect that you know that this block actually was some drill-able block. 
 * Places rubble on the ground, primes it with ores.
 * Returns an two-elemented array with first the crystal-number and second with the ore-number. 
 */
/*
void drillBlock(int & crystalNumber, int & oreNumber) {
    crystalNumber = metaA;
    oreNumber = metaB;
    type = RUBBLE;
    metaA = 5;
    metaB = 5;
}*/