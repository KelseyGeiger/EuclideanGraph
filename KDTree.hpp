#ifndef KDTREE_HPP
#define KDTREE_HPP

#include "Vec2D.hpp"

struct EuclideanGraph::KDTree {

    struct KDTreeNode;

    const EuclideanGraph& owner;
    KDTreeNode* root;
    size_t count;

    KDTree(const EuclideanGraph& own) : owner(own) {
        root = NULL;
        count = 0;
    }

    ~KDTree();

    Vec2D get_point(size_t idx) const;
    size_t nearest_neighbor(Vec2D point) const;
    std::vector<size_t> radius_search(Vec2D point, double radius) const;

    void insert(size_t idx);
    void insert(std::vector<size_t> points);
    void remove(size_t idx);
    void clear();

    struct KDTreeNode {
        const KDTree& owner;
        size_t separator;
        size_t level;
        KDTreeNode* less;
        KDTreeNode* greater;

        KDTreeNode(const KDTree& own, size_t idx, size_t lvl) : owner(own) {
            separator = idx;
            level = lvl;
            less = NULL;
            greater = NULL;
        }

        ~KDTreeNode();

        void insert(size_t idx);
        void insert(std::vector<size_t> points);
        void remove(size_t idx);
        std::vector<size_t> get_children() const;
        void update_indices(size_t idx);
        size_t nearest_neighbor(Vec2D point, size_t current_best) const;
        std::vector<size_t> radius_search(Vec2D point, double radius) const;

    };
};

#include "KDTree.impl"

#endif
