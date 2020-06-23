#ifndef GRAPH_H
#define GRAPH_H

#include "SFML/Graphics.hpp"

class Node;

struct Edge {
    public :
        Node * target;
        unsigned int weight;

        Edge();
        Edge(Node * n, unsigned int w) {
            target = n;
            weight = w; 
        }
};

class Node {

    public :
        Node() {};
        Node(sf::Vector2u pos) {
            position = pos;
        }
        void addEdge(Edge e) {
            edges.push_back(e);
        }
        std::vector<Edge> getEdges() {
            return edges; 
        }
        sf::Vector2u getPosition() {
            return position; 
        }

    private :
        sf::Vector2u position;
        std::vector<Edge> edges; 
        
};

class Graph {
    public :
        Graph(); 
        Graph(int w, int h); 
        ~Graph();
        //!No bound check is done here. 
        Node * getNode(sf::Vector2u pos);
        void connect(sf::Vector2u a, sf::Vector2u b, unsigned int weight);
        //!Returns a vector of positions that lead from start to target, first element is start, last element is target. 
        //If the vector is empty, no path was found. 
        std::vector<sf::Vector2u> findPathBetween(sf::Vector2u start, sf::Vector2u target, int & length);
    private : 
        int width = 0, height = 0;
        Node * nodes; 
        void init();
};

#endif