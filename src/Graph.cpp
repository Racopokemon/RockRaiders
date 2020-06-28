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

std::vector<sf::Vector2i> Graph::findPathBetween(sf::Vector2i start, std::vector<sf::Vector2i> targets, int & length, int exitConditionLength) {
    std::vector<Node*> nodes;
    for (sf::Vector2i t : targets) {
        nodes.push_back(getNode(t));
    }
    return dijkstra(getNode(start), nodes, width, height, length, exitConditionLength);
}