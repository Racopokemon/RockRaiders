#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include <iostream>
#include "Menu.h"
#include "main.h"
#include "MenuLevelStart.h"

//!The base menu shown ingame if there is no other menu
class MenuMain : public Menu {
    public : 
        MenuMain(ButtonContext context) : Menu(context) {};
        virtual ~MenuMain() {}

        virtual void init() {
            addButtonAtBottom(Button(quitToMainMenu, "Quit to main menu", ICON_LEAVE));
            addButtonAtBottom(Button(showTaskAgain, "See the task description again", ICON_INFO));
        }
        static void quitToMainMenu(ButtonContext context) {
            exitLevel();
        }
        static void showTaskAgain(ButtonContext context) {
            //This is a bit hacky, requires the global knowledge which menu to show with which text. 
            //But turns out, when we request the world to do it, what if is paused and doesnt receive updates? 
            setMessage<MenuLevelStart>(context.world->getMapData().startText);
        }
        
};

#endif