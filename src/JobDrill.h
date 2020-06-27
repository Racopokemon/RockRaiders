#ifndef JOB_DRILL_H
#define JOB_DRILL_H

#include <SFML/Graphics.hpp>
#include "Job.h"
#include "World.h"
#include "TileJobs.h"

class JobDrill : public Job {
    public : 
        JobDrill(std::shared_ptr<World> w, sf::Vector2i t) : Job(w) {
            target = t;
        }
        bool canBeExecutedBy(std::shared_ptr<JobDoer> jd) {
            //Actually, this is a problem. 
            return false;
        }
        void onActionFinished(int callNumber) {
            if (callNumber == 0) {
                //doer->walkTo(target); //Yeeeah, this is an issue ...
            } else if (callNumber == 1) {
                int * data = new int;
                *data = world->getMap()->getWallStrength(target);
                doer->playAnimation("drill", data);
            } else if (callNumber == 2) {
                world->getTileJobs()->unsetJobDrill(target);
                //world->destroyWall();

                doer->onJobFinished();//this might kill us, lets do all important things before!
            } else {
                throw std::runtime_error("onActionFinished called a 4th time on a JobDrill");
            }
        }
        void onCancelledByUser() {
            //... if the user actually cancelled, we of course not send another one. 
            //However ... if the worker died --> someone else should do it. 
            //If he was clicked away ... well ... I think this is the case where the job stays cancelled. 

            deleteLastReference();
        }

    protected : 
        sf::Vector2i target;
};

#endif