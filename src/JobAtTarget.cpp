#include "JobAtTarget.h"
#include "World.h"
#include "GameMap.h"

JobAtTarget::JobAtTarget(std::shared_ptr<World> w, std::vector<sf::Vector2f> targets) : Job(w) {
    this->targets = targets;
}

JobAtTarget::JobAtTarget(std::shared_ptr<World> w, sf::Vector2f target) : Job(w) {
    targets.push_back(target);
}

JobAtTarget::JobAtTarget(std::shared_ptr<World> w) : Job(w) {}

bool JobAtTarget::canBeExecutedBy(std::shared_ptr<JobDoer> jd) {
    return world->getMap()->connected(jd->getPosition(), targets);
}

void JobAtTarget::onActionFinished() {
    //Were re-using callNumber here
    ++callNumber;
    if (callNumber == 0) {
        doer->walkTo(targets);
    } else {
        onActionFinished(callNumber-1);
    }
}

std::vector<sf::Vector2f> JobAtTarget::getTargets() {
    return targets;
}