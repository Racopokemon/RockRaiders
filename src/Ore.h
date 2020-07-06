#ifndef ORE_H
#define ORE_H

#include "Pickup.h"
#include <SFML/Graphics.hpp>
#include "TextureLoader.h"

class Ore : public Pickup {

    public :

        Ore(std::shared_ptr<World> w, sf::Vector2f pos, bool startsAsCarried) : Pickup(w, pos, startsAsCarried) {
            texture = TextureLoader::getTextureByName("img/ore.png");
            texture->setSmooth(true);
        };

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states, float delta, float invDelta, bool debug) {
            sf::Sprite s(*texture);
            sf::Vector2u size = texture->getSize();
            s.setOrigin(0.5f*sf::Vector2f(size.x, size.y));
            float scale = 1.f/size.x * 0.6f;
            s.setScale(sf::Vector2f(scale, scale));
            target.draw(s, states);
        }

    private :
        sf::Texture * texture;
};

#endif