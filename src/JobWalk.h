#ifndef JOB_WALK_H
#define JOB_WALK_H

#include "JobAtTarget.h"
#include <SFML/Graphics.hpp>

class JobWalk : public JobAtTarget {
    public :
        JobWalk(std::shared_ptr<World> w, sf::Vector2f target) : JobAtTarget(w, target) {}

        virtual void onActionFinished(int callNumber) {
            if (callNumber == 0) {
                 doer->onJobFinished();
            } else {
                throw "3rd onActionFinished call on JobWalk";
            }
        }
        virtual void onCancelledByUser() {
            //Walk commands are (later on) usually caused by users - if they cancel
            //it again, we don't need to restore anything. 
            //Therefore we just compete the deletion of the Job:
            deleteLastReference();
        }
};

#endif