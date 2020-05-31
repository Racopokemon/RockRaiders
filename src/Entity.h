#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
    public :
        //!Returning true means that we want to be deleted! 
        virtual bool update() = 0;
        virtual void draw (sf::RenderTarget &target, float delta) = 0; 
        //used to be const, but the map e.g. only updates its render data if there is actually a frame coming. 
};

#endif