#include "TileJobs.h"

#include "JobDrill.h"

TileJobs::TileJobs(int w, int h) {
    width = w;
    height = h;
    init();
}
void TileJobs::init() {
    tileJobs = new TileJobData[width*height];
}
TileJobs::~TileJobs() {
    delete[width*height] tileJobs;
}

//!Cancels all wall-related jobs at the given position.
void TileJobs::deleteWall(sf::Vector2i w) {
    cancelJobDrillBySystem(w);
    //I doubt were getting more kinds of jobs for such a wall (Detonate, reinforce ...)
}

void TileJobs::setJobDrill(sf::Vector2i w, std::shared_ptr<JobDrill> j) {
    if (get(w).drill) {
        throw std::runtime_error("Tried to store a jobDrill for a tile that already has such a job!");
    }
    get(w).drill = j;
}

void TileJobs::unsetJobDrill(sf::Vector2i w) {
    if (!get(w).drill) {
        throw std::runtime_error("Someone finished a drill job ... that was not even set..");
    }
    get(w).drill.reset();
}

std::shared_ptr<JobDrill> TileJobs::getJobDrill(sf::Vector2i w) {
    return get(w).drill;
}

void TileJobs::cancelJobDrillBySystem(sf::Vector2i w) {
    if (get(w).drill) {
        get(w).drill->cancelBySystem();
        get(w).drill.reset();
    }
}

TileJobData & TileJobs::get(sf::Vector2i pos) {
    return tileJobs[pos.x+pos.y*width];
}

//int width, int height;
//TileJobs* tileJobs;

