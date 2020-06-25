#ifndef JOB_WALK_H
#define JOB_WALK_H

#include "Job.h"
#include <SFML/Graphics.hpp>

class JobWalk : public Job {
    public :
        JobWalk(std::shared_ptr<World> w, sf::Vector2f target) : Job(w) {
            this->target = target;
        }

        virtual bool canBeExecutedBy(std::shared_ptr<JobDoer> jd) {
            return world->getMap()->connected(jd->getPosition(), target);
        }

        virtual void onActionFinished() {
            if (!nextActionIsFinish) {
                doer->walkTo(target);
                nextActionIsFinish = true;
            } else {
                doer->onJobFinished();
            }
        }
        virtual void onCancelledByUser() {
            //Walk commands are (later on) usually caused by users - if they cancel
            //it again, we don't need to restore anything. 
            //Therefore we just compete the deletion of the Job:
            deleteLastReference();
        }
    protected :
        bool nextActionIsFinish = false; 
        sf::Vector2f target;
};

#endif