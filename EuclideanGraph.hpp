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

    double& operator[](size_t idx);
    const double& operator[](size_t idx) const;
};

bool cmp_x(const Point2D& a, const Point2D& b);
bool cmp_y(const Point2D& a, const Point2D& b);

void sortRangeByComponent(std::vector<Point2D>& input, size_t component, size_t start, size_t end);
void sortByComponent(std::vector<Point2D>& input, size_t component);

class EuclideanGraph {
    public:
        
        EuclideanGraph(double r);
        ~EuclideanGraph();
        
        void insert(Point2D point);

        size_t nearest_neighbor(Point2D point);

        void build_tree();
        void clear();
        
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
            
            ~KDTree();
            
            Point2D getPoint(size_t idx) const;
            size_t nearest_neigbor(Point2D point);
            std::vector<size_t> radius_search(Point2D point, double radius);

            void insert(size_t idx);

            void insert(std::vector<Point2D>& points);

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

                KDTreeNode* insert(size_t idx);
                void insert(std::vector<Point2D>& points, size_t start, size_t end);

                size_t nearest_neighbor(Point2D point, size_t current_best);
                std::vector<size_t> radius_search(std::vector<size_t>& in_rad, Point2D point, double radius);

            };
        };
};
