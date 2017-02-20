#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "EuclideanGraph.hpp"
#include <map>
#include <vector>
#include <stack>
#include <queue>

struct EuclideanGraph::Graph {
    struct Vertex;
    struct Edge;

    const EuclideanGraph& owner;
    std::map<size_t, Vertex> vertices;
    std::vector<Edge> edges;

    Graph(const EuclideanGraph& own) : owner(own) {}
    ~Graph();

    void insert_vertex(size_t index) {
        vertices[index] = Vertex(*this, index);
    }

    void insert_edge(size_t from, size_t to) {
        edges.push_back(vertices.at(from), vertices.at(to));
        vertices[from].edge_indices.push_back(edges.size()-1);
        vertices[to].edge_indices.push_back(edges.size()-1);
    }

    void insert(size_t index, double radius) {
        Vec2D point = getPoint(index);
        std::vector<size_t> nearby = owner.radius_search(point, radius);

        vertices[index] = Vertex(*this, index);
        const Vertex& from = vertices[index];

        for(int i = 0; i < nearby.size(); i++) {
            insert_edge(from, vertices.at(nearby[i]));
        }
    }

    Vec2D getPoint(size_t idx) const {
        return owner.points.at(idx);
    }

    struct Vertex {
        const Graph& owner;
        size_t index;
        std::vector<size_t> edge_indices;

        Vertex(const EuclideanGraph::Graph& own, size_t idx) : owner(own), index(idx) {}

        Vertex& operator=(const Vertex& other);

        bool operator==(const Vertex& other);
        bool operator!=(const Vertex& other);
        bool operator<(const Vertex& other);
        bool operator>(const Vertex& other);
        bool operator<=(const Vertex& other);
        bool operator>=(const Vertex& other);
    };

    struct Edge {
        const Vertex& from;
        const Vertex& to;
        double weight;

        Edge(const Vertex& f, const Vertex& t) : from(f), to(t) {
            Vec2D from_vec = from.owner.getPoint(from.index);
            Vec2D to_vec = to.owner.getPoint(to.index);
            if(from != to) {
                weight = distance(from_vec, to_vec);
            } else {
                weight = 0;
            }
        }
    };

};

#include "Graph.impl"

#endif
