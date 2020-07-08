#include "Entity.h"
#include <iostream>
#include "Pickup.h" //TEMPTileJobsETC
#include <typeinfo> //

Entity::Entity() {
    reference = std::shared_ptr<Entity>(this); 
}

bool Entity::update_impl() {
    /*
    if (deleteRequested) {
        return true;
    }*/
    update();
    if (deleteRequested) {
        //reference.reset();
        return true;
    }
    return false; 
} 

void Entity::requestDeletion() {
    deleteRequested = true; 
    prepareDeletion();
}

void Entity::prepareDeletion() {
    reference.reset();
}

bool Entity::getDeleteRequested() {
    return deleteRequested;
}

std::shared_ptr<Entity> Entity::ref() {
    if (!reference || deleteRequested) {
        throw new std::runtime_error("After an Entity requested to delete itself, there was still a request for a new reference. Has one never before created, or is someone still working with this?");
    }
    return reference;
}

