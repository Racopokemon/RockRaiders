#ifndef JOB_DRILL_H
#define JOB_DRILL_H

#include <SFML/Graphics.hpp>
#include "JobAtTarget.h"
#include "World.h"
#include "TileJobs.h"
#include "Worker.h"
#include "GameMap.h"

#define DRILL_DISTANCE 0.05f

class JobDrill : public JobAtTarget {
    public : 
        JobDrill(std::shared_ptr<World> w, sf::Vector2i t) : JobAtTarget(w) {
            target = t;
            sf::Vector2f center = LocatedEntity::getTileCenter(t);
            //This will actually cause crashes when blocks at the rim of the map are not massive. 
            //But ... who would do that?
            targets.push_back(center + sf::Vector2f(0, 0.5f+DRILL_DISTANCE));
            targets.push_back(center + sf::Vector2f(0, -0.5f-DRILL_DISTANCE));
            targets.push_back(center + sf::Vector2f(0.5f+DRILL_DISTANCE, 0));
            targets.push_back(center + sf::Vector2f(-0.5f-DRILL_DISTANCE, 0));

            world->getTileJobs()->setJobDrill(t, std::dynamic_pointer_cast<JobDrill>(ref()));
        }
        void onActionFinished(int callNumber) {
            if (callNumber == 0) {
                int * data = new int;
                *data = world->getMap()->getWallStrength(target);
                doer->playAnimation(ANIMATION_DRILL, data);
            } else if (callNumber == 1) {
                world->getTileJobs()->unsetJobDrill(target);
                world->destroyWall(target);
                doer->onJobFinished();//this might kill us, lets do all important things before!
            } else {
                throw std::exception("onActionFinished called a 4th time on a JobDrill");
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