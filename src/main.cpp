#include "main.h"

#include <iostream>
//#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <SFML/System.hpp>
#include "TestEntity.h"
#include "World.h"
#include "LocatedEntity.h" //Temp actually

//This is the pretty messy main file. Better take a look at the other, way more structured files ;)

std::vector<std::shared_ptr<Entity>> entities;
std::shared_ptr<World> world; 

bool isHalt, shouldHalt;
sf::Time currentFrameLength, nextFrameLength, timePassedSinceFrameStart;
int tps; //Is never used (except for displaying), gets just new values set
sf::Time zeroTime = sf::seconds(0);
sf::Time oneSecondTime = sf::seconds(1);

sf::Time timePassedSinceLastSecond;
unsigned int lastFPS, liveFPSCounter;

//rather temp
int updateTime;
int renderTime;

bool editor = false;
int * editorMetaA;
int * editorMetaB;
int editorWidth, editorHeight;

sf::View gameView; 
sf::View HUDView;  
sf::View menuView; 

sf::Vector2f windowSize(1.f,1.f);

sf::Vector2f camCenter; //Thinking strictly, this is redundant, since the center is also contained in the gameView. However, this is always the right vaule, the gameView then gets updated to fit to this. 
float camZoom = 0.01f; //Considering that with a 1 to 1 zoom every tile was only one pixel big, and this being the factor multiplied to the window size to determine the visible area

sf::RenderWindow * gameWindow;
sf::RenderTarget * target;

bool debugMode = false; 

//sf::Vector2f lastMousePos;
sf::Vector2f mouseDragStart;
sf::Vector2i mouseDragStartOnScreen;
float scrollSinceLastUpdate;
bool lastFirstDown;

float editorAScrollSinceLastUpdate;
float editorBScrollSinceLastUpdate;


#define BUTTON_SPACE 100.f
#define BUTTON_INSET 5.f
#define BUTTON_SIZE BUTTON_SPACE-2*BUTTON_INSET

//!this is closest possible zoom
#define ZOOM_MIN 0.0025f
//!this is the most distanced possible zoom
#define ZOOM_MAX 0.05f

sf::Font mainFont;

void onMapClicked(sf::Vector2f pos) {
    //Here we might first check whether there was an entity that we clicked. 
    sf::Vector2i tilePos = LocatedEntity::toTile(pos);
    if (world->getMap()->inMapBounds(tilePos)) {
        world->onTileClicked(pos);
    }
}

