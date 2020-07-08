#include "TileJobs.h"

#include "JobDrill.h"
#include "JobDeliver.h"

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

void TileJobs::addJobDeliver(sf::Vector2i w, std::shared_ptr<JobDeliver> j) {
    get(w).deliver.push_back(j);
}
void TileJobs::removeJobDeliver(sf::Vector2i w, std::shared_ptr<JobDeliver> j) {
    std::vector<std::shared_ptr<JobDeliver>> & d = get(w).deliver;
    auto f = std::find(d.begin(), d.end(), j);
    if (f == d.end()) {
        std::cout << "WARNING: There was a request to delete a JobDeliver at TileJobs - but this Job was not even registered in the first place! " << std::endl;
    } else {
        d.erase(f);
    }
}
void TileJobs::cancelAllJobDeliverBySystem(sf::Vector2i w) {
    for (auto j : get(w).deliver) {
        j->cancelBySystem();
    }
    get(w).deliver.clear();
}

void TileJobs::cancelAllJobs() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            sf::Vector2i pos (x, y);
            if (getJobDrill(pos)) {
                cancelJobDrillBySystem(pos);
            }
            cancelAllJobDeliverBySystem(pos);
        }
    }
}