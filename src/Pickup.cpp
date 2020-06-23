#include "Pickup.h"

Pickup::Pickup(std::shared_ptr<World> w, bool startsAsCarried) {
    world = w;
    if (!startsAsCarried) {
        beDropped();
    }
}

void Pickup::bePickedUpBy(std::shared_ptr<JobDoer> j) {
    if (!carrier) {
        throw new std::runtime_error("Wanted to pick up a Pickup that already is picked up by a JobDoer");
    }
    carrier = j;
}

void Pickup::beDropped() {
    carrier.reset();
    job.reset();
    world->pickupDropped(std::dynamic_pointer_cast<Pickup>(ref()));
}

void Pickup::update() {
    if (!carrier) {
        //We are being carried and therefore our update is instead executed in the updateWhileCarried function. 
        return;
    } else {
        LocatedEntity::update();
        //Actually, if we are not carried, we do noting either. Maybe in the future? Animations? ... who knows
    }
}
        
//Be careful if you call the requestDeletion anywhere, this forcefully 
//destroys the Pickup and hard-cancels the Job bound to it. 
void Pickup::prepareDeletion() {
    LocatedEntity::prepareDeletion();
    if (carrier) {
        //This should never happen, but for the case lets restore some order
        carrier->drop();
        carrier.reset();
    }
    cancelJobBySystem();
};

void Pickup::updateWhileCarried(sf::Vector2f position, bool interpolate) {
    LocatedEntity::update();
    setPosition(position, interpolate);
}

void Pickup::cancelJobBySystem() {
    if (job) {
        job->cancelBySystem();
        job.reset();
    }
}

void Pickup::setJob(std::shared_ptr<JobPickup> p) {
    if (job) {
        throw new std::runtime_error("Tried to setJob on a pickup, that already has a job assigned!");
    }
}

std::shared_ptr<JobPickup> Pickup::getJob() {
    return job;
}
