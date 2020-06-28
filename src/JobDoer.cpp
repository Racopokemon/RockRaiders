#include <iostream>
#include "JobDoer.h"
#include "Pickup.h"
#include "World.h"
#include "Job.h"
#include "GameMap.h"

#define JOB_DOER_BUSY state != idle && state != waiting

JobDoer::JobDoer(std::shared_ptr<World> w) {
    world = w;
}

void JobDoer::update() {
    LocatedEntity::update();
    if (dropNextTick) {
        dropNextTick = false;
        drop();
    }
    if (finished) {
        finished = false;
        if (job) {
            state = waiting; 
            job->onActionFinished();
        } else {
            state = idle; 
        }
        starting = true;
    } else if (state == idle) {
        //Every tick without a job is a lost tick! Let's spam the world with requests every tick until we get one! 
        world->requestJob(std::dynamic_pointer_cast<JobDoer>(ref()));
    }
    onBeginOfTick();

    if (state == idle) {
        if (starting) {
            starting = false; 
            initIdle();
        }
        idleAnimation();
    } else if (state == walking) {
        if (starting) {
            starting = false; 
            initPath();
        }
        if (followPath()) {
            finished = true;
        }
    } else if (state == animation) {
        if (starting) {
            starting = false; 
            initAnimation();
        }
        if (updateAnimation()) {
            finished = true;
            if (animationName == ANIMATION_DROP) {
                dropNextTick = true;
            }
        }
    } else { //waiting here, ... should not happen
        if (starting) {
            starting = false; 
            //...
        }
        //...
        std::cout << "Interestingly, we are waiting as a JobDoer for our Job to drop us the next command! " << std::endl;
    }

    if (pickup) {
        updatePickupAnimation();
    }
}

void JobDoer::onBeginOfTick() {}

//!Called by the World to determine a suitable Job for us
std::shared_ptr<Pickup> JobDoer::getPickup() {
    return pickup;
}

void JobDoer::setJob(std::shared_ptr<Job> j) {
    if (job) {
        throw new std::runtime_error("Tried to assign a job to a JobDoer that still has a job");
    }
    jobCancelled = false; 
    job = j;
    job->onJobStarted(std::dynamic_pointer_cast<JobDoer>(ref()));
}

void printVector(sf::Vector2i v) {
    std::cout << v.x << ", " << v.y << std::endl;
}

void JobDoer::walkTo(sf::Vector2f v) {
    if (JOB_DOER_BUSY) {
        throw std::runtime_error("WalkTo called on JobDoer, that was still busy with another task");
    }
    state = walking; 
    starting = true; 
    pathEnd = v;
    path = world->getMap()->findPathBetween(getTile(), toTile(v));
    if (path.empty()) {
        std::cout << "THIS IS AN ERROR! There was no path found to the destination of the JobDoer. Therefore the jobDoer is actually not able to perform the Job and cancels it now!" << std::endl;
        path.push_back(getTile());
        cancelJobByUser();
    }
}
void JobDoer::playAnimation(std::string s, void * data) {
    if (JOB_DOER_BUSY) {
        throw std::runtime_error("playAnimation called on JobDoer, that was still busy with another task");
    }
    state = animation;
    starting = true;
    dropOnAnimationEnd = false; 
    animationName = s;
    animationData = data;
}
void JobDoer::pickUpAnimation(std::shared_ptr<Pickup> p) {
    if (JOB_DOER_BUSY) {
        throw std::runtime_error("pickUpAnimation called on JobDoer, that was still busy with another task");
    }
    state = animation;
    starting = true;
    dropOnAnimationEnd = false; 
    animationName = ANIMATION_PICKUP;
    pickUp(p);
}
//!Inits animation "drop" and afterwards drop()s the Pickup.
void JobDoer::dropAnimation() {
    if (JOB_DOER_BUSY) {
        throw std::runtime_error("dropAnimation called on JobDoer, that was still busy with another task");
    }
    state = animation;
    starting = true;
    dropOnAnimationEnd = true; 
    animationName = ANIMATION_DROP;
}
void JobDoer::onJobFinished() {
    if (state != waiting && state != idle) {
        throw std::runtime_error("onJobFinished called on JobDoer, but we were still busy with some action! ");
    }
    job->deleteLastReference();
    job.reset();
    state = idle; 
}

void JobDoer::setNextJob(std::shared_ptr<Job> j) {
    if (JOB_DOER_BUSY) {
        throw std::runtime_error("setNextJob called on JobDoer, that was still busy with a task");
    }
    job->deleteLastReference(); //This actually deletes the Job. Done in here so that it doesnt need to be written in every subclass of Job. 
    job.reset();
    state = waiting; 
    setJob(j);
}

void JobDoer::pickUp(std::shared_ptr<Pickup> p) {
    if (pickup) {
        throw new std::runtime_error("Tried to have a JobDoer pickup a Pickup ... but it already carries a Pickup.");
    }
    pickup = p;
    p->bePickedUpBy(std::dynamic_pointer_cast<JobDoer>(ref()));
}

void JobDoer::drop() {
    pickup->beDropped();
    pickup.reset();
}

void JobDoer::cancelJobByUser() {
    job->onCancelledByUser();
    job.reset();
    jobCancelled = true; 
}

void JobDoer::cancelJobBySystem() {
    job.reset(); 
    jobCancelled = true; 
}

void JobDoer::prepareDeletion() {
    LocatedEntity::prepareDeletion();
    if (job) {
        cancelJobByUser();
    }
    if (pickup) {
        drop();
    }
}

bool JobDoer::isJobCancelled() {
    return jobCancelled;
}