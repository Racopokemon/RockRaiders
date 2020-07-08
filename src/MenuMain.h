#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include "Menu.h"
#include <iostream>
#include "TextureLoader.h"

class MenuMain : public Menu {
    public : 
        //MenuMain() {};
        MenuMain(ButtonContext context) : Menu(context) {};
        virtual ~MenuMain() {std::cout << "MenuMain deleted" << std::endl;}

        virtual void init() {

            Button b(doNothing, "Does... nothing. ", TextureLoader::getTextureByName("img/map.png"));

            addButtonAtTop(b);
            addButtonAtTop(b);
            addButtonAtTop(b);

            addButtonAtBottom(b);
            addButtonAtBottom(b);
        }
        static void doNothing(ButtonContext context) {
            std::cout << "Did ... nothing. (Hey, did you press a button???)" << std::endl;
        }
};

#endif