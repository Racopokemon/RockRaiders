#include "main.h"

#include <iostream>
//#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <SFML/System.hpp>
#include "TestEntity.h"
#include "World.h"
#include "LocatedEntity.h" //Temp actually

std::vector<std::shared_ptr<Entity>> entities;
std::shared_ptr<World> world; 

bool isHalt, shouldHalt;
sf::Time currentFrameLength, nextFrameLength, timePassedSinceFrameStart;
sf::Time zeroTime = sf::seconds(0);
sf::Time oneSecondTime = sf::seconds(1);

sf::Time timePassedSinceLastSecond;
unsigned int lastFPS, liveFPSCounter;

sf::View gameView; 
sf::View HUDView;  
sf::View menuView; 

sf::Vector2f windowSize(1.f,1.f);

sf::Vector2f camCenter; //Thinking strictly, this is redundant, since the center is also contained in the gameView. However, this is always the right vaule, the gameView then gets updated to fit to this. 
float camZoom = 0.01f; //Considering that with a 1 to 1 zoom every tile was only one pixel big, and this being the factor multiplied to the window size to determine the visible area

sf::RenderWindow * gameWindow;
sf::RenderTarget * target;

//sf::Vector2f lastMousePos;
sf::Vector2f mouseDragStart;
float scrollSinceLastUpdate;
bool lastFirstDown;

#define BUTTON_SPACE 100.f
#define BUTTON_INSET 5.f
#define BUTTON_SIZE BUTTON_SPACE-2*BUTTON_INSET

//this is closest possible zoom
#define ZOOM_MIN 0.0025f
//this is the most distanced possible zoom
#define ZOOM_MAX 0.5f

sf::Font mainFont;

//!If you want to halt, the TPS don't matter, if you don't want to halt, TPS are the requested ticks per second
void updateGameSpeed(bool halt, float TPS) {
    if (halt) {
        shouldHalt = true;
    } else {
        shouldHalt = false;
        nextFrameLength = sf::seconds(1.0f/TPS);
    }
}

void updateGameView() {
    gameView.setCenter(camCenter);
    gameView.setSize(camZoom*windowSize);
}

void updateViewport(sf::Vector2f newSize) {
    windowSize = newSize;
    gameView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    updateGameView();

    HUDView = sf::View(sf::FloatRect(0.f, 0.f, newSize.x, newSize.y));
    HUDView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    menuView = sf::View(sf::FloatRect(BUTTON_SPACE-newSize.x, 0.f, newSize.x, newSize.y));
    menuView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
}

sf::Vector2f getCamCenterSoThatTheMouseMapsTo(float camZoom, sf::Vector2f mouseCenteredOnWindow, sf::Vector2f mouseOnMap) {
    return mouseOnMap - mouseCenteredOnWindow*camZoom;
}

//!Caps the camZoom variable if its too small or big
void capZoom() {
    if (camZoom < ZOOM_MIN) {
        camZoom = ZOOM_MIN;
    } else if (camZoom > ZOOM_MAX) {
        camZoom = ZOOM_MAX;
    }
}

//The bad, not deterministic menu handling is here. Totally dependent on the frames, called before every single render. 
void tick() {
    if (!gameWindow->hasFocus()) {
        scrollSinceLastUpdate = 0;
        return; 
    }
    sf::Vector2i local = sf::Mouse::getPosition(*gameWindow);
    sf::Vector2f mouseCenteredOnWindow = sf::Vector2f(local.x+0.f, local.y+0.f) - windowSize*0.5f;
    sf::Vector2f mouseOnMap = gameWindow->mapPixelToCoords(local, gameView);

    bool firstDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    //bool secondDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    float scroll = scrollSinceLastUpdate;
    
    if (firstDown) {
        if (!lastFirstDown) {
            mouseDragStart = mouseOnMap;
        } else {
            camCenter = getCamCenterSoThatTheMouseMapsTo(camZoom, mouseCenteredOnWindow, mouseDragStart);
            updateGameView();
        }
    } else {

    }
 
    if (scrollSinceLastUpdate != 0.0f) {
        camZoom *= powf(0.8f, scrollSinceLastUpdate);
        capZoom();
        camCenter = getCamCenterSoThatTheMouseMapsTo(camZoom, mouseCenteredOnWindow, mouseOnMap);
        updateGameView();
    }


    lastFirstDown = firstDown;
    //lastMousePos = mousePos;
    scrollSinceLastUpdate = 0;
}

void update() {
    unsigned int i = 0;
    while (i < entities.size()) {
        if (entities[i]->update_impl()) {
            //Delete this entity! 
            entities.erase(entities.begin()+i);
        } else {
            //keep the entity, just go on
            i++;
        }
    }
}

//Screen Coordinates here span from 0 to windowSize.x and 0 to windowSize.y
void renderHUD() {
    sf::Text fpsDisplay;
    fpsDisplay.setFont(mainFont);
    fpsDisplay.setString(std::to_string(lastFPS));
    fpsDisplay.setCharacterSize(18);
    fpsDisplay.setPosition(sf::Vector2f(10.f, 2.f));
    fpsDisplay.setFillColor(sf::Color::Black);
    target->draw(fpsDisplay);
}

