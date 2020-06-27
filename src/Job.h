#ifndef JOB_H
#define JOB_H

class World;
class JobDoer;

#include <SFML/Graphics.hpp>

//Jobs ...
//lots of derived classes, each with storage on the specific task (pointer or coordinates mostly)
//Jobs change the data of the map, and delete ores etc when delivered. JobDoers only perform the motions and animations. 
//Every Job must call the onJobFinished on their JobDoer once they are finished.
//Jobs can be cancelled from two sides. 
//Due to *user* commands, any Job may be cancelled anytime, which is noted with the onJobCancelledByUser function. 
//When this is called, the Job is already removed from the JobDoers knowledge, the Job then can ... restore the task if wished, or do nothing, and has to delete its reference to the JobDoer. 
//The other direction is also possible, when the system somehow cancels the job, because its target became invalid or any other thing occured that makes it from now on impossible to 
//do this job (ores disappear, monsters interact, the user assigned the task already, ...). This call comes somewhere from world and Pickups, and we have to cancel the execution in the
//JobDoer and simply delete all references we hold any anybody holds to us. 

//Abstract
class Job {
    public :
        Job() {}; //You should not use this tho except for default initializations
        Job(std::shared_ptr<World> w); 

        //!The worlds calls this on us before with a JobDoer before assigning the task to us. 
        virtual bool canBeExecutedBy(std::shared_ptr<JobDoer> jd) = 0;

        //!Called by the JobDoer who was assigned to this task. CanBeExecutedBy was called before for him, so the Doer can actually perform this Job. 
        void onJobStarted(std::shared_ptr<JobDoer> jd);

        //!Called by the JobDoer once he has finished the previous task we gave him. 
        //Is ALSO called by the system when the Job is started. 
        //When the last step is executed, first make sure that the target of the job knows that the job is finished, and by THIS job, so that it deletes
        //its reference to this Job. Then call onJobFinished on the JobDoer. After losing the referencing from the target and the JobDoer, 
        //this Job should get deleted automatically. (Every JobDoer does the deletion-call deleteLastReference() on us when were finished)
        //CallNumber is an internal counter, starting from 0 after onJobStarted, that you may use ... or just ignore. 
        virtual void onActionFinished(int callNumber) = 0; 
        //!See onActionFinished(int callNumber). This is actually called, before we add the counter. 
        void onActionFinished();
        //!This Job was cancelled (and the JobDoer already erased us etc.) but the Job probably still needs to be done, and the target still thinks that this
        //Job is on it! Decide if the Job should still be done, and then do the right preparations! Delete this job by removing the reference in the target and
        //call deleteLastReference(). 
        virtual void onCancelledByUser() = 0;
        //!This Job has become invalid, for whatever reason. The world or somewhere from that side told us. 
        //Our target, whatever it is, has already dropped all references to us. Now lets free all of our references and stop a worker if hes on us already. 
        //If there is no worker with us, then we are still in a joblist, that we leave with this call. 
        void cancelBySystem();

        //!Last step to finally delete this Job, since we are ourself holding the last reference to ourselves. 
        //Deletion happens in three cases: 
        //* Successfully finished, then the JobDoer does this call. 
        //* Cancelled by JobDoer (user mostly): must be done in onCancelledByUser()
        //* Cancelled by System: We are owned by a JobDoer or by the joblist, cancelBySystem() ends this and calls deleteLastReference
        void deleteLastReference();

        //!Use this to get your piece of a shared_ptr to this Job. Kinda the initial *this* reference
        std::shared_ptr<Job> ref();
    
    protected :
        std::shared_ptr<World> world;
        //!If this doesnt point anywhere, we are not executed AND in the joblist of the world. On execution we are not in there. 
        std::shared_ptr<JobDoer> doer; 

        std::shared_ptr<Job> reference = std::shared_ptr<Job>(this); 

    private : 
        int callNumber;
};

#endif