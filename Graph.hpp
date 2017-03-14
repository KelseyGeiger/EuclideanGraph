#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "EuclideanGraph.hpp"
#include <map>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
#include <limits>

struct EuclideanGraph::Graph {
    struct Vertex;
    struct Edge;

    const EuclideanGraph& owner;
    std::map<size_t, Vertex> vertices;
    std::vector<Edge> edges;

    Graph(const EuclideanGraph& own) : owner(own) {}
    ~Graph() {}

    void insert_vertex(size_t index);
    void insert_edge(size_t from, size_t to);
    void insert(size_t index, double radius);
    void remove(size_t index);

    std::vector<Vec2D> A_star(size_t from, size_t to) const;
    std::vector<Vec2D> A_star_exclusive(size_t from, size_t to, std::vector<size_t> excluded) const;
    std::vector<Vec2D> construct_path(const std::map<size_t, size_t>& pred, size_t current) const;

    Vec2D get_point(size_t idx) const;

    void clear();

    struct Vertex {
        size_t index;
        std::vector<size_t> edge_indices;

        Vertex() : index((size_t)(-1)) {}
        Vertex(size_t idx) : index(idx) {}

        Vertex& operator=(const Vertex& other);

        bool operator==(const Vertex& other);
        bool operator!=(const Vertex& other);
        bool operator<(const Vertex& other);
        bool operator>(const Vertex& other);
        bool operator<=(const Vertex& other);
        bool operator>=(const Vertex& other);
    };

    struct Edge {
        size_t from;
        size_t to;
        double weight;

        Edge(size_t f, size_t t, double w) : from(f), to(t), weight(w) {}

        bool operator==(const Edge& other);
        bool operator!=(const Edge& other);
    };

};

#include "Graph.impl"

#endif
