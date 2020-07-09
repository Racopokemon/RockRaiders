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
#include "MenuLevelStart.h" 
#include "MenuLevelWon.h" 

World::World(std::string mapName) {
    reference = std::shared_ptr<World>(this);
    try {
        map = loadFromFile(mapName, data);
    } catch (std::exception& e) {
        std::cout << "Loading the file failed. ";
        std::cout << e.what();
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
                    workers++; 
                    //TODO: Once this happens on other places as well, this should be an additional function. 
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
    //Can be used for groundChanges! Just set droppedNew to false! 
    vi pos = p->getTile();
    if (map->isAbsorbingPickups(pos)) {
        p->requestDeletion();
        if (find(storageTiles.begin(), storageTiles.end(), pos) != storageTiles.end()) {
            //https://stackoverflow.com/questions/11951121/checking-if-a-pointer-points-to-a-particular-class-c
            if (dynamic_cast<Crystal*>(p.get())) {
                crystals++;
            } else if (dynamic_cast<Ore*>(p.get())) {
                ores++;
            }
            checkWin();
        }
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

void World::getGameStatPointers(int ** crystals, int ** ores, int ** workers) {
    *crystals = &this->crystals;
    *ores = &this->ores;
    *workers = &this->workers;
}

void World::update() {
    if (showWinMessage) {
        showWinMessage = false;
        setMessage<MenuLevelWon>(data.winText);
    } else if (showStartMessage) {
        showStartMessage = false; 
        //Its better to schedule this and wait until one update and some frames have been seen until we show the message - 
        //it might eg happen that main.cpp was still initializing the game speed after etc. 
        setMessage<MenuLevelStart>(data.startText);
    }
}

 void World::checkWin() {
    if (!won) {
        int numberWinConditions = 0;
        int numberFulfilledConditions = 0;
        if (data.crystalsToWin > 0) {
            numberWinConditions++;
            if (crystals >= data.crystalsToWin) {
                numberFulfilledConditions++;
            }
        }
        if (data.oresToWin > 0) {
            numberWinConditions++;
            if (ores >= data.oresToWin) {
                numberFulfilledConditions++;
            }
        }
        //artifacts are not (yet) implemented, but should be easy and stuff ...
        if (data.basesToWin > 0) {
            numberWinConditions++;
            if (storageLocations.size() >= data.basesToWin) { 
                //Strictly taken, this will become wrong in the future, when there are several bases etc and not all are storageLocations etc. 
                numberFulfilledConditions++;
            }
        }
        if (numberWinConditions == numberFulfilledConditions) {
            won = true;
            showWinMessage = true;
        }
    }
}

void World::onMessageShown() {
    //If selections are implemented: Remove them here! 
}

void World::deleteWorld() {
    //Better not look too close at this hacky function that hopefully gets all circular references erased without causing trouble


    //Let's try to cancel as many jobs by system as possible: 

    //We delete all pickups, if theyre on the ground, this cancels their jobs (but they keep inside the Jobs list, because usually we delete them first)
    //If they are being carried, they are dropped and ~~create even new jobs to pick them up~~ that was an issue in general and is fixed now
    for (std::shared_ptr<Entity> e : entities) { //Yess, this is a hack, were directly using the list from main.cpp. 
        //But for later, we earlier or later needed access to this anyway, we might change this later (but only a bit, giving a pointer)
        if (std::shared_ptr<Pickup> p = std::dynamic_pointer_cast<Pickup>(e)) {
            p->requestDeletion();
        }
    }
    //Easy thing with TileJobs: 
    tileJobs->cancelAllJobs();

    //The only jobs that remain after this are JobWalks. 
    for (std::shared_ptr<Entity> e : entities) { //Yess, this is a hack, were directly using the list from main.cpp. 
        //But for later, we earlier or later needed access to this anyway, we might change this later (but only a bit, giving a pointer)
        if (std::shared_ptr<JobDoer> p = std::dynamic_pointer_cast<JobDoer>(e)) {
            p->requestDeletion(); //By now: The only remaining walk jobs are not recovered
        }
    }

    //Only thing left are (not yet assigned) JobWalks, and they don't point anywhere
    jobList.clear();

    //Looking closely, it should actually not matter much in which order I delete the elements out of the entities vector, they should all be able to manage this properly. 
    //The situation we avoided is deleting workers who carry pickups that would then still request a new job after they got dropped forcefully

    entities.clear();

    map->requestDeletion();
    map.reset();

    reference.reset();
}