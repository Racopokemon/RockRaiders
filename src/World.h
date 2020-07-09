#ifndef WORLD_H
#define WORLD_H

#include "Shortcuts.h"

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
        virtual ~World();

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
        //Create a Pickup task for it
        //Remove the Pickup (Absorbed by some storage)
        void pickupDropped(std::shared_ptr<Pickup> p);

        //!We still receive the float position, because this allows us e.g. to specify the exact position for workers to navigate to
        void onTileClicked(sf::Vector2f pos);

        //!We expect that you know this to actually be a wall. Changes the map and spawns ores. 
        void destroyWall(sf::Vector2i pos);

        //!It's only a copy, do with it whatever you want. 
        std::vector<sf::Vector2f> getStorageLocations();

        TileJobs * getTileJobs();

        GameMap * getMap();

        std::shared_ptr<World> ref();

        //!Has yet to be implemented, how to do this, where, etc. 
        //We should start clearing (deleting) all Entities, who will then also somehow
        //cancel their jobs (... maybe) and then finally we can self-destruct with reference.reset()
        //Also clears the entities list. 
        void deleteWorld();

        void getGameStatPointers(int ** crystals, int ** ores, int ** workers);

        //!Called after all entities were updated. Really doesn't do much, plays no role in deleting, 
        //but on showing messages, which should happen after all updates and not in between (menus might still change mid-entity-list)
        //
        //General approach: Show and exit menus at anytime, even mid-update, they are handled instantly and nobody cares
        //but set messages only before or after the update-cascade
        void update();

        //!This is relevant because the menu changes due to this, and upon return, it is replaced with the default menu. 
        //Therefore, we should cancel all our selections (which might also change the menu, but thats fine because this is called before)
        //to match the state after the message was exited. 
        void onMessageShown();

    protected : 

        void pickupDropped(std::shared_ptr<Pickup> p, bool droppedNew);

        std::shared_ptr<GameMap> map; 
        std::list<std::shared_ptr<Job>> jobList; 

        std::vector<sf::Vector2f> storageLocations;
        std::vector<sf::Vector2i> storageTiles;

        std::unique_ptr<TileJobs> tileJobs;
        
        std::shared_ptr<World> reference;

        int workers = 0;
        int crystals = 0;
        int ores = 0;

        //!If the win conditions are fulfilled, we get everything ready to show the message and store that this level is won. 
        void checkWin();
        bool won = false;
        bool showWinMessage = false; 
        bool showStartMessage = true; 
};

#endif