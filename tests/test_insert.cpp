#include "../EuclideanGraph.hpp"
#include <iostream>

int main(int argc, char** argv) {
    EuclideanGraph euclid_graph;

    euclid_graph.insert(Vec2D(0, 0), 0.1);
    euclid_graph.insert(Vec2D(-1, 0), 10);
    euclid_graph.insert(Vec2D(1, 0), 10);
    euclid_graph.insert(Vec2D(-0.5, -1), 1.2);
    euclid_graph.insert(Vec2D(-0.5, 1), 1.2);
    euclid_graph.insert(Vec2D(0.5, -1), 1.2);
    euclid_graph.insert(Vec2D(0.5, 1), 2);

    std::cout << "Pause here and check structure.\n";

    euclid_graph.insert(Vec2D(-0.1, 0.1), 20);
}
