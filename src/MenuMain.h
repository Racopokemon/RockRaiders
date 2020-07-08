#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include <iostream>
#include "Menu.h"
#include "main.h"

class MenuMain : public Menu {
    public : 
        MenuMain(ButtonContext context) : Menu(context) {};
        virtual ~MenuMain() {std::cout << "MenuMain deleted" << std::endl;}

        virtual void init() {

            Button b(doNothing, "Does... nothing. ", ICON_1);

            addButtonAtTop(b);
            addButtonAtTop(b);
            addButtonAtTop(b);

            addButtonAtBottom(b);
            addButtonAtBottom(Button(end, "im sooooo scared :(", ICON_X));
        }
        static void doNothing(ButtonContext context) {
            std::cout << "Did ... nothing. (Hey, did you press a button???)" << std::endl;
        }
        static void end(ButtonContext context) {
            exitLevel();
        }
        
};

#endif