#ifndef JOB_AT_TARGET
#define JOB_AT_TARGET

#include <SFML/Graphics.hpp>
#include "Job.h"

//!Turns out, most jobs consist of walking to a specific (not changing) target position [or several ones, eg. for drilling walls]
//and performing an action there. This is the base class for all these jobs. 
class JobAtTarget : public Job {

    public : 
        JobAtTarget() {};
        JobAtTarget(std::shared_ptr<World> w, std::vector<sf::Vector2f> targets);
        JobAtTarget(std::shared_ptr<World> w, sf::Vector2f target);
        //!This is also possible, if you populate the targets vector yourself in the constructor. 
        JobAtTarget(std::shared_ptr<World> w);
        virtual ~JobAtTarget() {}

        //!The worlds calls this on us before with a JobDoer before assigning the task to us. 
        //This is already implemented and checks, if the JobDoer can reach one of the targets. 
        //If you need more conditions, add them and return them together with JobAtTarget::canBeExecutedBy
        virtual bool canBeExecutedBy(std::shared_ptr<JobDoer> jd);
    
        //!Called by the JobDoer once he has finished the previous task we gave him,
        //the first task assigned in JobAtTarget is the walkTo task. [you can change this by overwriting onActionFinished]
        //When the last step is executed, first make sure that the target of the job knows that the job is finished, and by THIS job, so that it deletes
        //its reference to this Job. Then call onJobFinished on the JobDoer. After losing the referencing from the target and the JobDoer, 
        //this Job should get deleted automatically. (Every JobDoer does the deletion-call deleteLastReference() on us when were finished)
        //CallNumber is an internal counter, starting from 0 after onJobStarted, that you may use ... or just ignore. 
        virtual void onActionFinished(int callNumber) = 0; 

        //!Custom adaption, that does the walking in the first call already. 
        //You can overwrite this tho, and do other stuff that may come before walking,
        //we only add a counter anyway
        virtual void onActionFinished();

        //!This Job was cancelled (and the JobDoer already erased us etc.) but the Job probably still needs to be done, and the target still thinks that this
        //Job is on it! Decide if the Job should still be done, and then do the right preparations! Delete this job by removing the reference in the target and
        //call deleteLastReference(). 
        virtual void onCancelledByUser() = 0;

        //!Called by the world to determine the job with the shortest distance to a jobDoer. 
        std::vector<sf::Vector2f> getTargets();

    protected :
        std::vector<sf::Vector2f> targets;

};

#endif