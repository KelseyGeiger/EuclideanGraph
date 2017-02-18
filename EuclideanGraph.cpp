#include "EuclideanGraph.hpp"

double& Point2D::operator[](size_t idx) {
    if((idx % 2) == 0) {
        return x;
    } else {
        return y;
    }
}

const double& Point2D::operator[](size_t idx) const {
    if((idx % 2) == 0) {
        return x;
    } else {
        return y;
    }
}

bool cmp_x(const Point2D& a, const Point2D& b) {

    if(a[0] < b[0]) {
        return true;
    } else {
        return false;
    }
}

bool cmp_y(const Point2D& a, const Point2D& b) {

    if(a[1] < b[1]) {
        return true;
    } else {
        return false;
    }
}

void sortRangeByComponent(std::vector<Point2D>& input, size_t component, size_t start, size_t end) {
    if(start == end) {
        return;
    }

    if(component == 0) {
        std::sort(input.begin()+start, input.begin()+end, cmp_x);
    } else if(component == 1) {
        std::sort(input.begin()+start, input.begin()+end, cmp_y);
    }
}

void sortByComponent(std::vector<Point2D>& input, size_t component) {
    sortRangeByComponent(input, component, 0, input.size());
}



EuclideanGraph::EuclideanGraph(double r) {
    radius = r;
    pointTree = new KDTree(*this);
}

EuclideanGraph::~EuclideanGraph() {
    delete pointTree;
}

void EuclideanGraph::insert(Point2D point) {
    points.push_back(point);
    pointTree->insert(points.size()-1);
}

size_t EuclideanGraph::nearest_neighbor(Point2D point) {
    return pointTree->nearest_neigbor(point);
}

void EuclideanGraph::build_tree() {
    pointTree->clear();
    pointTree = new KDTree(*this);
    pointTree->insert(points);
}

void EuclideanGraph::clear() {
    pointTree->clear();
    points.clear();
}

EuclideanGraph::KDTree::~KDTree() {
    delete root;
}

Point2D EuclideanGraph::KDTree::getPoint(size_t idx) const {
    return owner.points.at(idx);
}

size_t EuclideanGraph::KDTree::nearest_neigbor(Point2D point) {
    if(root) {
        return root->nearest_neighbor(point, root->separator);
    } else {
        return (size_t)(-1);
    }
}

std::vector<size_t> EuclideanGraph::KDTree::radius_search(Point2D point, double radius) {
    if(root) {
        std::vector<size_t> in_rad;
        root->radius_search(in_rad, point, radius);
    } else {
        return std::vector<size_t>();
    }
}

void EuclideanGraph::KDTree::insert(size_t idx) {
    if(root == NULL) {
        root = new KDTreeNode(*this, idx, 0);
    } else {
        root->insert(idx);
    }
    count++;
}

void EuclideanGraph::KDTree::insert(std::vector<Point2D>& points) {
    if(root == NULL) {
        root = new KDTreeNode(*this, (size_t)(-1), 0);
    }
    root->insert(points, 0, points.size()-1);
    count += points.size();
}

void EuclideanGraph::KDTree::clear() {
    delete root;
    root = NULL;
    count = 0;
}

EuclideanGraph::KDTree::KDTreeNode::~KDTreeNode() {
    delete less;
    delete greater;
}

EuclideanGraph::KDTree::KDTreeNode* EuclideanGraph::KDTree::KDTreeNode::insert(size_t idx) {
    size_t component = level % 2;
    Point2D to_insert = owner.getPoint(idx);
    Point2D sep = owner.getPoint(separator);

    if(to_insert[component] < sep[component]) {
        if(less != NULL) {
            return less->insert(idx);
        } else {
            less = new KDTreeNode(owner, idx, level + 1);
            return less;
        }
    } else {
        if(greater != NULL) {
            return greater->insert(idx);
        } else {
            greater = new KDTreeNode(owner, idx, level + 1);
            return greater;
        }
    }
}

void EuclideanGraph::KDTree::KDTreeNode::insert(std::vector<Point2D>& points, size_t start, size_t end) {
    if(points.size() > 0) {
        if((end-start) > 1) {
            size_t median = (start + ((end - start)/2));
            size_t component = (level+1) % 2;
            separator = median;

            if(less == NULL) {
                less = new KDTreeNode(owner, (size_t)(-1), level+1);
            }

            if(greater == NULL) {
                greater = new KDTreeNode(owner, (size_t)(-1), level+1);
            }

            sortRangeByComponent(points, component, start, median);
            sortRangeByComponent(points, component, median+1, end);

            less->insert(points, start, median);
            greater->insert(points, median+1, end);

        } else {
            separator = start;
        }
    }
}

size_t EuclideanGraph::KDTree::KDTreeNode::nearest_neighbor(Point2D point, size_t current_best) {
    Point2D sep = owner.getPoint(separator);

    if(less != NULL) {
        current_best = less->nearest_neighbor(point, current_best);
    }

    Point2D best_pt = owner.getPoint(current_best);
    double best_dist = std::sqrt((best_pt.x - point.x) * (best_pt.x - point.x) + (best_pt.y - point.y) * (best_pt.y - point.y));

    if(std::abs(best_pt[level%2] - sep[level%2]) > best_dist) {
        return current_best;
    }

    if(greater != NULL) {
        current_best = greater->nearest_neighbor(point, current_best);
    }

    return current_best;
}

std::vector<size_t> EuclideanGraph::KDTree::KDTreeNode::radius_search(std::vector<size_t>& in_rad, Point2D point, double radius) {
    Point2D sep = owner.getPoint(separator);

    double dist = std::sqrt(((sep.x - point.x) * (sep.x - point.x)) + ((sep.y - point.y) * (sep.y - point.y)));

    if(dist <= radius) {
        in_rad.push_back(separator);
    }

    if(std::abs(sep[level%2] - point[level%2]) > radius) {
        if((less != NULL) && (point[level%2] < sep[level%2])) {
            less->radius_search(in_rad, point, radius);
        } else {
            if(greater != NULL) {
                greater->radius_search(in_rad, point, radius);
            }
        }
    } else {
        less->radius_search(in_rad, point, radius);
        greater->radius_search(in_rad, point, radius);
    }
}


int main(int argc, char** argv) {

    EuclideanGraph eucl_graph = EuclideanGraph(0.3048);



    return 0;
}
