#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "EuclideanGraph.hpp"
#include <vector>
#include <stack>
#include <queue>

struct EuclideanGraph::Graph {
    struct Vertex;
    struct Edge;

    const EuclideanGraph& owner;

    Graph(const EuclideanGraph& own) : owner(own) {}

    Vec2D getPoint(size_t idx) const {
        return owner.points.at(idx);
    }

    struct Vertex {
        const Graph& owner;
        size_t index;
        std::vector<Edge> edges;

        Vertex(const EuclideanGraph& own, size_t idx) : owner(own), index(idx) {}
    };

    struct Edge {
        const Graph& owner;
        const Vertex& from;
        const Vertex& to;
        double weight;

        Edge(const EuclideanGraph& own, const Vertex& f, const Vertex& t) : owner(own), from(f), to(t) {
            Vec2D from_vec = owner.getPoint(from.index);
            Vec2D to_vec = owner.getPoint(to.index);
            weight = distance(from_vec, to_vec);
        }
    };

};

#include "Graph.impl"

#endif
