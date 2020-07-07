#ifndef GAME_STAT_DISPLAY_H
#define GAME_STAT_DISPLAY_H

#include <SFML/Graphics.hpp>
#include "World.h"

class GameStatDisplay {
    public : 
        GameStatDisplay() {};
        GameStatDisplay(std::shared_ptr<World> world, sf::Vector2f * viewportSize); 
        
        void draw(sf::RenderTarget &target);
    protected : 
        int * crystals; 
        int * ores; 
        int * workers; 
        sf::Vector2f * size;
        sf::Texture * crystalT;
        sf::Texture * oreT;
        sf::Font * font;

        void drawText(sf::RenderTarget &target, sf::Text & text, sf::Color c, std::string s, sf::Vector2f positionofCenter, bool completelyCentered);
        void drawCentered(sf::RenderTarget &target, sf::Texture & t, sf::Color shade, sf::Vector2f center, float scale, int times, sf::Vector2f offset = sf::Vector2f(1.f,1.f));
};

#endif