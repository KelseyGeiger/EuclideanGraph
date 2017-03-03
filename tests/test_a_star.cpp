#include "../EuclideanGraph.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    EuclideanGraph euclid_graph;

    euclid_graph.insert(Vec2D(0,0), 10);
    srand(time(NULL));

    size_t n_points = rand()%200 + 50;
    for(size_t i = 0; i < n_points; i++) {
        double x = 50 * (double)(rand() - (RAND_MAX/2))/(double)(RAND_MAX);
        double y = 50 * (double)(rand() - (RAND_MAX/2))/(double)(RAND_MAX);

        euclid_graph.insert(Vec2D(x, y), 15);
    }

    double x = 50 * (double)(rand() - (RAND_MAX/2))/(double)(RAND_MAX);
    double y = 50 * (double)(rand() - (RAND_MAX/2))/(double)(RAND_MAX);

    Vec2D start = Vec2D(x, y);

    x = 50 * (double)(rand() - (RAND_MAX/2))/(double)(RAND_MAX);
    y = 50 * (double)(rand() - (RAND_MAX/2))/(double)(RAND_MAX);

    Vec2D end = Vec2D(x, y);

    std::vector<Vec2D> path = euclid_graph.A_star(start, end);
    std::cout << "Path: {\n";
    while(path.size() > 0) {
        std::cout << "(" << path.back().x << ", " << path.back().y << "), \n";
        path.pop_back();
    }
    std::cout << "}\n";

    return 0;
}
