#include "World.h"
#include <algorithm>
#include "string.h"
#include "MapLoader.h"
#include "main.cpp"
#include "SFML/Graphics.hpp"
#include "Worker.h"
#include "JobWalk.h"
#include "TileJobs.h"
#include "JobDrill.h"
#include "Crystal.h"
#include "Ore.h"
#include "JobDeliver.h"
#include "JobPickup.h"


World::World(std::string mapName) {
    reference = std::shared_ptr<World>(this);
    try {
        map = loadFromFile(mapName);
    } catch (std::exception& e) {
        std::cout << "Loading the file failed. ";
        std::cout << e.what();
        exit(-1);
    }
    addEntity(map);
    tileJobs = std::make_unique<TileJobs>(map->getWidth(), map->getHeight());
    srand(1337);

    for (int y = 0; y < map->getHeight(); y++) {
        for (int x = 0; x < map->getWidth(); x++) {
            sf::Vector2i pos(x, y);
            BlockType t = map->getBlockType(pos);
            if (t == GROUND || t == PLATE) {
                int w = map->getWorkersAtStart(pos);
                for (int i = 0; i < w; i++) {
                    Worker * w = new Worker(ref(), map->getRandomPositionInTile(pos, 0.05f));
                    addEntity(w->ref());
                }
            } else if (t == BUILDING) {
                if (map->isStorageBuilding(pos)) {
                    storageLocations.push_back(LocatedEntity::getTileCenter(pos) + sf::Vector2f(-0.3f, 0.3f));
                    storageTiles.push_back(pos);
                }
            }
        }
    }

}

World::~World() {
    std::cout << "DANG the world got deleted D::::: mommy pick me um im SCARED" << std::endl;
}

void World::requestJob(std::shared_ptr<JobDoer> j) {
    
    if (j->getPickup()) {
        vf closestStorage = map->getClosest(j->getPosition(), storageLocations);
        if (closestStorage == sf::Vector2f(-1.f, -1.f)) {
            std::cout << "We got a problem: Some worker picked up a pickup ... and cannot drop it anywhere. He will stand there forever now. " << std::endl;
            return;
        } else {
            JobDeliver * jd = new JobDeliver(ref(), closestStorage);
            j->setJob(std::dynamic_pointer_cast<JobDeliver>(jd->ref()));
        }
    } else {
        auto it = jobList.begin();
        while (it != jobList.end() && !(*it)->canBeExecutedBy(j)) {
            it++;
        }
        if (it != jobList.end()) {
            j->setJob(*it);
            jobList.erase(it);
        } else {
            //No job found for you, I'm sorry little one
        }
    }
}

void World::addJobToList(std::shared_ptr<Job> j) {
    jobList.push_back(j);
}

void World::removeJobFromList(std::shared_ptr<Job> j) {
    auto it = std::find(jobList.begin(), jobList.end(), j);
    if (it == jobList.end()) {
        std::cout << "Warning: We were called to remove a job from the joblist, that wasnt actually there!" << std::endl;
    } else {
        jobList.erase(it);
    }
}

std::vector<sf::Vector2f> World::getStorageLocations() {
    return storageLocations;
}

//!We still receive the float position, because this allows us e.g. to specify the exact position for workers to navigate to
void World::onTileClicked(sf::Vector2f pos) {
    //This is very ... ... very temp. 
    sf::Vector2i t = LocatedEntity::toTile(pos);
    if (!map->isVisible(t)) {
        return;
    }
    if (map->isPositionWalkable(t)) {
        JobWalk * j = new JobWalk(world, pos);
        addJobToList(j->ref());
    } else if (map->isBreakableWall(t)) {
        if (tileJobs->getJobDrill(t)) {
            tileJobs->cancelJobDrillBySystem(t);
        } else {
            JobDrill * jd = new JobDrill(ref(), t);
            addJobToList(jd->ref());
        }
    }
}

void World::destroyWall(sf::Vector2i pos) {
    int crystal, ore;
    map->destroyWall(pos, crystal, ore);
    tileJobs->deleteWall(pos);
    for (int i = 0; i < crystal; i++) {
        Crystal * c = new Crystal(ref(), map->getRandomPositionInTile(pos, 0.4f), false);
        addEntity(c->ref());
    }
    for (int i = 0; i < ore; i++) {
        Ore * o = new Ore(ref(), map->getRandomPositionInTile(pos, 0.2f), false);
        addEntity(o->ref());
    }
}

TileJobs * World::getTileJobs() {
    return tileJobs.get();
}

void World::pickupDropped(std::shared_ptr<Pickup> p) {
    pickupDropped(p, true);
}

void World::pickupDropped(std::shared_ptr<Pickup> p, bool droppedNew) {
    //Can be used for broundChanges! Just set droppedNew to false! 
    vi pos = p->getTile();
    if (map->isAbsorbingPickups(pos)) {
        p->requestDeletion();
        /*
        if (storageTiles.contains(p->getTile())) {
            //somehow increase the count or anything
        }
        */
    } else if (map->isHoldingPickups(pos)) {
        if (!droppedNew) {
            p->cancelJobBySystem();
        }
    } else {
        if (!p->hasJob()) {
            JobPickup * jp = new JobPickup(ref(), p);
            addJobToList(jp->ref());
        }
    }
}

GameMap * World::getMap() {
    return map.get();
}

std::shared_ptr<World> World::ref() {
    return reference;
}