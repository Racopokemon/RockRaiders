#include "World.h"
#include <algorithm>
#include "string.h"
#include "MapLoader.h"
#include "main.cpp"
#include "SFML/Graphics.hpp"
#include "Worker.h"
#include "JobWalk.h"

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
    srand(42069);
    for (int i = 0; i < 20; i++) {
        sf::Vector2i pos = map->getRandomPosition();
        if (map->isPositionWalkable(pos)) {
            Worker * w = new Worker(ref(), LocatedEntity::getTileCenter(pos));
            addEntity(w->ref());
        }
    }
    for (int i = 0; i < 500; i++) {
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