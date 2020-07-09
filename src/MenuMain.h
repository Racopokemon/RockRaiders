#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include <iostream>
#include "Menu.h"
#include "main.h"

class MenuMain : public Menu {
    public : 
        MenuMain(ButtonContext context) : Menu(context) {};
        virtual ~MenuMain() {}

        virtual void init() {
            addButtonAtBottom(Button(quitToMainMenu, "Quit to main menu", ICON_LEAVE));
        }
        static void quitToMainMenu(ButtonContext context) {
            exitLevel();
        }
        
};

#endif