#include "TextureLoader.h"
#include <iostream>

sf::Texture * TextureLoader::getTextureByName(std::string name) {
    static std::map<std::string, std::shared_ptr<sf::Texture>> txts;
    if (txts[name]) {
        return txts[name].get();
    } else {
        std::shared_ptr<sf::Texture> textPointer = std::make_shared<sf::Texture>();
        if (!textPointer->loadFromFile(name)) {
            throw "Could not load texture file " + name; 
        }
        txts[name] = textPointer;
        return textPointer.get();
    }
}
sf::Font mainFont;
bool fontLoaded = false; 
sf::Font * TextureLoader::getFont() {
    if (!fontLoaded) {
        if (!mainFont.loadFromFile(MAIN_FONT_NAME)) {
            std::cout << "Could not load font file. ";
        }
        fontLoaded = true; 
    }
    return &mainFont; 
}
