#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Entity.h"
#include "Block.h"
#include "Graph.h"
#include <SFML/Graphics.hpp>
#include "BlockPropertiesLookup.h"

//! Manages the entire map, including the rendering, but also responsible for logic, modification, etc. 
class GameMap : public Entity {

    public : 
        virtual void update();
        virtual void draw (sf::RenderTarget &target, float delta, bool debug);

        int getWidth();
        int getHeight();

        GameMap(Block ** m, int width, int height, std::string texture);

        ~GameMap();

        std::vector<sf::Vector2i> findPathBetween(sf::Vector2i start, sf::Vector2i target, int & length, int exitConditionLength = ~0u);
        //!Finds the shortest path to the closest of the targets. 
        std::vector<sf::Vector2i> findPathBetween(sf::Vector2i start, std::vector<sf::Vector2i> targets, int & length, int exitConditionLength = ~0u);
        std::vector<sf::Vector2i> findPathBetween(sf::Vector2i start, sf::Vector2i target, int exitConditionLength = ~0u);
        //!Finds the shortest path to the closest of the targets. 
        std::vector<sf::Vector2i> findPathBetween(sf::Vector2i start, std::vector<sf::Vector2i> targets, int exitConditionLength = ~0u);
        //!-1 means that there doesn't exist a path. Just calls findPathBetween. 
        int getPathLength(sf::Vector2i start, sf::Vector2i target, int exitConditionLength = ~0u);
        //! True if target can be reached from start. Just calls getPathLength.
        bool connected(sf::Vector2i start, sf::Vector2i target);
        //!see the other connected() function.
        bool connected(sf::Vector2f start, sf::Vector2f target);
        //!Finds the shortest path to any of the targets. -1 means that there doesn't exist a path. Just calls findPathBetween. 
        int getPathLength(sf::Vector2i start, std::vector<sf::Vector2i> targets, int exitConditionLength = ~0u);
        //!True, if one of the targets is reachable
        bool connected(sf::Vector2i start, std::vector<sf::Vector2i> targets);
        //!True, if one of the targets is reachable
        bool connected(sf::Vector2f start, std::vector<sf::Vector2f> targets);


        int getMovementSpeed(sf::Vector2i pos);

        //The value that should be used when calculating the path travel lengths for navigation. 
        //For normal blocks (plates, rubble, normal ground) it is calculated by calculating a movement time from the movement speed, but
        //buildings have an (imaginary, they just feel like plates) high traversal time so that workers avoid walking at them ... if they can. 
        float getMovementTimePriority(sf::Vector2i pos);

        bool isPositionWalkable(sf::Vector2i pos);

        bool isBreakableWall(sf::Vector2i pos);

        bool isGeneralWall(sf::Vector2i pos);

        //!This requires the given position to be a wall, check before with isWall. The hardness then is a constant value for each type of wall. 
        int getWallStrength(sf::Vector2i pos);

        //!Obvoiusly this is only for quick debugging purposes. 
        sf::Vector2i getRandomPosition();
        
        bool inMapBounds(sf::Vector2i pos);

        //! We expect that you know that this block actually was some drill-able block. 
        //Places rubble on the ground, primes it with ores.
        void destroyWall(sf::Vector2i pos, int & crystalNumber, int & oreNumber);
        
        //Cheap and dirty function that writes the metadata of all blocks into two given locations (expecting that the space is allocated etc.)
        void getEditorData(int * metaA, int* metaB);

    protected : 

        int width, height; 
        int singleTextureSize;
        Block ** map;
        sf::Texture texture; 
        sf::VertexArray renderDataTextures;
        //!Set to true, this regenerates the vertex data for rendering (redraws the map) on the next draw
        bool viewModified = true; 
        //!Set to true, it causes the graph to be regenerated when it is requested the next time
        bool graphModified = true; 
        std::shared_ptr<Graph> graph; 

        void updateRenderData();
        void setModified();
        std::shared_ptr<Graph> getGraph();
        void generateGraph();

    private : 
        Block & getBlock(sf::Vector2i pos);
        bool inBounds(int x, int y);
        bool isPositionWalkable(int x, int y);
        void connectIfFree(Graph & g, sf::Vector2i start, sf::Vector2i* pos, int listSize, bool onlyAverageFirstAndLast);
};

#endif