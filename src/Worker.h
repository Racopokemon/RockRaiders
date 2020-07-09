#ifndef WORKER_H
#define WORKER_H

#include "JobDoer.h"
#include <SFML/Graphics.hpp>
#include "World.h"
#include "TextureLoader.h"

#define ANIMATION_DRILL "drill"

#define WORKER_TILES_COUNT 5
#define WORKER_TILE_IDLE 0
#define WORKER_TILE_CARRY 1
#define WORKER_TILE_DRILL 2
#define WORKER_TILE_TOOL 3
#define WORKER_TILE_CARRY_DRILL 4

#define WORKER_TILE_CENTER_FACTOR 1.46f

class Worker : public JobDoer {
    public : 
        Worker(std::shared_ptr<World> w, sf::Vector2f spawnPos);
        virtual ~Worker() {}
        virtual void onBeginOfTick();
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
        bool drilling = false;
        bool drillPlaying = false;

        sf::Texture * texture = nullptr;

};

#endif