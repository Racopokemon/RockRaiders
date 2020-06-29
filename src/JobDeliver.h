#ifndef JOB_DELIVER_H
#define JOB_DELIVER_H

#include "Job.h"
#include "Shortcuts.h"
#include "LocatedEntity.h"
#include "JobDoer.h"
#include "World.h"
#include "TileJobs.h"

//!JobDeliver requires a JobDoer to already carry a pickup when assigned.
//The JobDoer then carries it (walks) to the target position and performs a drop there. 
//JobDeliver are never stored in a joblist, but instantly created for a specific JobDoer,
//who picked up a pickup and now requests a new job. 
//Actually, if we somehow cannot deliver the pickup 
class JobDeliver : public Job {
    public :
        JobDeliver(std::shared_ptr<World> w, sf::Vector2f t) : Job(w) {
            target = t;
            targetTile = LocatedEntity::toTile(target);
            world->getTileJobs()->addJobDeliver(targetTile, std::dynamic_pointer_cast<JobDeliver>(ref()));
        }

        virtual bool canBeExecutedBy(std::shared_ptr<JobDoer> jd) {
            //We are never in a jobList and therefore this is never checked. 
            //We are instantly created for a jobDoer. 
            return false;
        }

        virtual void onActionFinished(int callNumber) {
            if (callNumber == 0) {
                doer->walkTo(target);
            } else if (callNumber == 1) {
                doer->dropAnimation();
            } else if (callNumber == 2) {
                world->getTileJobs()->removeJobDeliver(targetTile, std::dynamic_pointer_cast<JobDeliver>(ref()));
                doer->onJobFinished();
            } else {
                throw std::runtime_error("onActionFinished called a 4th time on JobDeliver");
            }
        }

        virtual void onCancelledByUser() {
            //We do not restore this job, if the pickup is dropped, this causes a new pickup job for it to
            //be created, and on pickup we handle it again, and if the user interacted with us, after he finished 
            //what he wanted to do, if the jobDoer still carries the pickup, it will create a new JobDeliver again.
            world->getTileJobs()->removeJobDeliver(targetTile, std::dynamic_pointer_cast<JobDeliver>(ref()));
            deleteLastReference();
        }

    protected : 

        vf target;
        vi targetTile; 
};

#endif