#include <vector>
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

void sortByComponent(std::vector<Point2D>& input, size_t component) {
    if(component == 0) {
        std::sort(input.begin(), input.end(), cmp_x);
    } else if(component == 1) {
        std::sort(input.begin(), input.end(), cmp_y);
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
                root->insert(points, 0, points.size());
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
                        if((end-start) > 0) {
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

    Point2D origin = {0.0, 0.0};
    eucl_graph.insert(origin);

    for(int i = 0; i < 500000; i++) {
        double x = ((double)(std::rand() - (RAND_MAX/2))/(RAND_MAX)) * 2.0;
        double y = ((double)(std::rand() - (RAND_MAX/2))/(RAND_MAX)) * 2.0;
        Point2D pnt = {x*100, y*100};
        eucl_graph.insert(pnt);
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
    std::cout << "Time for incremental insertion: " << diff.count() << "ms\n";

    start = start = std::chrono::high_resolution_clock::now();
    eucl_graph.build_tree();
    end = std::chrono::high_resolution_clock::now();
    diff = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);

    std::cout << "Time for balanced tree build: " << diff.count() << "ms\n";

    return 0;
}
