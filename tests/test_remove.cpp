#include "../EuclideanGraph.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

double rand_range(double min, double max) {
    double t = ((double) rand()) / ((double) RAND_MAX);
    double diff = max - min;
    return min + (t * diff);
}

int main(int argc, char** argv) {
    srand(time(NULL));

    EuclideanGraph graph;

    Vec2D pos;

    for(size_t i = 0; i < 1000; i++) {
        pos.x = rand_range(-50, 50);
        pos.y = rand_range(-50, 50);
        double radius = rand_range(0.1, 5.0);
        graph.insert(pos, radius);
    }

    for(size_t i = 0; i < 50; i++) {
        size_t idx = rand() % graph.size();
        Vec2D point = graph.get_point(i);
        std::cout << "Removing [" << point.x << ", " << point.y << "], index " << idx << "\n";
        graph.remove(point, 0.001);
    }

    return 0;
}
