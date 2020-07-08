#ifndef MENU_LEVEL_SELECT_H
#define MENU_LEVEL_SELECT_H

#include <iostream>
#include "Menu.h"
#include "main.h"

class MenuLevelSelect : public Menu {
    public : 
        MenuLevelSelect(ButtonContext context) : Menu(context) {};
        virtual ~MenuLevelSelect() {std::cout << "MenuLevelSelect deleted" << std::endl;}

        virtual void init() {

            addButtonAtTop(Button(loadLevel1, "Play the first level", ICON_1));
            addButtonAtTop(Button(loadLevel2, "Play the second level", ICON_2));
            addButtonAtTop(Button(loadLevel3, "Play the third level", ICON_3));

            addButtonAtBottom(Button(exitGame, "Exit the game", ICON_X));
        }
        
        static void exitGame(ButtonContext context) {
            exit(0); //ez.
        }

        static void loadLevel1(ButtonContext context) {
            loadLevel("maps/level1.txt");
        }
        static void loadLevel2(ButtonContext context) {
            loadLevel("maps/drillMap.txt");
        }
        static void loadLevel3(ButtonContext context) {
            loadLevel("maps/defaultMap.txt");
        }
        
};

#endif