//Screen coordinates are scaled so that one button has the size of BUTTON_SPACE, and from origin at top x left
void renderMenu() {
    sf::RectangleShape shade(sf::Vector2f(BUTTON_SPACE, windowSize.y));
    shade.setFillColor(sf::Color(0,0,0,150));
    target->draw(shade);
    //Temp of course
    for (int i = 0; i < 4; i++) {
        sf::RectangleShape button(sf::Vector2f(BUTTON_SIZE, BUTTON_SIZE));
        button.setPosition(BUTTON_INSET, BUTTON_INSET + i*BUTTON_SIZE);
        button.setFillColor(sf::Color::White);
        target->draw(button);
    }
}

//!Delta 0: Previous frame 100% interpolated, 1 current frame 100% interpolated
void render(float delta) {
    tick();

    target->setView(gameView);
    target->clear(sf::Color::Black);
    for (auto entity : entities) {
        entity->draw(*target, delta);
    }

    target->setView(HUDView);
    renderHUD();
    
    target->setView(menuView);
    renderMenu();

    gameWindow->display();
}

const std::vector<std::shared_ptr<Entity>> getEntities() {
    return entities;
}

void addEntity(std::shared_ptr<Entity> entity) {
    entities.push_back(entity);
}

int main() {

    sf::Vector2i newSize(800, 600);

    if (!mainFont.loadFromFile("fonts/redline.ttf")) {
        std::cout << "Could not load font file. ";
    }

    sf::ContextSettings contextSetting;
    contextSetting.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(newSize.x,newSize.y), "RockRaiders", sf::Style::Default, contextSetting);
    window.setKeyRepeatEnabled(false);
    gameWindow = &window;
    target = &window; //If we at some point decide to draw into an image instead ... were prepared! 

    updateViewport(sf::Vector2f((float)newSize.x, (float)newSize.y));

    // The sf::VideoMode class has some interesting static functions to get the desktop resolution, or the list of valid video modes for fullscreen mode. 
    World * w = new World("maps/defaultMap.txt");
    world = w->ref();

    camCenter = sf::Vector2f(world->getMap()->getWidth()*0.5f, world->getMap()->getHeight()*0.5f);
    updateGameView();
    
    isHalt = true; //Lets don't do any initialization and instead pretend like the game was paused before ... 
    //so all initialization is done by itself in the next update when it is confronted with continuing the loop
    updateGameSpeed(false, 60);

    timePassedSinceLastSecond = zeroTime;

    sf::Clock clock;
    sf::Clock FPSClock;
    while (window.isOpen()) {
        sf::Event event; 
        while (window.pollEvent(event)) {
            switch (event.type)
            {
            case sf::Event::Resized:
                updateViewport(sf::Vector2f((float)event.size.width, (float)event.size.height));
                break;
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    scrollSinceLastUpdate += event.mouseWheelScroll.delta;
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Num1) {
                    updateGameSpeed(true, 60);
                }
                if (event.key.code == sf::Keyboard::Num2) {
                    updateGameSpeed(false, 4);
                }
                if (event.key.code == sf::Keyboard::Num3) {
                    updateGameSpeed(false, 10);
                }
                if (event.key.code == sf::Keyboard::Num4) {
                    updateGameSpeed(false, 25);
                }
                if (event.key.code == sf::Keyboard::Num5) {
                    updateGameSpeed(false, 60);
                }
                if (event.key.code == sf::Keyboard::Num6) {
                    updateGameSpeed(false, 100);
                }
                if (event.key.code == sf::Keyboard::Num7) {
                    updateGameSpeed(false, 250);
                }
                if (event.key.code == sf::Keyboard::Num8) {
                    updateGameSpeed(false, 500);
                }
                break; 
            }
        }
        
        //Tick-Dependent Render (and Update) step
        if (isHalt) {
            if (!shouldHalt) {
                isHalt = false;
                currentFrameLength = nextFrameLength;
                update();
                render(0.f);
                timePassedSinceFrameStart = zeroTime;
                clock.restart();
            } else {
                render(1.f);
            }
        } else {
            sf::Time timePassed = clock.restart();
            timePassedSinceFrameStart += timePassed;
            if (timePassedSinceFrameStart > currentFrameLength) {
                if (shouldHalt) {
                    isHalt = true;
                    timePassedSinceFrameStart = currentFrameLength;
                } else {
                    while(timePassedSinceFrameStart > currentFrameLength) {
                        update();
                        timePassedSinceFrameStart -= currentFrameLength;
                        currentFrameLength = nextFrameLength;
                    }
                }
            }
            render(timePassedSinceFrameStart/currentFrameLength);
        }

        //Independent FPS Counter step
        timePassedSinceLastSecond += FPSClock.restart(); 
        while (timePassedSinceLastSecond > oneSecondTime) {
            timePassedSinceLastSecond -= oneSecondTime;
            lastFPS = liveFPSCounter;
            liveFPSCounter = 0;
        }
        liveFPSCounter++;
    }

    return 0;
}