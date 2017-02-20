#ifndef EUCLIDEANGRAPH_HPP
#define EUCLIDEANGRAPH_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <stdexcept>

#include "Vec2D.hpp"

bool cmp_x(const Vec2D& a, const Vec2D& b);
bool cmp_y(const Vec2D& a, const Vec2D& b);

void sortRangeByComponent(std::vector<Vec2D>& input, size_t component, size_t start, size_t end);
void sortByComponent(std::vector<Vec2D>& input, size_t component);

class EuclideanGraph {
    public:
        
        EuclideanGraph();
        ~EuclideanGraph();
        
        void insert(Vec2D point, double radius);

        size_t nearest_neighbor(Vec2D point) const ;
        std::vector<size_t> radius_search(Vec2D point, double radius) const;

        void clear();
        
    private:
        
        struct KDTree;
        struct Graph;
        
        std::vector<Vec2D> points;\
        KDTree* pointTree;
        Graph* graph;
};

#include "KDTree.hpp"
#include "Graph.hpp"

#endif
