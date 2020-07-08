#include "Button.h"
#include "Colors.h"
#include "TextureLoader.h"
#include "IconLookup.h"

Button::Button(void (*clickAction)(ButtonContext context), std::string description, int iconIndex) {
    clickFunction = clickAction;
    this->description = description;
    this->texture = TextureLoader::getTextureByName(TEXTURE_NAME_ICONS);
    texture->setSmooth(true);
    tile = IconLookup::getIconTileFromIndex(iconIndex, texture->getSize().x);
}

void scaleToWidth(sf::Sprite & s, float w) {
    float scale = w / s.getLocalBounds().width;
    s.setScale(sf::Vector2f(scale, scale));
}

void Button::draw(sf::RenderTarget & target, sf::Time time, ButtonState state) {

    sf::RectangleShape b(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
    b.setFillColor(COLORS_BUTTON);
    b.setOrigin(sf::Vector2f(b.getLocalBounds().width*0.5f, b.getLocalBounds().height*0.5f));

    sf::Sprite s(*texture, tile);
    s.setOrigin(sf::Vector2f(s.getLocalBounds().width*0.5f, s.getLocalBounds().height*0.5f));
    scaleToWidth(s, BUTTON_SIZE * 0.8f);

    sf::Transform trans;
    trans.translate(BUTTON_SIZE*0.5f, BUTTON_SIZE*0.5f);
    trans.translate(rect.left, rect.top);

    switch (state)
    {
    case NORMAL:
        trans.scale(sf::Vector2f(0.96f, 0.96f));
        break;
    case PRESSED:
        trans.scale(sf::Vector2f(0.9f, 0.9f));
        break;
    case PRESSED_BUT_LEFT:
        trans.scale(sf::Vector2f(0.93f, 0.93f));
        break;
    case HOVER:
        //trans.scale(sf::Vector2f(0.9f, 0.9f));
        break;
    }

    if (state != NORMAL) {
        //Draw Description to the left, but ignoring (!) the random transform
    }

    if (time < sf::seconds(1)) { 
        //Fade in ... like A. BOSS. !!!!!!!!!!!!!!!!!!
    }


    sf::RenderStates states(trans);

    target.draw(b, states);
    target.draw(s, states);
}

void Button::setPosition(sf::Vector2f pos) {
    rect = sf::FloatRect(pos.x, pos.y, BUTTON_SIZE, BUTTON_SIZE); 
}

bool Button::containsMouse(sf::Vector2f pos) {
    return rect.contains(pos);
}

void Button::click(ButtonContext context) {
    clickFunction(context);
}

void Button::setSpawnIndex(int i) {
    this->spawnIndex = i;
}
