#include <vector>
#include <algorithm>

struct Point2D {
    double v[2];
}

bool cmp_x(const Point2D& a, const Point2D& b) {
    
    if(a.v[0] < b.v[0]) {
        return true;
    } else {
        return false;
    }
}

bool cmp_y(const Point2D& a, const Point2D& b) {
    
    if(a.v[1] < b.v[1]) {
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
    if(component == 0) {
        std::sort(input.begin()+start, input.begin()+end, cmp_x);
    } else if(component == 1) {
        std::sort(input.begin()+start, input.begin()+end, cmp_y);
    }
}

class EuclideanGraph {
    public:
        
        EuclideanGraph() {
            pointTree = new KDTree(*this);
        }
        
        ~EuclideanGraph() {
            delete pointTree;
        }
        
        
    private:
        
        std::vector<Point2D> points;
        double radius;
        KDTree* pointTree;
        
        struct KDTree {
            
            const EuclideanGraph& owner;
            KDTreeNode* root;
            size_t count;
            
            KDTree(const EuclideanGraph& own) : owner(own) {
                root = NULL;
                count = 0
            }
            
            ~KDTree() {
                delete root;
            }
            
            Point2D getPoint(size_t idx) {
                return owner.points.at(idx);
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
                    component = level % 2;
                    Point2D to_insert = owner.getPoint(idx);
                    Point2D sep = owner.getPoint(separator);
                    
                    if(to_insert.v[component] < sep.v[component]) {
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
                    if(points.size() > 1) {
                        size_t median = start + ((end - start)/2);
                        size_t component = (level+1) % 2;
                        separator = median;
                        
                        if(less == NULL) {
                            less = new KDTreeNode(own, (size_t)(-1), level+1);
                        }
                        
                        if(greater == NULL) {
                            greater = new KDTreeNode(own, (size_t)(-1), level+1);
                        }
                        
                        if((median % 2) == 1) {
                            sortRangeByComponent(points, component, start, median);
                            sortRangeByComponent(points, component, median+1, end);
                            
                            less->insert(points, start, median);
                            greater->insert(points, median+1, end);
                            
                        } else {
                            sortRangeByComponent(points, component, start, median-1);
                            sortRangeByComponent(points, component, median, end);
                            
                            less->insert(points, start, median-1);
                            greater->insert(points, median, end);
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    
    EuclideanGraph eucl_graph;

    return 0;
}
