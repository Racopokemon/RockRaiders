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
    for (int i = 0; i < 28; i++) {
        sf::Vector2i pos = map->getRandomPosition();
        if (map->isPositionWalkable(pos)) {
            Worker * w = new Worker(ref(), LocatedEntity::getTileCenter(pos));
            addEntity(w->ref());
        }
    }
    for (int i = 0; i < 50; i++) {
        sf::Vector2i pos = map->getRandomPosition();
        if (map->isPositionWalkable(pos)) {
            sf::Vector2f dest = sf::Vector2f(pos.x, pos.y);
            sf::Vector2f rnd = sf::Vector2f((rand()%900+50)*0.001f, (rand()%900+50)*0.001f);
            dest+=rnd;
            //dest+=sf::Vector2f(0.5f, 0.5f);
            JobWalk* j = new JobWalk(ref(), dest);
            jobList.push_back(j->ref());
        }
    }
}

void World::requestJob(std::shared_ptr<JobDoer> j) {
    if (j->getPickup()) {
        std::cout << "We got a mate pickn up stuff, what were gonna do now? Are there finally ... delivery jobs?" << std::endl;
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

//!We still receive the float position, because this allows us e.g. to specify the exact position for workers to navigate to
void World::onTileClicked(sf::Vector2f pos) {
    //This is very ... ... very temp. 
    sf::Vector2i t = LocatedEntity::toTile(pos);
    if (map->isPositionWalkable(t)) {
        JobWalk * j = new JobWalk(world, pos);
        addJobToList(j->ref());
    } else if (map->isBreakableWall(t)) {
        if (tileJobs->getJobDrill(t)) {
            tileJobs->cancelJobDrillBySystem(t);
        } else {
            JobDrill * jd = new JobDrill(ref(), t);
            tileJobs->setJobDrill(t, std::dynamic_pointer_cast<JobDrill>(jd->ref()));
            addJobToList(jd->ref());
        }
    }
}

TileJobs * World::getTileJobs() {
    return tileJobs.get();
}

        //! Every Pickup that is ever dropped on the ground (also when newly created) calls this.
        //The world will do one of the following three actions, depending on the ground below the Pickup: 
        //Just let the Pickup lay there (waiting for construction to finish)
        //Create a Pickup task for it (and tell the Pickup)
        //Remove the Pickup (Dropped in some storage)
        void World::pickupDropped(std::shared_ptr<Pickup> p) {
            //TODO IMPLEMENT THIS!
        }

GameMap * World::getMap() {
    return map.get();
}

std::shared_ptr<World> World::ref() {
    return reference;
}