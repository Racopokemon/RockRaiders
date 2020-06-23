//This is just an implementation of Dijkstra. And its truly horrible from every perspective ... but does its job somehow, so better just read something else ..
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "Graph.h"
#include "SFML/Graphics.hpp"

#define HIGHEST_UNSIGNED_INT ~0u

struct DijkstraData {
    //!Keeping a reference to the original node. We need it to later access the node and its edges again.
    //In addition, we keep track whether a node has already been seen (and with this been added to the seen vector).
    //If the reference is still the nullptr, the node is undiscovered yet. 
    Node * referenceToNode = nullptr;
    //Let's do this different, most nodes are never needed anyway
    //bool insideR = false; //!The set R is empty when the algorithm starts and is expanded during the execution. For all nodes in R we already determined the shortest path
    //This should hopefully work and become the biggest possible integer number (close enough to infinity)
    unsigned int l = HIGHEST_UNSIGNED_INT; //!Minimal known distance to this node. 
    sf::Vector2u p; //!Pointer to previous node on shortest path
};

unsigned int getNodeDataIndex(sf::Vector2u v, int width) {
    return v.x+v.y*width;
}

/**
 * Runs Dijkstra on the given grid graph. (We use the grid structure to store data for all the nodes and use the node coordinates, 
 * therefore we need to know width and height of the grid graph)
 * Returns a path starting with start node and ending with target note. If there is no path, we return an empty vector. 
 */
std::vector<sf::Vector2u> dijkstra(Node * start, Node * target, int gridWidth, int gridHeight,int & length) {
    //R is empty
    //Distances are infinity
    std::unique_ptr<DijkstraData[]> data(new DijkstraData[gridWidth*gridHeight]);
    std::vector<unsigned int> seenNodes;
    //Add start.
    int startIndex = getNodeDataIndex(start->getPosition(), gridWidth);
    data[startIndex].l = 0;
    data[startIndex].referenceToNode = start; 
    seenNodes.push_back(startIndex);

    unsigned int targetIndex = getNodeDataIndex(target->getPosition(), gridWidth);

    while(!seenNodes.empty()) {
        //Find [[maybe later: random]] node n with smallest value of l outside R [there was a trick to this tho, to prevent sorting]
        unsigned int minValue = HIGHEST_UNSIGNED_INT;
        unsigned minIndex = HIGHEST_UNSIGNED_INT;
        unsigned int minIndexIndex; //Just another reason why this code is ... cancer
        for (unsigned int i = 0; i < seenNodes.size(); i++) {
            int indexAtI = seenNodes[i];
            if (minValue > data[indexAtI].l) {
                minValue = data[indexAtI].l;
                minIndex = indexAtI;
                minIndexIndex = i;
            }
        }
        if (minIndex == targetIndex) {
            //We found a shortest path to the destination node. 
            //Trace back the path. 
            length = minValue; 
            std::vector<sf::Vector2u> pathBackwards;
            unsigned int currentIndex = targetIndex;
            while (currentIndex != startIndex) {
                pathBackwards.push_back(data[currentIndex].referenceToNode->getPosition());
            }
            pathBackwards.push_back(start->getPosition());
            //Flip the path (we went through it from the back)
            std::vector<sf::Vector2u> pathForward(pathBackwards.size());
            while (!pathBackwards.empty()) {
                pathForward.push_back(pathBackwards.back());
                pathForward.pop_back();
            }
            return pathForward;
        }
        //We found a shortest path to minIndex, but we have to continue. 
        Node * node = data[minIndex].referenceToNode;
        for (Edge e : node->getEdges()) {
            Node * adjacentNode = e.target;
            unsigned int adjacentNodeIndex = getNodeDataIndex(adjacentNode->getPosition(), gridWidth);
            DijkstraData& adjacentNodeData = data[adjacentNodeIndex];
            if (adjacentNodeData.referenceToNode == nullptr) {
                //We discovered a new node. Let's mark it as seen! 
                adjacentNodeData.referenceToNode = adjacentNode;
                seenNodes.push_back(adjacentNodeIndex);
            }
            if (adjacentNodeData.l > e.weight + data[minIndex].l) {
                adjacentNodeData.l = e.weight + data[minIndex].l;
                adjacentNodeData.p = data[minIndex].referenceToNode->getPosition();
            }
        }
        seenNodes.erase(seenNodes.begin()+minIndexIndex); //Sure there isnt any other option to simply ..... remove one element at an index???
    }
    //If we leave the big while loop without discovering any path to the target,
    //than start and target are not connected. In this case we just return the nullptr
    return std::vector<sf::Vector2u>(); 
}

#endif