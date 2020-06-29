#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "Pickup.h"

class Crystal : public Pickup {

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states, float delta, float invDelta, bool debug) {
        float thiccness = 0.1f;
        sf::CircleShape circle(thiccness, 3);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(sf::Color::Green);
        circle.setOutlineThickness(0.04f);
        circle.setPosition(-thiccness, -thiccness);
        target.draw(circle, states);
    }

};


#endif