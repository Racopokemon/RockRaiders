#include "TestEntity.h"
#include <SFML/Graphics.hpp>
#define STEP_SIZE 1 

bool TestEntity::update() {
    lastX = currentX;
    lastY = currentY;
    currentX = baseX;
    currentY = baseY;
    if (++step >= 4) {
        step = 0;
    }
    switch (step) {
    case 0:
        currentY -= STEP_SIZE;
        break;
    case 1:
        currentX += STEP_SIZE;
        break;
    case 2:
        currentY += STEP_SIZE;
        break; 
    default:
        currentX -= STEP_SIZE;
        break;
    }
    return false;
}
void TestEntity::draw (sf::RenderTarget &target, float delta) {
    sf::CircleShape circle(0.5f);
    float invDelta = 1.f-delta;
    circle.setFillColor(color);
    circle.setPosition(sf::Vector2f(currentX*delta + lastX*invDelta, currentY*delta + lastY*invDelta));
    target.draw(circle);
}

TestEntity::TestEntity(float x, float y, sf::Color col) : Entity() {
    baseX = x;
    baseY = y;
    this->color = col; 
    update();
}