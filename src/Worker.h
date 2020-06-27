#ifndef WORKER_H
#define WORKER_H

#include "JobDoer.h"
#include <SFML/Graphics.hpp>
#include "World.h"

#define ANIMATION_DRILL "drill"

class Worker : public JobDoer {
    public : 
        Worker(std::shared_ptr<World> w, sf::Vector2f spawnPos);
        virtual void initPath();
        virtual bool followPath();
        virtual void initAnimation();
        virtual bool updateAnimation();
        virtual void updatePickupAnimation();
        virtual void initIdle();
        virtual void idleAnimation();

        void draw(sf::RenderTarget &target, sf::RenderStates states, float delta, float invDelta, bool debug);
    protected :
        //Index of the tile we are currently heading at
        //Once this int is the size (yes the size, not size-1) of the path vector, 
        //we fine-tune the position on the target tile
        int walkingTileIndex;
        bool readyForNextTileInPath();

        int animationFrame;
        int animationLength;

        bool animationPlaying = false; 
};

#endif