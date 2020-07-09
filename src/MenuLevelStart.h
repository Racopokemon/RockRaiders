#ifndef MENU_LEVEL_STARTH
#define MENU_LEVEL_STARTH

#include <iostream>
#include "Menu.h"
#include "main.h"

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