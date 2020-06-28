#ifndef JOB_DRILL_H
#define JOB_DRILL_H

#include <SFML/Graphics.hpp>
#include "Job.h"
#include "World.h"
#include "TileJobs.h"
#include "Worker.h"

class JobDrill : public Job {
    public : 
        JobDrill(std::shared_ptr<World> w, sf::Vector2i t) : Job(w) {
            target = t;
        }
        bool canBeExecutedBy(std::shared_ptr<JobDoer> jd) {
            return world->getMap()->connected(jd->getTile(), getAdjacentPositions());
        }
        void onActionFinished(int callNumber) {
            if (callNumber == 0) {
                auto path = world->getMap()->findPathBetween(doer->getTile(), getAdjacentPositions());
                sf::Vector2i dest = path[path.size()-1];
                sf::Vector2i diff = target - dest;
                doer->walkTo(sf::Vector2f(dest.x + 0.5f + diff.x*0.45f, dest.y + 0.5f + diff.y*0.45f)); 
            } else if (callNumber == 1) {
                int * data = new int;
                *data = world->getMap()->getWallStrength(target);
                doer->playAnimation(ANIMATION_DRILL, data);
            } else if (callNumber == 2) {
                world->getTileJobs()->unsetJobDrill(target);
                world->destroyWall(target);
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
        std::vector<sf::Vector2i> getAdjacentPositions() {
            std::vector<sf::Vector2i> pos;
            pos.push_back(target+sf::Vector2i(0, 1)); 
            pos.push_back(target+sf::Vector2i(0, -1)); 
            pos.push_back(target+sf::Vector2i(1, 0)); 
            pos.push_back(target+sf::Vector2i(-1, 0)); 
            //This actually might cause crashes when blocks at the rim of the map are not massive. 
            //But ... who would do that?
            return pos;
        }
};

#endif