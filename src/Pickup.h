#ifndef PICKUP_H
#define PICKUP_H

#include "World.h"
#include "JobDoer.h"
#include "Job.h"
class JobPickup;
#include "LocatedEntity.h"
#include <iostream>
/**
 * !Pickups are rather lazy Entities, not really doing anything, even letting their carriers move them.
 * The only thing they care about is feeding the World with a new Job to pick them up when dropped on a
 * place in the ground where theyre not needed. 
 * 
 * While they are carried, pickups are updated with the updateWhileCarried function instead of the update function,
 * this might lead to hiccups (a second update in the pickup or drop tick) but shouldnt matter much. 
 */
class Pickup : public LocatedEntity {
    public : 
        Pickup() {} //!Don't use this except for placeholder initializations
        //!If startsAsCarried is false, we will notify the world about us being dropped (might create JobPickups etc.)
        Pickup(std::shared_ptr<World> w, sf::Vector2f pos, bool startsAsCarried);

        //Still in for debugging purposes
        virtual ~Pickup() {}

        void update();

        //!The name indicates it: JobDoers decide to pick up a Pickup
        //This only happens due to the JobPickup connected to this Pickup, ~~and that is being finished 
        //when the Pickup happened - so a Pickup forgets its Job here.~~ actually we don't delete the job on ourself, thats done by the job with unset job,
        //we have weird consistency errors when the pickup doesn't know its job anymore but the doer cancels the job which wahts to delete the job on us
        void bePickedUpBy(std::shared_ptr<JobDoer> j); 
        //!The name indicates it: JobDoers decide to drop a Pickup
        //A freshly dropped Pickup notifies the world about being dropped
        //A dropped Pickup has no job connected to it, the world decides if a new job is assigned (or if the Pickup maybe even gets deleted)
        void beDropped();

        //!This ... just sets the new position of this pickup to the given position. 
        //Since this call mostly replaces the update call while being carried, call this on every update as carrier! 
        void updateWhileCarried(sf::Vector2f position, bool interpolate); 

        //!Does what it says. 
        //Called by deletion etc. 
        //This Pickup then remains without Job. 
        void cancelJobBySystem();

        //!Used by the world to assign a new JobPickup for this Pickup. 
        //Requires that there is no other Job bound to this one yet. 
        //If the Job is deleted, it happens by system: use cancelJobBySystem().
        void setJob(std::shared_ptr<JobPickup> p);

        //!Requires that there is already a job set - and in the case someone decides that another job
        //is better to pick this pickup up, then this function will just forget the current job, without
        //calling anything on it. [The one calling this takes responsibility to assign a new job, should 
        //only be done by a job that is responsible about this from the beginning]
        void unsetJob();

        //!Needed by the world, when the ground changes and ... maybe we need a new job assigned, but maybe there is already
        //a worker coming for us ..
        bool hasJob();

        std::shared_ptr<JobPickup> getJob();

    protected : 

        virtual void prepareDeletion();

        std::shared_ptr<World> world; 
        //!If this doesn't point anywhere, we are not carried currently
        std::shared_ptr<JobDoer> carrier; 

        std::shared_ptr<JobPickup> job; 
};

#endif