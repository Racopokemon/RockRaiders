#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <SFML/Graphics.hpp>

class TextureLoader {
    public : 
        static 
        sf::Texture * getTextureByName(std::string name);
};

#endif