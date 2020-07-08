#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>

#include <SFML/Graphics.hpp>

//!Base class for everything that part of the game (not the menus etc): Entities can be updated and rendered, always interpolated by delta between two frames
//Because of C++ being primitive and ugly workarounds seem to be unavoidable at all, they also come with a functionality to get a shared pointer
//to them (like a this. reference), including routines to request deletion. 
class Entity {
    public :
        Entity();
        virtual ~Entity() {std::cout << "Entity deleted." << std::endl;}

        //!Always call super.update() first!
        virtual void update() = 0;

        //!Returning true means that we want to be deleted! 
        bool update_impl();
        virtual void draw(sf::RenderTarget &target, float delta, bool debug) = 0;

        //Call this to delete the entity. It will call the internal prepareDeletion inheritance tree, and after that trigger the deletion with the next update. 
        void requestDeletion();
        
        //Use the dynamic pointer cast to get the actual pointer. 
        std::shared_ptr<Entity> ref();
    protected :

        virtual void prepareDeletion();

        bool getDeleteRequested();
    
    private :
        bool deleteRequested = false; 
        std::shared_ptr<Entity> reference;
};

#endif