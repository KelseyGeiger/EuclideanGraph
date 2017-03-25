#ifndef EUCLIDEANGRAPH_HPP
#define EUCLIDEANGRAPH_HPP

#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdexcept>

#include "Vec2D.hpp"

class EuclideanGraph {
    public:
        
        EuclideanGraph();
        ~EuclideanGraph();

        Vec2D get_point(size_t index) const;
        void adjust_points(Vec2D offset);
        
        void insert(Vec2D point, double radius);
        void remove(Vec2D point, double epsilon);
        void remove(size_t index);

        size_t nearest_neighbor(Vec2D point) const;
        std::vector<size_t> k_nearest(Vec2D point, size_t k) const;
        std::vector<size_t> radius_search(Vec2D point, double radius) const;
        std::vector<Vec2D> A_star(Vec2D start, Vec2D goal) const;
        std::vector<Vec2D> A_star_exclusive(Vec2D start, Vec2D goal, std::vector<Vec2D> excluded) const;

        void clear();
        size_t size() const;
        
    private:
        
        struct KDTree;
        struct Graph;
        
        std::vector<Vec2D> points;
        KDTree* pointTree;
        Graph* graph;
};

#include "KDTree.hpp"
#include "Graph.hpp"
#include "EuclideanGraph.impl"

#endif
