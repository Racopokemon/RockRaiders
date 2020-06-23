#ifndef JOB_PICKUP_H
#define JOB_PICKUP_H

#include "Job.h"
#include "Pickup.h"

class JobPickup : public Job {
    public :
        JobPickup(std::shared_ptr<World> w, std::shared_ptr<Pickup> p);
};

#endif