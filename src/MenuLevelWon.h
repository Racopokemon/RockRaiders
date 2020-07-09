#ifndef MENU_LEVEL_WON_H
#define MENU_LEVEL_WON_H

#include <iostream>
#include "Menu.h"
#include "main.h"

class MenuLevelWon : public Menu {
    public : 
        MenuLevelWon(ButtonContext context) : Menu(context) {};
        virtual ~MenuLevelWon() {}

        virtual void init() {

            addButtonAtTop(Button(continuePlaying, "Continue playing this level", ICON_START));

            addButtonAtTop(Button(backToMainMenu, "Back to main menu", ICON_LEAVE));
        }
        
        static void continuePlaying(ButtonContext context) {
            resetMessage();
        }

        static void backToMainMenu(ButtonContext context) {
            resetMessage();
            exitLevel();
        }

};

#endif