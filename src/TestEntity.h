#ifndef TESTENTITY_H
#define TESTENTITY_H
#include "Entity.h"

class TestEntity : public Entity {
    public :
        virtual void update();
        virtual void draw (sf::RenderTarget &target, float delta); 
        TestEntity(float x, float y, sf::Color col);
    protected :
        float baseX, baseY,
            currentX, currentY,
            lastX, lastY;
        int step = 0;
        sf::Color color;
};

#endif