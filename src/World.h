#ifndef WORLD_H
#define WORLD_H

#include "Pickup.h"
#include "Job.h"
#include "JobDoer.h"
#include "GameMap.h"

class World {
    public : 
        //! The mastermind of job scheduling. 
        //Here we assign a job to the JobDoer, that he is able to perform (also reachable etc.)
        void requestJob(std::shared_ptr<JobDoer> j) {
            //If jd is carrying something: Bring it somewhere

            //Ellllse: Check the job list and assign the first compatible job

            //Nothing found: Return nullptr
        }

        void addJobToList(std::shared_ptr<Job> j);

        void removeJobFromList(std::shared_ptr<Job> j);

        //! Every Pickup that is ever dropped on the ground (also when newly created) calls this.
        //The world will do one of the following three actions, depending on the ground below the Pickup: 
        //Just let the Pickup lay there (waiting for construction to finish)
        //Create a Pickup task for it (and tell the Pickup)
        //Remove the Pickup (Dropped in some storage)
        void pickupDropped(std::shared_ptr<Pickup> p);

        GameMap * getMap();

    protected : 

        std::shared_ptr<GameMap> map; 
};

#endif