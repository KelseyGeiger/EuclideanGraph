//#include "EuclideanGraph.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

Vec2D EuclideanGraph::get_point(size_t index) const {
    return points.at(index);
}

void EuclideanGraph::adjust_points(Vec2D offset) {
    for(size_t i = 0; i < points.size(); i++) {
        points[i] = points[i] + offset;
    }
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

std::stack<Vec2D> EuclideanGraph::A_star(Vec2D start, Vec2D goal) const {
    size_t start_idx = nearest_neighbor(start);
    size_t goal_idx = nearest_neighbor(goal);

    return graph->A_star(start_idx, goal_idx);
}

void EuclideanGraph::clear() {
    pointTree->clear();
    points.clear();
}

//UNCOMMENT THIS SECTION AND COMPILE THIS FILE FOR TESTS
//THIS WILL BE MOVED TO EXTERNAL TEST FILES SOON

/*
Vec2D new_point(const EuclideanGraph& graph, Vec2D current, double heading) {
    std::vector<size_t> within_meter = graph.radius_search(current, 1.0);
    double x_partial_der = 0.0;
    double y_partial_der = 0.0;

    for(size_t i = 0; i < within_meter.size(); i++) {
        Vec2D pnt = graph.get_point(within_meter[i]);
        double x_num = -.6096 * (pnt.x + (pnt.x - current.x));
        double y_num = -.6096 * (pnt.y + (pnt.y - current.y));

        double denom = mag_sqr(pnt - current) + 1.0;
        denom = denom * denom;

        x_partial_der += (x_num / denom);
        y_partial_der += (y_num / denom);
    }

    Vec2D grad = Vec2D(x_partial_der, y_partial_der);
    Vec2D head = Vec2D(std::cos(heading), std::sin(heading));
    grad += head;
    return .3048 * normal(grad);
}


int main(int argc, char** argv) {

    EuclideanGraph eucl_graph = EuclideanGraph();
    srand(time(NULL));

    double initial_heading = (double)(rand() % 360);
    initial_heading = (initial_heading / 360) * M_PI * 2.0;


    return 0;
}*/
