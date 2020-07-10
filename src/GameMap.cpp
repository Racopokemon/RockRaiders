#include "GameMap.h"
#include "LocatedEntity.h"
#include "TextureLoader.h"
#include "Colors.h"

#define TEXTURE_TILES_PER_ROW 8 //finally doing them consts like a boss ... ok "boss" ...

void GameMap::update() {}


GameMap::GameMap(Block ** m, int width, int height, std::string textureName, std::shared_ptr<World> world) {
    this->world = world;
    this->width = width; 
    this->height = height; 

    map = m; 
    this->texture = TextureLoader().getTextureByName(textureName);
    //this->texture.setSmooth(true); 
    singleTextureSize = this->texture->getSize().x / TEXTURE_TILES_PER_ROW; 
    renderDataTextures = sf::VertexArray(sf::Quads, width*height*4); 

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            sf::Vector2i pos(x, y);
            Block & b = getBlock(pos);
            if (b.getVisibleAtStart()) {
                setVisible(pos, true);
                cameraCenterOnStart = LocatedEntity::getTileCenter(pos);
            }
        }
    }
    recalculateConnectedness();
} 

void GameMap::draw (sf::RenderTarget &target, float delta, bool debug) {
    if (viewModified) {
        viewModified = false;
        updateRenderData();
    }
    target.draw(renderDataTextures, texture);

    if (debug) {
        sf::Color col = sf::Color(255,255,255,100);
        auto g = getGraph();
        std::vector<sf::Vertex> vlist;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                Node * n = g->getNode(sf::Vector2i(x,y));
                for (Edge e : n->getEdges()) {
                    sf::Vector2i vec = e.target->getPosition();
                    vlist.push_back(sf::Vertex(sf::Vector2f(vec.x + 0.5f, vec.y + 0.5f), col));
                    vlist.push_back(sf::Vertex(sf::Vector2f(x + 0.5f, y + 0.5f), col));
                }
            }
        }
        if (!vlist.empty()) {
            target.draw(&vlist[0], vlist.size(), sf::Lines);
        }
    }
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
void GameMap::prepareDeletion() {
    Entity::prepareDeletion();
    world.reset();
}

bool GameMap::inMapBounds(sf::Vector2i pos) {
    return inBounds(pos.x, pos.y);
}

void GameMap::generateGraph() {
    Graph * g = new Graph(width, height);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (isPositionWalkable(x, y)) {
                sf::Vector2i start = sf::Vector2i(x, y);

                sf::Vector2i horz[1] = {sf::Vector2i(1, 0)};
                connectIfFree(*g, start, horz, 1, true);

                sf::Vector2i vert[1] = {sf::Vector2i(0, 1)};
                connectIfFree(*g, start, vert, 1, true);

                sf::Vector2i diag1[3] = {sf::Vector2i(0, 1), sf::Vector2i(1, 0), sf::Vector2i(1, 1)};
                connectIfFree(*g, start, diag1, 3, true);

                sf::Vector2i diag2[3] = {sf::Vector2i(0, 1), sf::Vector2i(-1, 0), sf::Vector2i(-1, 1)};
                connectIfFree(*g, start, diag2, 3, true);

                sf::Vector2i cross1[3] = {sf::Vector2i(1, 0), sf::Vector2i(1, 1), sf::Vector2i(2, 1)};
                connectIfFree(*g, start, cross1, 3, false);

                sf::Vector2i cross2[3] = {sf::Vector2i(0, 1), sf::Vector2i(1, 1), sf::Vector2i(1, 2)};
                connectIfFree(*g, start, cross2, 3, false);

                sf::Vector2i cross3[3] = {sf::Vector2i(0, 1), sf::Vector2i(-1, 1), sf::Vector2i(-1, 2)};
                connectIfFree(*g, start, cross3, 3, false);

                sf::Vector2i cross4[3] = {sf::Vector2i(-1, 0), sf::Vector2i(-1, 1), sf::Vector2i(-2, 1)};
                connectIfFree(*g, start, cross4, 3, false);
            }
        }
    }
    graph = std::shared_ptr<Graph>(g);
}

