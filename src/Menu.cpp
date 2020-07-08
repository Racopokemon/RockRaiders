#include "Menu.h"

Menu::Menu(ButtonContext context) {
    this->context = context;
    //https://stackoverflow.com/questions/8630160/call-to-pure-virtual-function-from-base-class-constructor
    //init();
}

void Menu::afterCreated(float height) {
    init();
    int index = 0;
    for (Button & b : buttonsTop) {
        b.setSpawnIndex(index++);
        buttons.push_back(&b);
    }
    for (int i = buttonsBottom.size() - 1; i >= 0; i--) {
        buttonsBottom[i].setSpawnIndex(index++);
        buttons.push_back(&buttonsBottom[i]);
    }
    updateHeight(height);
}

void Menu::addButtonAtTop(Button b) {
    buttonsTop.push_back(b);
}

void Menu::addButtonAtBottom(Button b) {
    buttonsBottom.push_back(b);
}

void Menu::updateHeight(float height) {
    this->height = height;
    int i = 0;
    for (Button & b : buttonsTop) {
        b.setPosition(sf::Vector2f(MENU_SPACE, MENU_SPACE + (i++)*(MENU_SPACE+BUTTON_SIZE)));
    }
    i = 1;
    for (Button & b : buttonsBottom) {
        b.setPosition(sf::Vector2f(MENU_SPACE, height - (i++)*(MENU_SPACE+BUTTON_SIZE)));
    }
}

void Menu::guiTick(sf::Vector2f mousePosition, bool mouseDown) {
    bool lMouseDown = lastMouseDown;
    lastMouseDown = mouseDown;
    Button * buttonAtMouse = nullptr;
    for (Button * b : buttons) {
        if (b->containsMouse(mousePosition)) {
            buttonAtMouse = b;
            break; 
        }
    }
    if (mouseDown) {
        if (lMouseDown) {
            if (activeButton != nullptr) {
                if (activeButton == buttonAtMouse) {
                    state = PRESSED;
                } else {
                    state = PRESSED_BUT_LEFT;
                }
            } 
        } else {
            if (activeButton != nullptr) {
                state = PRESSED;
            }
        }
    } else {
        if (lMouseDown) {
            if (activeButton != nullptr) {
                if (activeButton == buttonAtMouse) {
                    activeButton->click(context);
                    //We probably don't exist anymore after this here. 
                } else {
                    activeButton = buttonAtMouse;
                    state = HOVER;
                }
            }
        } else {
            activeButton = buttonAtMouse;
            state = HOVER;
        }
    }
}

void Menu::draw(sf::RenderTarget & target) {
    sf::Time time = creationTime.getElapsedTime();
    for (Button * b : buttons) {
        b->draw(target, time, activeButton == b ? state : NORMAL);
    }
}
        