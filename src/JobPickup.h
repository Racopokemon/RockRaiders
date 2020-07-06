#ifndef JOB_PICKUP_H
#define JOB_PICKUP_H

#include "Shortcuts.h"
#include "Job.h"
#include "Pickup.h"
#include <iostream>

class JobPickup : public Job {
    public :
        JobPickup(std::shared_ptr<World> w, std::shared_ptr<Pickup> p) : Job(w) {
            pickup = p;
            p->setJob(std::dynamic_pointer_cast<JobPickup>(ref()));
        }

        virtual ~JobPickup() {}

        virtual bool canBeExecutedBy(std::shared_ptr<JobDoer> jd) {
            if (world->getMap()->connected(pickup->getPosition(), world->getStorageLocations())) { //this later needs to be expanded for any place to drop pickups (all constructions)
                return world->getMap()->connected(jd->getPosition(), pickup->getPosition());
            }
            return false; //No place (storage locations at least) to drop this pickup, ignore me please. 
        }

        virtual void onActionFinished(int callNumber) {
            if (callNumber == 0) {
                doer->walkTo(pickup->getPosition());
            } else if (callNumber == 1) {
                doer->pickUpAnimation(pickup);
            } else if (callNumber == 2) {
                //Pickups already drop their reference to their job when they get picked up. 
                doer->onJobFinished();
            } else {
                throw std::runtime_error("onActionFinished called a 4th time on a JobPickup");
            }
        } 

        virtual void onCancelledByUser() {
            pickup->unsetJob();
            JobPickup * jp = new JobPickup(world, pickup);
            world->addJobToList(jp->ref());
            deleteLastReference();
        }

    protected :
        sp<Pickup> pickup;
};

#endif