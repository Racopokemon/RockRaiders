#include "TextureLoader.h"

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
