#include "GameMap.h"
#define TEXTURE_TILES_PER_ROW 8 //finally doing them consts like a boss ... ok "boss" ...

void GameMap::update() {}

void GameMap::draw (sf::RenderTarget &target, float delta) {
    if (viewModified) {
        viewModified = false;
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
    this->texture.setSmooth(true); 
    singleTextureSize = this->texture.getSize().x / TEXTURE_TILES_PER_ROW; 
    renderDataTextures = sf::VertexArray(sf::Quads, width*height*4); 
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

void GameMap::generateGraph() {
    Graph g = Graph(width, height);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (isPositionWalkable(x, y)) {
                sf::Vector2i start = sf::Vector2i(x, y);

                sf::Vector2i horz[1] = {sf::Vector2i(1, 0)};
                connectIfFree(g, start, horz, 1, true);

                sf::Vector2i vert[1] = {sf::Vector2i(0, 1)};
                connectIfFree(g, start, vert, 1, true);

                sf::Vector2i diag1[3] = {sf::Vector2i(0, 1), sf::Vector2i(1, 0), sf::Vector2i(1, 1)};
                connectIfFree(g, start, diag1, 3, true);

                sf::Vector2i diag2[3] = {sf::Vector2i(0, 1), sf::Vector2i(-1, 0), sf::Vector2i(-1, 1)};
                connectIfFree(g, start, diag2, 3, true);

                sf::Vector2i cross1[3] = {sf::Vector2i(1, 0), sf::Vector2i(1, 1), sf::Vector2i(2, 1)};
                connectIfFree(g, start, cross1, 3, false);

                sf::Vector2i cross2[3] = {sf::Vector2i(0, 1), sf::Vector2i(1, 1), sf::Vector2i(1, 2)};
                connectIfFree(g, start, cross2, 3, false);

                sf::Vector2i cross3[3] = {sf::Vector2i(0, 1), sf::Vector2i(-1, 1), sf::Vector2i(-1, 2)};
                connectIfFree(g, start, cross3, 3, false);

                sf::Vector2i cross4[3] = {sf::Vector2i(-1, 0), sf::Vector2i(-1, 1), sf::Vector2i(-2, 1)};
                connectIfFree(g, start, cross4, 3, false);
            }
        }
    }
    graph = std::shared_ptr<Graph>(&g);
}

std::shared_ptr<Graph> GameMap::getGraph() {
    if (graphModified) {
        generateGraph();
        graphModified = false;
    }
    return graph;
}

//!See the documentation of findPathBetween in Graph.h
std::vector<sf::Vector2u> GameMap::findPathBetween(sf::Vector2i start, sf::Vector2i target, int & length) {
    return getGraph()->findPathBetween(sf::Vector2u(start.x, start.y), sf::Vector2u(target.x, target.y), length);
}

void GameMap::updateRenderData() {
    int i = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            sf::IntRect textureTile = getTextureRect(map[x][y], singleTextureSize);

            renderDataTextures[i+0].position = sf::Vector2f(x+0.f, y+0.f);
            renderDataTextures[i+1].position = sf::Vector2f(x+1.f, y+0.f);
            renderDataTextures[i+2].position = sf::Vector2f(x+1.f, y+1.f);
            renderDataTextures[i+3].position = sf::Vector2f(x+0.f, y+1.f);

            renderDataTextures[i+0].texCoords = sf::Vector2f(textureTile.left + 0.5f, textureTile.top + 0.5f);
            renderDataTextures[i+1].texCoords = sf::Vector2f(textureTile.left + textureTile.width - 0.5f, textureTile.top + 0.5f);
            renderDataTextures[i+2].texCoords = sf::Vector2f(textureTile.left + textureTile.width - 0.5f, textureTile.top + textureTile.height - 0.5f);
            renderDataTextures[i+3].texCoords = sf::Vector2f(textureTile.left + 0.5f, textureTile.top + textureTile.height - 0.5f);

            i += 4;
        }
    }
}

//!Helper function that is used when we generate the graph. It does what it says, but for simplifications it does bound-checks and returns false if a position is out of the map
bool GameMap::isPositionWalkable(int x, int y) {
    if (!inBounds(x, y)) {
        return false;
    }
    return map[x][y].isWalkable();
}

bool GameMap::inBounds(int x, int y) {
    return !(x < 0 || x >= width || y < 0 || y > height);
}

//!We dont check the collision of the first position, we expect this to be already checked
void GameMap::connectIfFree(Graph g, sf::Vector2i start, sf::Vector2i* pos, int listSize, bool onlyAverageFirstAndLast) {
    int sum = map[start.x][start.y].getMovementSpeed();
    sf::Vector2i nextPos;
    for (int i = 0; i < listSize; i++) {
        nextPos = start+pos[i];
        if (!isPositionWalkable(nextPos.x, nextPos.y)) {
            return;
        }
        if (!onlyAverageFirstAndLast) {
            //We only ask for the movement speed when isPositionWalkable was true, meaning that our position was in bounds
            sum += map[nextPos.x][nextPos.y].getMovementSpeed();
        }
    }
    if (onlyAverageFirstAndLast) {
        sum += map[nextPos.x][nextPos.y].getMovementSpeed();
        sum /= 2;
    } else {
        sum /= (listSize+1);
    }
    g.connect(sf::Vector2u(start.x, start.y), sf::Vector2u(nextPos.x, nextPos.y), sum);
}

void GameMap::setModified() {
    viewModified = true;
    graphModified = true; 
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