#include "Graph.h"
#include <iostream>
#include "Navigation.h"

//Graph::Graph() {}

Graph::Graph(int w, int h) {
    width = w;
    height = h;
    init();
}

Graph::~Graph() {
    delete[width*height] nodes;
}

void Graph::connect(sf::Vector2i a, sf::Vector2i b, unsigned int weight) {
    Node * nodeA = getNode(a);
    Node * nodeB = getNode(b);
    nodeA->addEdge(Edge(nodeB, weight));
    nodeB->addEdge(Edge(nodeA, weight));
}

//!No bound check is done here. 
Node * Graph::getNode(sf::Vector2i pos) {
    int index = pos.x+pos.y*width;
    return &(nodes[index]);
}

void Graph::init() {
    nodes = new Node[width*height];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            nodes[x+y*width] = Node(sf::Vector2i(x, y));
        }
    }
}

/**
 *!Returns a shortest path from start to target position. 
 */
std::vector<sf::Vector2i> Graph::findPathBetween(sf::Vector2i start, sf::Vector2i target, int & length) {
    return dijkstra(getNode(start), getNode(target), width, height, length);
}