std::shared_ptr<Graph> GameMap::getGraph() {
    if (graphModified) {
        generateGraph();
        graphModified = false;
    }
    return graph;
}

//!See the documentation of findPathBetween in Graph.h
std::vector<sf::Vector2i> GameMap::findPathBetween(sf::Vector2i start, sf::Vector2i target, int & length, int exitConditionLength) {
    std::vector<sf::Vector2i> vec;
    vec.push_back(target);
    return findPathBetween(start, vec, length, exitConditionLength);
}

std::vector<sf::Vector2i> GameMap::findPathBetween(sf::Vector2i start, std::vector<sf::Vector2i> targets, int & length, int exitConditionLength) {
    return getGraph()->findPathBetween(start, targets, length, exitConditionLength);
}

std::vector<sf::Vector2i> GameMap::findPathBetween(sf::Vector2i start, sf::Vector2i target, int exitConditionLength) {
    int i;
    return findPathBetween(start, target, i, exitConditionLength);
}

std::vector<sf::Vector2i> GameMap::findPathBetween(sf::Vector2i start, std::vector<sf::Vector2i> targets, int exitConditionLength) {
    int i;
    return findPathBetween(start, targets, i, exitConditionLength);
}

int GameMap::getPathLength(sf::Vector2i start, sf::Vector2i target, int exitConditionLength) {
    int length = -1;
    findPathBetween(start, target, length, exitConditionLength);
    return length;
}
bool GameMap::connected(sf::Vector2i start, sf::Vector2i target) {
    return getBlock(start).getConnectedComponent() == getBlock(target).getConnectedComponent();
}
bool GameMap::connected(sf::Vector2f start, sf::Vector2f target) {
    return connected(LocatedEntity::toTile(start), LocatedEntity::toTile(target));
}
int GameMap::getPathLength(sf::Vector2i start, std::vector<sf::Vector2i> targets, int exitConditionLength) {
    int length = -1;
    findPathBetween(start, targets, length, exitConditionLength);
    return length;
}
bool GameMap::connected(sf::Vector2i start, std::vector<sf::Vector2i> targets) {
    int cc = getBlock(start).getConnectedComponent();
    for (auto v : targets) {
        if (getBlock(v).getConnectedComponent() == cc) {
            return true;
        }
    }
    return false; 
}

bool GameMap::connected(sf::Vector2f start, std::vector<sf::Vector2f> targets) {
    return connected(LocatedEntity::toTile(start), LocatedEntity::toTiles(targets));
}

sf::Vector2i GameMap::getClosest(sf::Vector2i start, std::vector<sf::Vector2i> targets) {
    int i;
    std::vector<sf::Vector2i> path = findPathBetween(start, targets);
    if (path.empty()) {
        return sf::Vector2i(-1, -1);
    } else {
        return path[path.size()-1];
    }
}
sf::Vector2f GameMap::getClosest(sf::Vector2f start, std::vector<sf::Vector2f> targets) {
    sf::Vector2i pos = getClosest(LocatedEntity::toTile(start), LocatedEntity::toTiles(targets));
    if (pos == sf::Vector2i(-1, -1)) {
        return sf::Vector2f(-1, -1);
    }
    for (sf::Vector2f v : targets) {
        if (LocatedEntity::toTile(v) == pos) {
            return v;
        }
    }
    std::cout << "Congrats, you reached a line of code that should be impossible to reach. (GameMap::getClosest)" << std::endl;
    return sf::Vector2f(-1, -1);
}

int GameMap::getMovementSpeed(sf::Vector2i pos) {
    return map[pos.x][pos.y].getMovementSpeed();
}

float GameMap::getMovementTimePriority(sf::Vector2i pos) {
    return map[pos.x][pos.y].getMovementTimePriority();
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

            sf::Color c = getBlock(sf::Vector2i(x, y)).getShade();
            for (int j = 0; j < 4; j++) {
                renderDataTextures[i+j].color = c;
            }

            i += 4;
        }
    }
}

