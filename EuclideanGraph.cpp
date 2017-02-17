#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <stdexcept>

struct Point2D {
    double x;
    double y;

    double& operator[](size_t idx) {
        if(idx == 0) {
            return x;
        } else if(idx == 1) {
            return y;
        } else {
            throw std::out_of_range("Index out of bounds for Point2D.\n");
        }
    }

    const double& operator[](size_t idx) const {
        if(idx == 0) {
            return x;
        } else if(idx == 1) {
            return y;
        } else {
            throw std::out_of_range("Index out of bounds for Point2D.\n");
        }
    }
};

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

class EuclideanGraph {
    public:
        
        EuclideanGraph(double r) {
            radius = r;
            pointTree = new KDTree(*this);
        }
        
        ~EuclideanGraph() {
            delete pointTree;
        }
        
        void insert(Point2D point) {
            points.push_back(point);
            pointTree->insert(points.size()-1);
        }

        size_t nearest_neighbor(Point2D point) {
            return pointTree->nearest_neigbor(point);
        }

        void build_tree() {
            pointTree->clear();
            pointTree = new KDTree(*this);
            pointTree->insert(points);
        }
        
    private:
        
        struct KDTree;
        
        std::vector<Point2D> points;
        double radius;
        KDTree* pointTree;
        
        struct KDTree {
            
            struct KDTreeNode;
            
            const EuclideanGraph& owner;
            KDTreeNode* root;
            size_t count;
            
            KDTree(const EuclideanGraph& own) : owner(own) {
                root = NULL;
                count = 0;
            }
            
            ~KDTree() {
                delete root;
            }
            
            Point2D getPoint(size_t idx) const {
                return owner.points.at(idx);
            }


            size_t nearest_neigbor(Point2D point) {
                if(root) {
                    return root->nearest_neighbor(point, root->separator);
                } else {
                    return (size_t)(-1);
                }
            }

            void insert(size_t idx) {
                if(root == NULL) {
                    root = new KDTreeNode(*this, idx, 0);
                } else {
                    root->insert(idx);
                }
                count++;
            }

            void insert(std::vector<Point2D>& points) {
                if(root == NULL) {
                    root = new KDTreeNode(*this, (size_t)(-1), 0);
                }
                root->insert(points, 0, points.size()-1);
                count += points.size();
            }

            void clear() {
                delete root;
                root = NULL;
                count = 0;
            }
            
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
                
                ~KDTreeNode() {
                    delete less;
                    delete greater;
                }

                size_t nearest_neighbor(Point2D point, size_t current_best) {
                    Point2D sep = owner.getPoint(separator);

                    if(less != NULL) {
                        current_best = less->nearest_neighbor(point, current_best);
                    }

                    Point2D best_pt = owner.getPoint(current_best);
                    double best_dist = std::sqrt((best_pt.x - point.x) * (best_pt.x - point.x) + (best_pt.y - point.y) * (best_pt.y - point.y));

                    if(best_pt[level%2] - sep[level%2] > best_dist) {
                        return current_best;
                    }

                    if(greater != NULL) {
                        current_best = greater->nearest_neighbor(point, current_best);
                    }

                    return current_best;
                }
                
                KDTreeNode* insert(size_t idx) {
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
                
                void insert(std::vector<Point2D>& points, size_t start, size_t end) {
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
            };
        };
};

int main(int argc, char** argv) {
    
    EuclideanGraph eucl_graph = EuclideanGraph(0.3);
    std::srand(std::time(NULL));

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 500000; i++) {
        double x = ((double)(std::rand() - (RAND_MAX/2))/(RAND_MAX)) * 2.0;
        double y = ((double)(std::rand() - (RAND_MAX/2))/(RAND_MAX)) * 2.0;
        Point2D pnt = {x*100, y*100};
        eucl_graph.insert(pnt);
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
    std::cout << "Time for incremental insertion: " << diff.count() << "ms\n";

    double x = ((double)(std::rand() - (RAND_MAX/2))/(RAND_MAX)) * 2.0;
    double y = ((double)(std::rand() - (RAND_MAX/2))/(RAND_MAX)) * 2.0;
    Point2D pnt = {x*100, y*100};

    start = std::chrono::high_resolution_clock::now();
    size_t nn = eucl_graph.nearest_neighbor(pnt);
    end = std::chrono::high_resolution_clock::now();

    diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);

    std::cout << "Time for (potentially) unbalanced tree nearest neighbor search: " << diff.count() << "ms\n\n";

    start = std::chrono::high_resolution_clock::now();
    eucl_graph.build_tree();
    end = std::chrono::high_resolution_clock::now();
    diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);

    std::cout << "Time for balanced tree build: " << diff.count() << "ms\n";

    x = ((double)(std::rand() - (RAND_MAX/2))/(RAND_MAX)) * 2.0;
    y = ((double)(std::rand() - (RAND_MAX/2))/(RAND_MAX)) * 2.0;
    pnt = {x*100, y*100};

    start = start = std::chrono::high_resolution_clock::now();
    nn = eucl_graph.nearest_neighbor(pnt);
    end = std::chrono::high_resolution_clock::now();

    diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);

    std::cout << "Time for balanced tree nearest neighbor search: " << diff.count() << "ms\n\n";

    return 0;
}
