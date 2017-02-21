#include "EuclideanGraph.hpp"

bool cmp_x(const Vec2D& a, const Vec2D& b) {

    if(a[0] < b[0]) {
        return true;
    } else {
        return false;
    }
}

bool cmp_y(const Vec2D& a, const Vec2D& b) {

    if(a[1] < b[1]) {
        return true;
    } else {
        return false;
    }
}

void sortRangeByComponent(std::vector<Vec2D>& input, size_t component, size_t start, size_t end) {
    if(start == end) {
        return;
    }

    if(component == 0) {
        std::sort(input.begin()+start, input.begin()+end, cmp_x);
    } else if(component == 1) {
        std::sort(input.begin()+start, input.begin()+end, cmp_y);
    }
}

void sortByComponent(std::vector<Vec2D>& input, size_t component) {
    sortRangeByComponent(input, component, 0, input.size());
}



EuclideanGraph::EuclideanGraph() {
    pointTree = new KDTree(*this);
    graph = new Graph(*this);
}

EuclideanGraph::~EuclideanGraph() {
    delete pointTree;
    delete graph;
}

void EuclideanGraph::insert(Vec2D point, double radius) {
    points.push_back(point);
    graph->insert(points.size()-1, radius);
    pointTree->insert(points.size()-1);
}

size_t EuclideanGraph::nearest_neighbor(Vec2D point) const {
    return pointTree->nearest_neigbor(point);
}

std::vector<size_t> EuclideanGraph::radius_search(Vec2D point, double radius) const {
    return pointTree->radius_search(point, radius);
}

std::stack<Vec2D> EuclideanGraph::A_Star(Vec2D start, Vec2D goal) const {
    size_t start_idx = nearest_neighbor(start);
    size_t goal_idx = nearest_neighbor(goal);

    return graph->A_Star(start_idx, goal_idx);
}

void EuclideanGraph::clear() {
    pointTree->clear();
    points.clear();
}


int main(int argc, char** argv) {

    EuclideanGraph eucl_graph = EuclideanGraph();



    return 0;
}
