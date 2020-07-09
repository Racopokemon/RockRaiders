#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include "World.h"

#define BUTTON_SIZE 80.f
#define BUTTON_TEXT_DISTANCE 20.f
#define BUTTON_TEXT_SHADE_SPACE 6.f

enum ButtonState {
    NORMAL, HOVER, PRESSED, PRESSED_BUT_LEFT
};
//!Lightweight, only ponter(s), can be easily copied with no worries as often as wanted
struct ButtonContext {
    World * world;
    ButtonContext(World * w) {
        world = w;
    }
    ButtonContext() {};
};

class Button {
    public : 
        Button() {}; 
        Button(void (*clickAction)(ButtonContext context), std::string description, int iconIndex);
        ~Button() {};

        void draw(sf::RenderTarget & target, sf::Time time, ButtonState state);

        void setPosition(sf::Vector2f pos);

        bool containsMouse(sf::Vector2f pos);

        void click(ButtonContext context);

        void setSpawnIndex(int i);

    protected :
        int spawnIndex;
        sf::FloatRect rect;
        std::string description;
        sf::IntRect tile;
        sf::Texture * texture; 
        sf::Font * font;
        void (*clickFunction)(ButtonContext context);

};

#endif