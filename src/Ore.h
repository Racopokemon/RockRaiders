#ifndef ORE_H
#define ORE_H

#include "Pickup.h"

class Ore : public Pickup {

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states, float delta, float invDelta, bool debug) {
        float thiccness = 0.1f;
        sf::CircleShape circle(thiccness, 5);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(sf::Color::RED);
        circle.setOutlineThickness(0.04f);
        circle.setPosition(-thiccness, -thiccness);
        target.draw(circle, states);
    }

};

#endif