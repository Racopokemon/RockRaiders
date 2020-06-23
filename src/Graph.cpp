#include "Graph.h"

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

void Graph::connect(sf::Vector2u a, sf::Vector2u b, unsigned int weight) {
    Node * nodeA = getNode(a);
    Node * nodeB = getNode(b);
    nodeA->addEdge(Edge(nodeA, weight));
    nodeB->addEdge(Edge(nodeB, weight));
}

//!No bound check is done here. 
Node * Graph::getNode(sf::Vector2u pos) {
    return &nodes[pos.x+pos.y*width];
}

void Graph::init() {
    nodes = new Node[width*height];
}

/**
 *!Returns a shortest path from start to target position. 
 */
std::vector<sf::Vector2u> Graph::findPathBetween(sf::Vector2u start, sf::Vector2u target, int & length) {
    return dijkstra(getNode(start), getNode(target), width, height, length);
}