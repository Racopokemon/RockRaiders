#include "Pickup.h"
#include "JobPickup.h"

Pickup::Pickup(std::shared_ptr<World> w, sf::Vector2f pos, bool startsAsCarried) {
    world = w;
    setPosition(pos);
    if (!startsAsCarried) {
        beDropped();
    }
}

void Pickup::bePickedUpBy(std::shared_ptr<JobDoer> j) {
    if (carrier) {
        throw new std::runtime_error("Wanted to pick up a Pickup that already is picked up by a JobDoer");
    }
    if (job) {
        job.reset();
    } else {
        std::cout << "Sure that it makes any sense that this pickup was picked up ... without a job assigned to pick it up? Looks like an error!" << std::endl;
    }
    carrier = j;
}

void Pickup::beDropped() {
    carrier.reset();
    job.reset(); //Shouldn't do anything anymore, we lose the job reference already as soon as someone picks us up
    if (!getDeleteRequested()) {//This check is important, since we end up in here also when our deletion is requested. 
        world->pickupDropped(std::dynamic_pointer_cast<Pickup>(ref()));
    }
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
    job = p;
}

void Pickup::unsetJob() {
    if (!job) {
        throw new std::runtime_error("Tried to change the job, but there was no one assigned before");
    }
    job.reset();
}

bool Pickup::hasJob() {
    return job.operator bool(); //Wtf was ist das wieder für ein absurder Syntax? Dass man das überhaupt noch kompilieren *kann* ...
}

std::shared_ptr<JobPickup> Pickup::getJob() {
    return job;
}
