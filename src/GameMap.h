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

        std::vector<sf::Vector2i> findPathBetween(sf::Vector2i start, sf::Vector2i target, int & length);

        //!-1 means that there doesn't exist a path. Just calls findPathBetween. 
        int getPathLength(sf::Vector2i start, sf::Vector2i target);
        //! True if target can be reached from start. Just calls getPathLength.
        bool connected(sf::Vector2i start, sf::Vector2i target);
        //!see the other connected() function.
        bool connected(sf::Vector2f start, sf::Vector2f target);

        int getMovementSpeed(sf::Vector2i pos);

        //The time is consistent, but it is not per second or anything - but für navigation this is enough. 
        float getMovementTime(sf::Vector2i pos);

        bool isPositionWalkable(sf::Vector2i pos);

        bool isBreakableWall(sf::Vector2i pos);

        bool isGeneralWall(sf::Vector2i pos);

        //!This requires the given position to be a wall, check before with isWall. The hardness then is a constant value for each type of wall. 
        bool getWallStrength(sf::Vector2i pos);

        //!Obvoiusly this is only for quick debugging purposes. 
        sf::Vector2i getRandomPosition();
        
        bool inMapBounds(sf::Vector2i pos);

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