bool GameMap::isPositionWalkable(sf::Vector2i pos) {
    return isPositionWalkable(pos.x, pos.y);
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

sf::Vector2f GameMap::getRandomPositionInTile(sf::Vector2i tile, float insetToAllSides) {
    sf::Vector2f pos = sf::Vector2f(rand()%1000 / 1000.f, rand()%1000 / 1000.f);
    pos -= sf::Vector2f(0.5f, 0.5f);
    pos *= 1.f-2*insetToAllSides;
    return pos + LocatedEntity::getTileCenter(tile);
}

//!We dont check the collision of the first position, we expect this to be already checked
void GameMap::connectIfFree(Graph & g, sf::Vector2i start, sf::Vector2i* pos, int listSize, bool onlyAverageFirstAndLast) {
    float sum = getMovementTimePriority(start);
    sf::Vector2i nextPos;
    for (int i = 0; i < listSize; i++) {
        nextPos = start+pos[i];
        if (!isPositionWalkable(nextPos.x, nextPos.y)) {
            return;
        }
        if (!onlyAverageFirstAndLast) {
            //We only ask for the movement speed when isPositionWalkable was true, meaning that our position was in bounds
            sum += getMovementTimePriority(nextPos);
        }
    }
    if (onlyAverageFirstAndLast) {
        sum += getMovementTimePriority(nextPos);
        sum /= 2.f;
    } else {
        sum /= (listSize+1);
    }
    
    int length = (int)(sum * hypotf(pos[listSize-1].x, pos[listSize-1].y));

    g.connect(start, nextPos, length);
}

void GameMap::setModified() {
    viewModified = true;
    graphModified = true; 
}

sf::Vector2i GameMap::getRandomPosition() {
    return sf::Vector2i(rand()%width, rand()%height);
}

//!why didnt I write this way earlier..
Block & GameMap::getBlock(sf::Vector2i pos) {
    return map[pos.x][pos.y];
}

void GameMap::getEditorData(int * metaA, int* metaB) {
    int index = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            getBlock(sf::Vector2i(x, y)).getMetaValuesForEditor(&metaA[index], &metaB[index]);
            index++;
        }
    }
}

BlockType GameMap::getBlockType(sf::Vector2i pos) {
    return getBlock(pos).getBlockType();
}

bool GameMap::isBreakableWall(sf::Vector2i pos) {
    return getBlock(pos).isBreakableWall();
}

bool GameMap::isGeneralWall(sf::Vector2i pos) {
    return getBlock(pos).isGeneralWall();
}

bool GameMap::isAbsorbingPickups(sf::Vector2i pos) {
    return getBlock(pos).isAbsorbingPickups();
}

bool GameMap::isHoldingPickups(sf::Vector2i pos) {
    return getBlock(pos).isHoldingPickups();
}

int GameMap::getWallStrength(sf::Vector2i pos) {
    return getBlock(pos).getWallStrength();
}

BuildingType GameMap::getBuildingType(sf::Vector2i pos) {
    return getBlock(pos).getBuildingType();
}

bool GameMap::isStorageBuilding(sf::Vector2i pos) {
    return getBlock(pos).isStorageBuilding();
}

void GameMap::destroyWall(sf::Vector2i pos, int & crystalNumber, int & oreNumber) {
    Block & b = getBlock(pos);
    crystalNumber = b.getCrystalAmount();
    oreNumber = b.getOreAmount();
    b.setBlockType(RUBBLE);
    b.setOreAmount(RUBBLE_STEPS-1);
    b.setRubbleAmount(0);
    if (b.isVisible()) {
        setVisible(pos, true);
    }
    updateConnectedness(pos);
    setModified();
}

bool GameMap::isVisible(sf::Vector2i pos) {
    return getBlock(pos).isVisible();
}

void GameMap::setVisible(sf::Vector2i pos, bool recursionStart) {
    if (!inMapBounds(pos)) {
        return;
    }
    Block & b = getBlock(pos);
    if (recursionStart || !b.isVisible()) {
        b.setVisible();
        b.setShade(COLORS_SHADE_NONE);
        if (!b.isGeneralWall()) {
            setVisible(pos + sf::Vector2i(0, 1), false);
            setVisible(pos + sf::Vector2i(0, -1), false);
            setVisible(pos + sf::Vector2i(1, 0), false);
            setVisible(pos + sf::Vector2i(-1, 0), false);
        }
        if (b.getBlockType() == GROUND || b.getBlockType() == PLATE) {
            int workers = b.getWorkersAtStart();
            if (workers > 0) {
                world->tileWithWorkersUncovered(pos, workers);
            }
        } else if (b.getBlockType() == ARTIFACT) {
            world->artifactDiscovered();
        }
    }
}

