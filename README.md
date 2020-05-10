# SoSe2020_Padi37 - LRR

This will be a recreation of an unknown Lego video game from 1999 named _Rock Raiders_. It will be a top-down real-time strategy cave mining game, where you command a crew of ai-controlled workers in a grid-shaped cave system to perform tasks like drilling walls, collecting resources and creating buildings. Due to the time limitation I will only realize a heavily boiled-down version, in that I mainly focus on a solid AI.

## Features

### Required

* Singleplayer [**C**]
* Loading a level from file [**B**]
  * 3+ Level should be doable [**C**]
 * Technology
   * Very basic top-down rendering with colored squares.
   * Enabling arbitrary framerates by interpolating motion between two ticks. This is also the foundation to the variable in-game time further below. [Maybe **C**]
* Game
  * Two counters for the two resources
  * A basic side-menu: Blocks in the cave are clickable and then tasks (drilling them at least) can be selected from the menu.
  * Drilled walls drop resources depending if written in the level file.
  * A base station where all resources are brought
  * AI [maybe **B** or **A**]
    * Navigation with Dijkstra (BFS would do it, but there are already different grounds with different speeds planned in the next section) [not that hard, but depends on the constantly changing level structure, maybe **B**]
    * Tasks are assigned when they occur (resources appear etc.), updated and swapped automatically, tasks by the player are scheduled, and the workers execute all of them continuously without further player-interaction.
  * AI-Tasks
    * Drill walls
    * Pick up resources
    * Bring resources to the (closest) base station

### Hopefully makes it as well
* Movable camera (e.g. when mouse at the rim of the screen), zooming in and out
* Influencing workers directly: Workers can be clicked, and can get immediate tasks assigned by the menu or by clicking directly at the part of the map
* Buildings
  * Some kind of buildings can be selected in the menu and placed
  * Then a construction site appears, once all resources are placed here, the building appears
  * AI-Task: Bring resources to a construction site (If necessary, instantly change jobs of workers who already carry resources etc.)
* Ground plates
  * Can be built from the menu for every selected ground square
  * Can be placed in level file
  * Requires workers dropping resources there
  * Builders walk faster on ground plates (navigation includes this in path calculation)
* Rubble
  * Is on every ground square where a wall was drilled
  * Can be placed in level file
  * Can be removed in 5 steps by workers as action when they stand there, spawning resources on the ground there
  * AI-Task: Remove rubble
  * Builders walk slower on ground plates (navigation includes this in path calculation)

### Nice to have

_I probably won’t realize many of those, but if I had infinite time, maybe I would do all of them …_
* Simple tool for more convenient level creation
* Some basic textures for everything.
* Various particle effects
* Camera can be rotated
* Only connected caves are shown, new cave parts are revealed as they are discovered by drilling walls.
* Different in-game time speeds (at least from command line as start argument)
* More menus to load levels, change game-time, …
* Enhanced Navigation, so that there is minimal variance in the workers paths, making it look more natural
* Navigation based on a navigation mesh for quicker /more realistic paths
* Changing / pausing the in-game time somehow in-game.
* Sound