//!If you want to halt, the TPS don't matter, if you don't want to halt, TPS are the requested ticks per second
void updateGameSpeed(bool halt, float TPS) {
    if (halt) {
        shouldHalt = true;
        tps = 0;
    } else {
        shouldHalt = false;
        nextFrameLength = sf::seconds(1.0f/TPS);
        tps = TPS;
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

void initEditor() {
    debugMode = true;
    updateTime = -1;
    updateGameSpeed(true, 60);
    editorWidth = world->getMap()->getWidth();
    editorHeight = world->getMap()->getHeight();
    editorMetaA = new int[editorWidth*editorHeight];
    editorMetaB = new int[editorWidth*editorHeight];
    world->getMap()->getEditorData(editorMetaA, editorMetaB);
}

void renderEditor(sf::RenderTarget &target) {
    int i = 0;
    for (int y = 0; y < editorHeight; y++) {
        for (int x = 0; x < editorWidth; x++) {
            std::string print = std::to_string(editorMetaA[i]) + ", " + std::to_string(editorMetaB[i]);
            sf::Text text;
            text.setFont(mainFont);
            text.setString(print);
            text.setCharacterSize(20.f);
            text.setPosition(sf::Vector2f(x + 0.2f, y+0.2f));
            text.setScale(sf::Vector2f(0.01f, 0.01f));
            text.setFillColor(sf::Color::White);
            target.draw(text);
            i++;
        }
    }
}

void updateEditor() {
    sf::Vector2i local = sf::Mouse::getPosition(*gameWindow);
    sf::Vector2f mouseOnMap = gameWindow->mapPixelToCoords(local, gameView);
    sf::Vector2i tile = LocatedEntity::toTile(mouseOnMap);
    if (world->getMap()->inMapBounds(tile)) {
        int i = tile.x + tile.y*editorWidth;
        if (editorAScrollSinceLastUpdate != 0) {
            editorMetaA[i] += round(editorAScrollSinceLastUpdate);
        }
        if (editorBScrollSinceLastUpdate != 0) {
            editorMetaB[i] += round(editorBScrollSinceLastUpdate);
        }
    }

    editorBScrollSinceLastUpdate = 0;
    editorAScrollSinceLastUpdate = 0;
}

void printEditor() {
    std::cout << ";	Please keep the empty line above, this ends the level data" << std::endl;
    std::cout << ";	Metadata 1 (#Crystals, Rubble amount, Buildung ID, #workers)" << std::endl;
    int i = 0;
    for (int y = 0; y < editorHeight; y++) {
        for (int x = 0; x < editorWidth; x++) {
            std::cout << editorMetaA[i++];
        }
        std::cout << std::endl;
    }
    i = 0;
    std::cout << std::endl;
    std::cout << ";	Please keep the empty line above, this ends the level data" << std::endl;
    std::cout << ";	Metadata 2 (#Ore, visible at start)" << std::endl;
    for (int y = 0; y < editorHeight; y++) {
        for (int x = 0; x < editorWidth; x++) {
            std::cout << editorMetaB[i++];
        }
        std::cout << std::endl;
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
            mouseDragStartOnScreen = local;
        } else {
            camCenter = getCamCenterSoThatTheMouseMapsTo(camZoom, mouseCenteredOnWindow, mouseDragStart);
            updateGameView();
        }
    } else {
        if (lastFirstDown) {
            //Released mouse - if we didn't move the mouse, this was a click! 
            if (mouseDragStartOnScreen == local) {
                onMapClicked(mouseOnMap);
            }
        }
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
    sf::Clock clock;
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
    updateTime = clock.restart().asMicroseconds()/10;
}

void drawText(std::string s, sf::Vector2f pos) {
    sf::Text text;
    text.setFont(mainFont);
    text.setString(s);
    text.setCharacterSize(18);
    text.setPosition(pos);
    text.setFillColor(sf::Color::White);
    target->draw(text);
}

//Screen Coordinates here span from 0 to windowSize.x and 0 to windowSize.y
void renderHUD() {
    if (debugMode) {
        drawText("FPS: " + std::to_string(lastFPS), sf::Vector2f(10.f, 20.f));
        drawText("TPS: " + std::to_string(tps), sf::Vector2f(10.f, 38.f));
        drawText("Update time: " + std::to_string(updateTime), sf::Vector2f(10.f, 56.f));
        drawText("Render time: " + std::to_string(renderTime), sf::Vector2f(10.f, 74.f));
        if (editor) {
            drawText("Welcome to the cheapest editor ever seen, sorry for this. Modify meta with A + Scroll, S + Scroll. Print with enter. ", sf::Vector2f(10.f, 2.f));
        }
    }
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
    sf::Clock clock;
    tick();
    if (editor) {
        updateEditor();
    }

    target->setView(gameView);
    target->clear(sf::Color::Black);
    for (auto entity : entities) {
        entity->draw(*target, delta, debugMode);
    }

    if (editor) {
        renderEditor(*target);
    }

    target->setView(HUDView);
    renderHUD();
    
    target->setView(menuView);
    renderMenu();

    gameWindow->display();
    renderTime = clock.restart().asMicroseconds()/10;
}

const std::vector<std::shared_ptr<Entity>> getEntities() {
    return entities;
}

void addEntity(std::shared_ptr<Entity> entity) {
    entities.push_back(entity);
}

int main() {

    std::cout << "pickup jobs arent deleted and neither are pickups themselves!!" << std::endl;

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
    World * w = new World("maps/drillMap.txt");
    world = w->ref();

    camCenter = world->getMap()->getCameraCenterOnStart();
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
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                        editorAScrollSinceLastUpdate += event.mouseWheelScroll.delta;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                        editorBScrollSinceLastUpdate += event.mouseWheelScroll.delta;
                    } else {
                        scrollSinceLastUpdate += event.mouseWheelScroll.delta;
                    }
                }
                break;
            case sf::Event::KeyPressed:
                if (!editor) {
                    if (event.key.code == sf::Keyboard::Tab) {
                        editor = true; 
                        initEditor();
                    }
                    if (event.key.code == sf::Keyboard::Space) {
                        debugMode = !debugMode;
                    }
                    if (event.key.code == sf::Keyboard::Num1) {
                        updateGameSpeed(true, 60);
                    }
                    if (event.key.code == sf::Keyboard::Num2) {
                        updateGameSpeed(false, 3);
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
                } else {
                    //Cheap editor commmands
                    if (event.key.code == sf::Keyboard::Enter) {
                        printEditor();
                    }
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