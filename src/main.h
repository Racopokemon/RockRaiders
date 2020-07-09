#ifndef MAIN_H
#define MAIN_H

#include "Entity.h"
//!The internal entities-vector contains everything that needs to be rendered or updated. 
const std::vector<std::shared_ptr<Entity>> getEntities();

//!This should even work while rendering (but you don't update here!!) or within an update-stack
void addEntity(std::shared_ptr<Entity> entity, int index = -1);

//!This instantly removes the old menu. Expected to be called from a menu button. 
template<class T>
void setMenu();

//!Removes the current menu, but does nothing further. 
void resetMenu();

//!Call this when you just want to go back to the base menu (level select or ingame main menu). 
//Remember that we always have to chain menus, once no one is shown (at least in level select)
//it might be impossible to ever get to see one. 
void resetMenuWithoutSettingNewMenu();

//!parameter level means the relative level path. Expected to be called from a menu button. 
void loadLevel(std::string level);
//!Brings you instantly into the main menu. Expected to be called from a menu button. 
void exitLevel();

//!Halts the game and displays the string in a message window, while loading menu T to respond (and close) to the message
//While menus can be shown at an arbitrary time, you should not call this while updating entities, anywhere before or after should work. 
//During the update step menus might change which becomes a problem when we want to close the message
template<class T>
void setMessage(std::string s);

//!Continues the game, resets the menu [without setting new one is called]
void resetMessage();

#endif