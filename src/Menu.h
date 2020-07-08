#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "Button.h"
#include <iostream>
#include "IconLookup.h" //Just for convenience for all implementing menus

#define MENU_SPACE 5.f
#define MENU_WIDTH (BUTTON_SIZE+MENU_SPACE*2)

class Menu {
    public :
        Menu() {}
        //!After calling a constructor, you also need to call init() ... because abstract classes and stuff etc. 
        //Luckily we create menus only at a single place in the main.cpp, so this is ok for this case
        Menu(ButtonContext context);
        virtual ~Menu() {std::cout << "menu deleted." << std::endl;}

        //!Called once the constructor of this menu has returned (happens in main.cpp)
        void afterCreated(float height);

        //!Adds this button into the top container, buttons are added from top to bottom. 
        void addButtonAtTop(Button b);

        //!Adds this button into the bottom container, buttons are added from bottom to top. 
        void addButtonAtBottom(Button b); 

        //!Recalculates the layout of the buttons. 
        void updateHeight(float height);

        //!Always followed by a draw() call, except we release the mouse at this guiTick and another menu is opened
        void guiTick(sf::Vector2f mousePosition, bool mouseDown);

        void draw(sf::RenderTarget & target);
    protected :
        float height;
        sf::Clock creationTime;
        ButtonContext context;
        //!All buttons in the top container, from top to bottom.
        std::vector<Button> buttonsTop;
        //!All buttons in the bottom container, from bottom to top.
        std::vector<Button> buttonsBottom;
        //!All buttons out of both containers above. In the order of the addButtonAtTop and addButtonAdBottom calls. 
        std::vector<Button *> buttons;

        bool lastMouseDown = false;
        Button * activeButton = nullptr;
        ButtonState state = NORMAL;

        //!Use this to call addButtonAtTop and addButtonAdBottom until the menu is constructed. 
        //Enjoy your menu! 
        //Call this manually after you created this menu. (reasons.)
        virtual void init() = 0;
};

#endif