void GameMap::recalculateConnectedness() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Block & block = getBlock(sf::Vector2i(x, y));
            block.setConnectedComponent(-1);
        }
    }
    int ccIndex = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            sf::Vector2i pos = sf::Vector2i(x, y);
            Block & block = getBlock(pos);
            if (block.isWalkable() && block.getConnectedComponent() == -1) {
                //Discovered a yet undiscovered connected component. 
                discoverConnectedComponent(pos, -1, ccIndex++);
            }
        }
    }
}

void GameMap::updateConnectedness(sf::Vector2i pos) {
    Block & b = getBlock(pos);
    if (!b.isWalkable()) {
        return; //This will not ever happen, anyway. 
    }
    int cc = b.getConnectedComponent(); //should be -1
    cc = cc == -1 ? getBlock(pos + sf::Vector2i(0, 1)).getConnectedComponent() : cc;
    cc = cc == -1 ? getBlock(pos + sf::Vector2i(0, -1)).getConnectedComponent() : cc;
    cc = cc == -1 ? getBlock(pos + sf::Vector2i(1, 0)).getConnectedComponent() : cc;
    cc = cc == -1 ? getBlock(pos + sf::Vector2i(-1, 0)).getConnectedComponent() : cc;
    b.setConnectedComponent(cc);
    discoverConnectedComponentIfDifferent(pos + sf::Vector2i(0, 1), cc);
    discoverConnectedComponentIfDifferent(pos + sf::Vector2i(0, -1), cc);
    discoverConnectedComponentIfDifferent(pos + sf::Vector2i(1, 0), cc);
    discoverConnectedComponentIfDifferent(pos + sf::Vector2i(-1, 0), cc);
}

void GameMap::discoverConnectedComponentIfDifferent(sf::Vector2i pos, int expectedConnectedComponent) {
    Block & b = getBlock(pos);
    int actualCC = b.getConnectedComponent();
    if (b.isWalkable() && actualCC != expectedConnectedComponent) {
        discoverConnectedComponent(pos, actualCC, expectedConnectedComponent);
    }
}

void GameMap::discoverConnectedComponent(sf::Vector2i pos, int expectedConnectedComponent, int newConnectedComponent) {
    Block & b = getBlock(pos);
    if (b.isWalkable() && b.getConnectedComponent() == expectedConnectedComponent) {
        b.setConnectedComponent(newConnectedComponent);
        discoverConnectedComponent(pos + sf::Vector2i(0, 1), expectedConnectedComponent, newConnectedComponent);
        discoverConnectedComponent(pos + sf::Vector2i(0, -1), expectedConnectedComponent, newConnectedComponent);
        discoverConnectedComponent(pos + sf::Vector2i(1, 0), expectedConnectedComponent, newConnectedComponent);
        discoverConnectedComponent(pos + sf::Vector2i(-1, 0), expectedConnectedComponent, newConnectedComponent);
    }
}

sf::Vector2f GameMap::getCameraCenterOnStart() {
    return cameraCenterOnStart;
}

void GameMap::setGlobalVisibilityForEditor() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            getBlock(sf::Vector2i(x,y)).setVisible();
            getBlock(sf::Vector2i(x,y)).setShade(COLORS_SHADE_NONE);
        }
    }
    setModified();
}

void GameMap::setShade(sf::Vector2i pos, sf::Color color) {
    Block & b = getBlock(pos);
    if (!b.isVisible()) {
        throw "Cannot set shade of an not yet visible block. ";
    }
    b.setShade(color);
    setModified();
}

void GameMap::resetShade(sf::Vector2i pos) {
    setShade(pos, COLORS_SHADE_NONE);
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

