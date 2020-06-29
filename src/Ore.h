#ifndef ORE_H
#define ORE_H

#include "Pickup.h"

class Ore : public Pickup {

    public :

        Ore(std::shared_ptr<World> w, sf::Vector2f pos, bool startsAsCarried) : Pickup(w, pos, startsAsCarried) {};

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states, float delta, float invDelta, bool debug) {
            float thiccness = 0.1f;
            sf::CircleShape circle(thiccness, 5);
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineColor(sf::Color::Red);
            circle.setOutlineThickness(0.04f);
            circle.setPosition(-thiccness, -thiccness);
            target.draw(circle, states);
        }

};

#endif