#ifndef MAIN_H
#define MAIN_H

#include "Entity.h"
//!The internal entities-vector contains everything that needs to be rendered or updated. 
const std::vector<std::shared_ptr<Entity>> getEntities();

//!This should even work while rendering (but you don't update here!!) or within an update-stack
void addEntity(std::shared_ptr<Entity> entity);

//!This instantly removes the old menu. Expected to be called from a menu button. 
template<class T>
void setMenu();

void resetMenu();

//!parameter level means the relative level path. Expected to be called from a menu button. 
void loadLevel(std::string level);
//Brings you instantly into the main menu. Expected to be called from a menu button. 
void exitLevel();

#endif