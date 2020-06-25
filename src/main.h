#ifndef MAIN_H
#define MAIN_H

#include "Entity.h"
//!The internal entities-vector contains everything that needs to be rendered or updated. 
const std::vector<std::shared_ptr<Entity>> getEntities();

//!This should even work while rendering (but you don't update here!!) or within an update-stack
void addEntity(std::shared_ptr<Entity> entity);

#endif