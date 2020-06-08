#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Entity.h"
#include "Block.h"
#include <SFML/Graphics.hpp>

//! Manages the entire map, including the rendering, but also responsible for logic, modification, etc. 
class GameMap : public Entity {

    public : 
        virtual bool update();
        virtual void draw (sf::RenderTarget &target, float delta);

        int getWidth();
        int getHeight();

        GameMap(Block ** m, int width, int height, std::string texture);
        ~GameMap();

    protected : 

        void updateRenderData();

        int width, height; 
        int singleTextureSize;
        Block ** map;
        sf::Texture texture; 
        sf::VertexArray renderDataTextures;
        bool renderDataNeedsToBeUpdatedBeforeNextDraw = true; 
        
};

#endif