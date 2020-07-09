#include "GameStatDisplay.h"
#include "TextureLoader.h"
#include "Colors.h"
#include "Menu.h"

#define RIM 28.f
#define GAP 35.f
#define STACK 25.f

#define SCALE 0.7f

GameStatDisplay::GameStatDisplay(std::shared_ptr<World> world, sf::Vector2f * viewportSize) {
    world->getGameStatPointers(&crystals, &ores, &workers);
    size = viewportSize;
    crystalT = TextureLoader::getTextureByName(TEXTURE_NAME_CRYSTAL);
    oreT = TextureLoader::getTextureByName(TEXTURE_NAME_ORE);
    font = TextureLoader::getFont();
    oreT->setSmooth(true);
    crystalT->setSmooth(true);
}
        
void GameStatDisplay::draw(sf::RenderTarget &target) {
    sf::RectangleShape rect(sf::Vector2f(size->x - MENU_WIDTH, RIM + GAP * 2.5f));
    rect.setFillColor(COLORS_TRANS_BACK);
    rect.setPosition(sf::Vector2f(0, size->y - (RIM + GAP * 2.5f))); //top left as 
    target.draw(rect);

    float height = size->y;
    int ore = *ores;
    int crystal = *crystals;
    sf::Text text;
    drawText(target, text, COLORS_STATS_TEXT_PICKUPS, std::to_string(ore), sf::Vector2f(RIM, height - RIM), true);
    drawText(target, text, COLORS_STATS_TEXT_PICKUPS, std::to_string(crystal), sf::Vector2f(RIM, height - RIM - GAP), true);
    drawText(target, text, COLORS_STATS_TEXT_WORKERS, std::to_string(*workers) + " workers", sf::Vector2f(RIM, height - RIM - GAP * 2.f), false);
    if (ore == 0) {
        drawCentered(target, *oreT, COLORS_STATS_NO_PICKUPS, sf::Vector2f(RIM + GAP, height - RIM), SCALE, 1, sf::Vector2f(STACK, 0.f));
    } else {
        drawCentered(target, *oreT, COLORS_STATS_WITH_PICKUPS, sf::Vector2f(RIM + GAP, height - RIM), SCALE, ore, sf::Vector2f(STACK, 0.f));
    }
    if (crystal == 0) {
        drawCentered(target, *crystalT, COLORS_STATS_NO_PICKUPS, sf::Vector2f(RIM + GAP, height - RIM - GAP), SCALE, 1, sf::Vector2f(STACK, 0.f));
    } else {
        drawCentered(target, *crystalT, COLORS_STATS_WITH_PICKUPS, sf::Vector2f(RIM + GAP, height - RIM - GAP), SCALE, crystal, sf::Vector2f(STACK, 0.f));
    }
}

void GameStatDisplay::drawCentered(sf::RenderTarget &target, sf::Texture & t, sf::Color shade, sf::Vector2f center, float scale, int times, sf::Vector2f offset) {
    sf::Sprite s(t);
    s.setScale(sf::Vector2f(scale, scale));
    s.setColor(shade);
    s.setOrigin(0.5f*sf::Vector2f(t.getSize().x, t.getSize().y));
    s.setPosition(center);
    for (int i = 0; i < times; i++) {
        target.draw(s);
        s.setPosition(s.getPosition() + offset);
    }
    
}

void GameStatDisplay::drawText(sf::RenderTarget &target, sf::Text & text, sf::Color c, std::string s, sf::Vector2f positionofCenter, bool completelyCentered) {
    text.setString(s);
    text.setFont(*font);
    text.setCharacterSize(22);
    sf::FloatRect r = text.getLocalBounds();
    if (completelyCentered) {
        text.setOrigin(r.left + r.width*0.5f, r.top + r.height*0.5f);
    } else {
        text.setOrigin(r.left+r.height*0.5f, r.top + r.height*0.5f);
    }
    text.setPosition(positionofCenter);
    text.setFillColor(c);
    target.draw(text);
}


/*
    sf::Text text;
    text.setFont(mainFont);
    text.setString(s);
    text.setCharacterSize(18);
    text.setPosition(pos);
    text.setFillColor(sf::Color::White);
    target->draw(text);
*/