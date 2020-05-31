/*! \file MapLoader.h
 * Loads maps from files. This is required, maps don't know to initialize themselves ...
 * Maybe I should have merged this content into the GameMap file, since its so tightly connected. 
 */


#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "GameMap.h"

std::shared_ptr<GameMap> loadFromFile(std::string file); 

#endif