#include "Worker.h"
#include "World.h"

Worker::Worker(std::shared_ptr<World> w, sf::Vector2f spawnPos) : JobDoer(w) {
    setPosition(spawnPos);
};

void Worker::initPath() {
    if (path.empty()) {
        throw std::runtime_error("Holy moly how can the path be empty, we even checked before in the JobDoer!!");
    }
    if (path.size() == 1) {
        //Only Fine-Tuning
        walkingTileIndex = 1;
    } else {
        walkingTileIndex = 0;
    }
}

bool Worker::followPath() {
    if (isJobCancelled()) {
        return true;
    }
    if (walkingTileIndex >= path.size()-1) {
        //Approaching target tile, fine-tuning starts once we enter the tile
        if (walkingTileIndex == path.size()-1) {
            if (getTile() == path[walkingTileIndex]) {
                walkingTileIndex++;
            }
        }
    } else {
        if (readyForNextTileInPath()) {
            walkingTileIndex++;
        }
    }
    float moveSpeed = (float)(world->getMap()->getMovementSpeed(getTile()));
    moveSpeed = moveSpeed * 0.001f * 0.2f;
    if ((int)path.size() <= walkingTileIndex) {
        //Fine-tune the position on the destination tile
        return moveForwardTo(pathEnd, moveSpeed);
    } else {
        sf::Vector2f target = getTileCenter(path[walkingTileIndex]);
        pointAt(target);
        moveForward(moveSpeed); 
    }
    return false;
}

bool Worker::readyForNextTileInPath() {
    return insideInnerTile(path[walkingTileIndex], 0.15f);
}

void Worker::initAnimation() {
    animationFrame = 0;
    if (animationName == ANIMATION_DRILL) {
        animationLength = *((int *)animationData);
        delete (int *)animationData;
    } else {
        animationLength = 10; //For now all other animations last simply 10 frames. 
    }
    
}
bool Worker::updateAnimation() {
    bool finishedAfterThisUpdate = ++animationFrame >= animationLength;
    animationPlaying = true; //This is not done in initAnimation, because we might already know that at the end of a tick the animation will begin the tick after

    if (isJobCancelled() && animationName == ANIMATION_DRILL) {
        finishedAfterThisUpdate = true; 
    }
    return finishedAfterThisUpdate;
}
void Worker::updatePickupAnimation() {
    pickup->updateWhileCarried(getPosition(), true); //Thinking about it, this is an issue. 
    //With a more consistent design (in that jumps actually happen) we needed to know our 
    //previous positions and *from where* we are interpolating etc etc. 
    //Nonono lets just keep this true, jumps into vecicles etc with no interpolation are
    //anyway never going to happen in this project, and should in addition also only be
    //executed without a pickup in hand. 
}
void Worker::initIdle() {};
void Worker::idleAnimation() {};

void Worker::onBeginOfTick() {
    animationPlaying = false;
}

void Worker::draw(sf::RenderTarget &target, sf::RenderStates states, float delta, float invDelta, bool debug) {
    float thiccness = 0.22f;
    sf::CircleShape circle(thiccness, 6);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(0.04f);
    circle.setPosition(-thiccness, -thiccness);
    target.draw(circle, states);

    if (animationPlaying) {
        float barWidth = 0.8f, barHeight = 0.1f;

        float interpolation = (animationFrame+delta)/animationLength;
        interpolation*=barWidth;

        sf::RectangleShape barProgress(sf::Vector2f(interpolation, barHeight));
        barProgress.setPosition(barWidth*-0.5f, thiccness + 0.1f);
        barProgress.setFillColor(sf::Color::Black);
        target.draw(barProgress, states);

        sf::RectangleShape barOutline(sf::Vector2f(barWidth, barHeight));
        barOutline.setPosition(barWidth*-0.5f, thiccness + 0.1f);
        barOutline.setFillColor(sf::Color::Transparent);
        barOutline.setOutlineThickness(0.01f);
        barOutline.setOutlineColor(sf::Color::Black);
        target.draw(barOutline, states);
    }

    if (debug) {
        sf::RectangleShape rect(sf::Vector2f(1.f, 1.f));
        sf::Vector2i t = getTile();
        rect.setPosition(sf::Vector2f(t.x, t.y));
        rect.setOutlineColor(sf::Color::Blue);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineThickness(0.03f);
        target.draw(rect);

        float cs = 0.05f;
        sf::CircleShape center(cs);
        center.setFillColor(sf::Color::Black);
        center.setPosition(getPosition() - sf::Vector2f(cs,cs));
        target.draw(center);

        if (path.size() > walkingTileIndex) {
            rect = sf::RectangleShape(sf::Vector2f(0.15f, 0.15));
            sf::Vector2i t = path[walkingTileIndex];
            rect.setPosition(sf::Vector2f(t.x+0.425f, t.y+0.425f));
            rect.setOutlineColor(sf::Color::Red);
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineThickness(0.03f);
            target.draw(rect);
        }
    }
}