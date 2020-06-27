#ifndef TILE_JOBS_H
#define TILE_JOBS_H

#include <SFML/Graphics.hpp>
class JobDrill;

struct TileJobData {
    std::shared_ptr<JobDrill> drill;
};

//!Just for some order (and more actually not really necessary inter-class calls)
//we have our own datastructure / class for storing the job pointers to Jobs related 
//to single tiles on the map (drilling, remove rubble, build stuff, etc.)
class TileJobs {
    public :
        TileJobs(int w, int h);
        ~TileJobs();

        //!Cancels all wall-related jobs at the given position.
        void deleteWall(sf::Vector2i w);

        //!Requires that there is no JobDrill yet. To delete, cancel it by system, or if you
        //were the Job to actually drill it, unset it!
        void setJobDrill(sf::Vector2i w, std::shared_ptr<JobDrill> j);
        void cancelJobDrillBySystem(sf::Vector2i w);
        //!Just setting it to null, but without error checking. Should be invoked by the
        //Job with the task to drill, either if the Job was finished or canceled. Does nothing further (no destruction call etc).
        void unsetJobDrill(sf::Vector2i w);
        std::shared_ptr<JobDrill> getJobDrill(sf::Vector2i w);

    protected : 
        int width;
        int height;
        TileJobData* tileJobs;

        void init();
        TileJobData & get(sf::Vector2i pos);

};

#endif