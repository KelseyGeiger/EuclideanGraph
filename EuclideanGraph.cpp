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
}

EuclideanGraph::~EuclideanGraph() {
    delete pointTree;
}

void EuclideanGraph::insert(Vec2D point) {
    points.push_back(point);
    pointTree->insert(points.size()-1);
}

size_t EuclideanGraph::nearest_neighbor(Vec2D point) const {
    return pointTree->nearest_neigbor(point);
}

std::vector<size_t> EuclideanGraph::radius_search(Vec2D point, double radius) const {
    return pointTree->radius_search(point, radius);
}

void EuclideanGraph::clear() {
    pointTree->clear();
    points.clear();
}


int main(int argc, char** argv) {

    EuclideanGraph eucl_graph = EuclideanGraph();



    return 0;
}
