#ifndef JOB_DOER_H
#define JOB_DOER_H

class Pickup;
class Job;
class Pickup;
class World;
#include "LocatedEntity.h"

#define ANIMATION_DROP "drop"
#define ANIMATION_PICKUP "pickup"

class JobDoer : public LocatedEntity {
    public : 
        //!But don't use this except for default placeholder initializations.
        JobDoer() {};
        JobDoer(std::shared_ptr<World> w);

        virtual ~JobDoer() {}

        //!Not virtual anymore, we move the functionality from here to a bunch of other virtual functions for special cases. 
        void update();

        //!Called by the World to determine a suitable Job for us
        std::shared_ptr<Pickup> getPickup();

        //!The place where everything starts, called by the world after we requested a job.
        void setJob(std::shared_ptr<Job> j);

        //Things Jobs tell us to do

        void walkTo(sf::Vector2f v);
        //!Depending on the animation name, you can pass additional information with the data pointer 
        //(that is then known by you and the JobDoer). Put the data on the heap, we might only read it later.
        void playAnimation(std::string s, void * data = nullptr);
        //!Instantly picks up the Pickup and inits animation ANIMATION_PICKUP
        void pickUpAnimation(std::shared_ptr<Pickup> p);
        //!Inits animation ANIMATION_DROP and afterwards drop()s the Pickup.
        void dropAnimation();
        void onJobFinished();
        //!Extension to onJobFinished, where we instantly get a new Job assigned instead of asking the World for it
        void setNextJob(std::shared_ptr<Job> j);

        //!Within one update-step, we first call the job for the next task (if our previous task was finished with a true return value in the previous tick) 
        //(or request a new job), then call the init funcions if we start some new animation or task, and directly after update it. 
        //In the very end we update the pickup position. 
        //You might need a call that happens before we init or update animations, and thats what this function is for. 
        virtual void onBeginOfTick();
        //!Place for individual motion styles, using your own variables. 
        //Don't move here yet, the first followPath() call comes directly after! 
        virtual void initPath() = 0;
        //!!Place for individual motion styles, called in every update. The path is already calculated and stored in
        //path, starting with the current tile and ending at the target tile, pathEnd is the precise position on the target tile. 
        //True means the following has ended and were up to do the next thing a job wants us to do. 
        //Maybe call isJobCancelled() every now and then, in this case you should directly stop walking your path. 
        virtual bool followPath() = 0;
        //!Place for individual animations, using your own variables. The name of the animation is written to animationName,
        //further custom animation data is contained in animationData. Make sure to *delete* it at the end of the animation, 
        //best practice would be to delete it right in here! 
        //Don't animate here yet, the first updateAnimation() call comes directly after! 
        virtual void initAnimation() = 0;
        //!!Place for individual animations, called in every update. 
        //True means the animation has finished and were up to do the next thing a job wants us to do. 
        //Maybe call isJobCancelled() every now and then, in this case you should directly end the animation. 
        virtual bool updateAnimation() = 0;
        //!Called at the end of each update cycle, if we are carrying a Pickup. You MUST call pickup->updateWhileCarried() 
        //in here so that the Pickup is somehow carried by the JobDoer
        virtual void updatePickupAnimation() = 0;
        //!Called once an idle-state ([no job], no path, no animation, feels like unemployment) is entered.
        //Use this only for initialization purposes, the first idleAnimation call comes directly after!
        virtual void initIdle() = 0;
        //!Called once per update in every tick in that we do not have a path or animation to follow. 
        virtual void idleAnimation() = 0;

        //!Does only the data-part (linking both to each other). 
        //For normal cases you shouldCall pickUpWithAnimation() instead, which also calls this. 
        void pickUp(std::shared_ptr<Pickup> p);
        //Unlinks the current Pickup from this JobDoer, but without animation. 
        //For normal purposes, you might therefore call dropAnimation() instead. 
        void drop();

        //!Actually, this doesnt need to be the user, if the JobDoer is deleted for some reason, it is also caused from the
        //job-performers side and someone still might do the job.
        //After this call, all pointers are unset etc, the Job knows it was cancelled,
        //the walk / animation however might continue, depending on the implementation. 
        void cancelJobByUser();

        //Invoked by our Job. The target, and with it our Job became somehow invalid, and all left to do is
        //to delete the reference to that Job and cancel whatever path or animation is currently running. 
        void cancelJobBySystem();

        virtual void prepareDeletion();

    protected : 

        //!Waiting should never be visible, this happens once Jobs work in threads and might give their next command later on. 
        enum JobDoerState {idle, walking, animation, waiting};

        std::shared_ptr<World> world; 
        std::shared_ptr<Pickup> pickup; 
        std::shared_ptr<Job> job; 
        
        bool isJobCancelled(); 

        //Name of the animation that is currently executed.
        std::string animationName;
        //!Path to walk when walkPath is called. If we should only move on the tile, the current tile is still inside the path. 
        //The exact target position is stored in pathEnd
        std::vector<sf::Vector2i> path;
        //Exact position where to arrive at the target tile at the end of the path
        sf::Vector2f pathEnd; 

        //!On heap, delete this within this animation ... I suggest doing it directly in initAnimation. 
        void* animationData;
    private :
        bool jobCancelled; 
        JobDoerState state = idle; 
        bool starting = true;
        bool finished = false; 
        bool dropOnAnimationEnd = false; 
        bool dropNextTick = false; 
};

#endif