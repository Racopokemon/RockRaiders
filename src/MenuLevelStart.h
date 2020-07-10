#ifndef MENU_LEVEL_STARTH
#define MENU_LEVEL_STARTH

#include <iostream>
#include "Menu.h"
#include "main.h"

//!The menu that is shown along with the level-start message
class MenuLevelStart : public Menu {
    public : 
        MenuLevelStart(ButtonContext context) : Menu(context) {};
        virtual ~MenuLevelStart() {}

        virtual void init() {
            addButtonAtTop(Button(start, "Start!", ICON_START));
        }
        
        static void start(ButtonContext context) {
            resetMessage();
        }

};

#endif