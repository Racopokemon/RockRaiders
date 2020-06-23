#include "Job.h"
#include "JobDoer.h"
#include "World.h"

Job::Job(std::shared_ptr<World> w) {
    world = w;
}

void Job::onJobStarted(std::shared_ptr<JobDoer> jd) {
    if (doer) {
        throw std::runtime_error("Job was started ... but there is already a JobDoer assigned to us! ");
    }
    doer = jd; 
    onActionFinished();
}

void Job::cancelBySystem() {
    if (doer) {
        doer->cancelJobBySystem();
        doer.reset();
    } else {
        world->removeJobFromList(ref());
    }
    deleteLastReference();
}

void Job::deleteLastReference() {
    reference.reset();
}

std::shared_ptr<Job> Job::ref() {
    return reference;
}