#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <SFML/Graphics.hpp>

#define TEXTURE_NAME_CRYSTAL "img/crystal.png"
#define TEXTURE_NAME_ORE "img/ore.png"
#define TEXTURE_NAME_WINDOW_ICON "img/window.png"
#define TEXTURE_NAME_ICONS "img/icons.png"
#define TEXTURE_NAME_SPLASH "img/splash.png"
#define MAIN_FONT_NAME "fonts/redline.ttf"

class TextureLoader {
    public : 
        static 
        sf::Texture * getTextureByName(std::string name);

        static sf::Font * getFont();
};

#endif