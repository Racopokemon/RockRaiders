#include "Entity.h"
#include <iostream>
#include "Pickup.h" //TEMPTileJobsETC
#include <typeinfo> //

bool Entity::update_impl() {
    update();
    if (deleteRequested) {
        reference.reset();
        return true;
    }
    return false; 
} 

void Entity::requestDeletion() {
    deleteRequested = true; 
    prepareDeletion();
}

std::shared_ptr<Entity> Entity::ref() {
    if (!reference) {
        if (deleteRequested) {
            throw new std::runtime_error("After an Entity requested to delete itself, there was still a request for a new reference. Has one never before created, or is someone still working with this?");
        }
        reference = std::shared_ptr<Entity>(this);
    }
    if (dynamic_cast<Pickup*>(this) != nullptr) { //holy fuck googled 30 minutes to find a workaround for instanceof. c++, you are SOO ugly. 
        std::cout << "Pickup referenced. " << reference.use_count() << std::endl;
    }
    return reference;
}

