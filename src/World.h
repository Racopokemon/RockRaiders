#ifndef WORLD_H
#define WORLD_H

/*
class JobDoer;
class Pickup;
class GameMap;
class Job;
*/

#include "Pickup.h"
#include "Job.h"
#include "JobDoer.h"
#include "GameMap.h"
class TileJobs;
#include <list>

/**
 * By now, the World turns more and more into the Job coordinator, knowing all Jobs and the map and doing stuff for them
 */
class World {
    public : 
        World(std::string mapName);

        //! The mastermind of job scheduling. 
        //Here we assign a job to the JobDoer, that he is able to perform (also reachable etc.)
        void requestJob(std::shared_ptr<JobDoer> j);
            //If jd is carrying something: Bring it somewhere

            //Ellllse: Check the job list and assign the first compatible job

            //Nothing found: Return nullptr

        void addJobToList(std::shared_ptr<Job> j);

        void removeJobFromList(std::shared_ptr<Job> j);

        //! Every Pickup that is ever dropped on the ground (also when newly created) calls this.
        //The world will do one of the following three actions, depending on the ground below the Pickup: 
        //Just let the Pickup lay there (waiting for construction to finish)
        //Create a Pickup task for it (and tell the Pickup)
        //Remove the Pickup (Dropped in some storage)
        void pickupDropped(std::shared_ptr<Pickup> p);

        //!We still receive the float position, because this allows us e.g. to specify the exact position for workers to navigate to
        void onTileClicked(sf::Vector2f pos);

        TileJobs * getTileJobs();


        GameMap * getMap();

        std::shared_ptr<World> ref();

        //!Has yet to be implemented, how to do this, where, etc. 
        //We should start clearing (deleting) all Entities, who will then also somehow
        //cancel their jobs (... maybe) and then finally we can self-destruct with reference.reset()
        void deleteWorld();

    protected : 

        std::shared_ptr<GameMap> map; 
        std::list<std::shared_ptr<Job>> jobList; 

        std::unique_ptr<TileJobs> tileJobs;
        
        std::shared_ptr<World> reference;
};

#endif