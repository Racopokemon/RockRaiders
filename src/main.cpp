#include <iostream>

//#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <SFML/System.hpp>
#include "MapLoader.h"
#include "TestEntity.h"

std::vector<std::shared_ptr<Entity>> entities;
GameMap * map;

bool isHalt, shouldHalt;
sf::Time currentFrameLength, nextFrameLength, timePassedSinceFrameStart;
sf::Time zeroTime = sf::seconds(0);
sf::Time oneSecondTime = sf::seconds(1);

sf::Time timePassedSinceLastSecond;
unsigned int lastFPS, liveFPSCounter;

sf::RenderWindow * gameWindow;

//!If you want to halt, the TPS don't matter, if you don't want to halt, TPS are the requested ticks per second
void updateGameSpeed(bool halt, float TPS) {
    if (halt) {
        shouldHalt = true;
    } else {
        shouldHalt = false;
        nextFrameLength = sf::seconds(1.0f/TPS);
    }
}

void update() {
    unsigned int i = 0;
    while (i < entities.size()) {
        if (entities[i]->update()) {
            //Delete this entity! 
            entities.erase(entities.begin()+i);
        } else {
            //keep the entity, just go on
            i++;
        }
    }
}

//!Delta 0: Previous frame 100% interpolated, 1 current frame 100% interpolated
void render(float delta) {
    gameWindow->clear(sf::Color::White);
    for (auto entity : entities) {
        entity->draw(*gameWindow, delta);
    }
    gameWindow->display();
}

int main() {
    std::shared_ptr<GameMap> map;
    try {
        map = loadFromFile("maps/defaultMap.txt");
    } catch (std::exception& e) {
        std::cout << "Loading the file failed. ";
        std::cout << e.what();
        exit(-1);
    }
    entities.push_back(map);

    entities.push_back(std::make_shared<TestEntity>(0.f,0.f,sf::Color::Black));
    entities.push_back(std::make_shared<TestEntity>(-150.f,150.f,sf::Color::Red));
    entities.push_back(std::make_shared<TestEntity>(150.f,200.f,sf::Color::Blue));
    entities.push_back(std::make_shared<TestEntity>(-200.f,-100.f,sf::Color::Yellow));
    entities.push_back(std::make_shared<TestEntity>(250.f,-130.f,sf::Color::Green));
    
    sf::ContextSettings contextSetting;
    contextSetting.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800,600), "RockRaiders", sf::Style::Default, contextSetting);
    gameWindow = &window;

    // The sf::VideoMode class has some interesting static functions to get the desktop resolution, or the list of valid video modes for fullscreen mode. 
    // Don't hesitate to have a look at its documentation. 
    
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
            case sf::Event::Closed:
                window.close();
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
            std::cout << lastFPS << " Frames last second\n";
            liveFPSCounter = 0;
        }
        liveFPSCounter++;
        
        //TODO: Find a font, load a font, and write the FPS and current updates
    }

    return 0;
}




/**
 * Todo: 
 * 
 * --Create a main function and write into the console. Might be harder than I think. 
 * 
 * Lets see a window and handle its events
 * (is there a *fullscreen???*)
 * 
 * create a texture and a sprite and draw both. 
 * Dive into the docs. Can I paint the texture from emptiness? 
 * Measure the time and write some fps into the corner 
 * find out if SFML wants doubles or floats
 * find out about transforms included in the renderer (then I don't have to pass this argument, but how do I check whether I'm in bounds? <-- included? )
 * can I already use sf::transform to shift all my stuff around at the same time? <-- ezezez it returns a "transform" (3x3 mat oh surprise) and we can .transform points, but also 
 *      just multiply all our vectors to it, simple does it
 * 
 * Dive deep into the view stuff [which really is just another transform with other parameters]
 * Undertand how you should be able to do several at once, if they fit for menus etc, if they rescale on window resize, ...
 * Maybe later I'd also like to have some 3D effects like fog in the foreground or water furtehr back. 
 * 
    sf::RenderStates states;
    states.transform = transform;
    window.draw(entity, states);
 * 
 * either way also find out about matrices in sflm
 * create the .h and .cpp file for a Renderable: 
 *      render(transform (?) , double/float timestep, renderer))
 * 
 * https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php#what-is-a-vertex-and-why-are-they-always-in-arrays
 * they LITERALLY generate a map (in a very fast manner) in this data structure. Oof. Okay lets dig right into it and copy this as well. 
 * 
 * Lol and they even contribute a particle system as well. Okay looks like 
 * 
 * 
 * next things to consider: Do we already have anti-alias on? If not <--- yeeah baby turn it ooohn (~Lucio, on E, always)
 * 
 * map (renderable)
 * 
 * later on we also need a Tickable
 * 
 * Make a basic renderable class / interface and render a bouncy thingy with interpolation and timer and everything
 */