#include "EuclideanGraph.hpp"
#include <iostream>

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

    if(radius > 0.0) {
        graph->insert(points.size()-1, radius);
    }

    pointTree->insert(points.size()-1);
}

size_t EuclideanGraph::nearest_neighbor(Vec2D point) const {
    return pointTree->nearest_neigbor(point);
}

std::vector<size_t> EuclideanGraph::radius_search(Vec2D point, double radius) const {
    return pointTree->radius_search(point, radius);
}

std::stack<Vec2D> EuclideanGraph::A_star(Vec2D start, Vec2D goal) const {
    if(pointTree->root != NULL) {
        size_t start_idx = nearest_neighbor(start);
        size_t goal_idx = nearest_neighbor(goal);

        return graph->A_star(start_idx, goal_idx);
    } else {
        return std::stack<Vec2D>();
    }
}

void EuclideanGraph::clear() {
    pointTree->clear();
    points.clear();
}

//UNCOMMENT THIS SECTION AND COMPILE THIS FILE FOR TESTS
//THIS WILL BE MOVED TO EXTERNAL TEST FILES SOON


Vec2D new_point(const EuclideanGraph& graph, Vec2D current, double heading) {
    std::vector<size_t> within_meter = graph.radius_search(current, 1.0);

    if(within_meter.size() == 0) {
        heading += ((double)(rand() - (RAND_MAX/2))/RAND_MAX) * (10.0 * M_PI/180.0);
        Vec2D next = current + .3048 * Vec2D(std::cos(heading), std::sin(heading));

        if(next.x >= 20.0) {
            next.x = 20.0;

        } else if(next.x <= -20.0) {
            next.x = -19.9;
        }

        if(next.y >= 20.0) {
            next.y = 20.0;
        } else if(next.y <= -20.0) {
            next.y = -20.0;
        }

        return next;
    }

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
    Vec2D next = current + .3048 * normal(grad);

    if(next.x >= 20.0) {
        next.x = 20.0;

    } else if(next.x <= -20.0) {
        next.x = -19.9;
    }

    if(next.y >= 20.0) {
        next.y = 20.0;
    } else if(next.y <= -20.0) {
        next.y = -20.0;
    }

    return next;
}

int main(int argc, char** argv) {

    const double MAX_VELOCITY = 0.3;
    const double MAX_TURN = (M_PI) / 2.0;
    const double TIME_STEP = 0.00001;

    bool returning = false;
    bool searching = true;
    bool picking_up = false;

    EuclideanGraph point_graph = EuclideanGraph();
    EuclideanGraph rover_graph = EuclideanGraph();

    size_t found = 0;
    srand(time(NULL));

    std::vector<Vec2D> random_points;

    for(int i = 0; i < 256; i++) {
        double rand_rad = 1.5 + ((double)(rand() - (RAND_MAX/2))/RAND_MAX) * 18.5;
        double rand_theta = ((double)(rand() - (RAND_MAX/2))/RAND_MAX) * 2 * M_PI;
        Vec2D rand_pt = Vec2D(std::cos(rand_theta), std::sin(rand_theta));
        rand_pt *= rand_rad;
        point_graph.insert(Vec2D(rand_pt.x, rand_pt.y), -1.0);
    }

    double current_heading = (double)(rand() % 360);
    current_heading = (current_heading / 360) * M_PI * 2.0;
    Vec2D current_pos = Vec2D(0.0, 0.0);
    Vec2D goal_pos = Vec2D(0.0, 0.0);
    std::stack<Vec2D> path;
    double t = 0.0;

    rover_graph.insert(current_pos, .3048);

    while(found < 256) {
        double to_turn = angle_rad(goal_pos - current_pos) - current_heading;
        double prev_heading = current_heading;
        double goal_heading = (prev_heading + to_turn);

        Vec2D prev_pos = current_pos;

        if(searching) {
            if(distance_sqr(rover_graph.get_point(rover_graph.nearest_neighbor(current_pos)), current_pos) > 0.2*0.2) {
                rover_graph.insert(current_pos, .3048);
            }

            current_heading += ((double)(rand() - (RAND_MAX/2))/RAND_MAX) * (10.0 * M_PI/180.0);
            goal_pos = new_point(rover_graph, current_pos, current_heading);

            std::cout << "\n\nSearch point: " << goal_pos.x << ", " << goal_pos.y << "\n";

            while(distance_sqr(goal_pos, current_pos) > 0.01) {
                if(std::abs(angle_rad(current_pos - goal_pos) >= M_PI/4.0)) {
                    goal_heading = current_heading + angle_rad(goal_pos - current_pos) - current_heading;

                    while(std::abs(current_heading - goal_heading) > M_PI/180.0) {
                        t += TIME_STEP;
                        current_heading = (current_heading + (MAX_TURN * TIME_STEP));

                        if(current_heading > M_PI) {
                            current_heading = current_heading - (2.0 * M_PI);
                        } else if(current_heading < -M_PI) {
                            current_heading = current_heading + (2.0 * M_PI);
                        }

                        Vec2D view_point = current_pos + (.3048 / 2.0) * Vec2D(std::cos(current_heading), std::sin(current_heading));

                        std::vector<size_t> near = point_graph.radius_search(view_point, .3048 / 2.0);
                        if(near.size() > 0) {
                            searching = false;
                            picking_up = true;
                            goal_pos = point_graph.get_point(point_graph.nearest_neighbor(view_point));
                            std::cout << "\nBlock found at " << goal_pos.x << ", " << goal_pos.y << " while turning.\n";
                            break;
                        }
                    }
                }

                if(!searching) {
                    break;
                }

                t += TIME_STEP;
                current_pos += Vec2D(std::cos(current_heading), std::sin(current_heading)) * (MAX_VELOCITY * TIME_STEP);

                Vec2D view_point = current_pos + (.3048 / 2.0) * Vec2D(std::cos(current_heading), std::sin(current_heading));

                if(point_graph.radius_search(view_point, .3048 / 2.0).size() > 0) {
                    searching = false;
                    picking_up = true;
                    goal_pos = point_graph.get_point(point_graph.nearest_neighbor(view_point));
                    rover_graph.insert(current_pos, .3048);

                    std::cout << "\nBlock found at " << goal_pos.x << ", " << goal_pos.y << " while moving.\n";
                    break;
                }
            }

        } else if(picking_up) {

            while(distance_sqr(goal_pos, current_pos) > 0.01*0.01) {
                if(std::abs(angle_rad(current_pos - goal_pos) >= M_PI/4.0)) {
                    goal_heading = current_heading + angle_rad(goal_pos - current_pos) - current_heading;

                    while(std::abs(current_heading - (prev_heading + to_turn)) > M_PI/180.0) {
                        t += TIME_STEP;
                        current_heading = (current_heading + (MAX_TURN * TIME_STEP));

                        if(current_heading > M_PI) {
                            current_heading = current_heading - (2.0 * M_PI);
                        } else if(current_heading < -M_PI) {
                            current_heading = current_heading + (2.0 * M_PI);
                        }

                    }
                }

                t += TIME_STEP;
                current_pos += Vec2D(std::cos(current_heading), std::sin(current_heading)) * (MAX_VELOCITY * TIME_STEP);
            }

            picking_up = false;
            returning = true;

            std::cout << "\nPicked up cube, returning to home.\n";

            rover_graph.insert(current_pos, .3048);
            path = rover_graph.A_star(current_pos, Vec2D(0.0, 0.0));

        } else {
            if(!path.empty()) {
                goal_pos = path.top();
                path.pop();
                std::cout << "\nNext point to visit: " << goal_pos.x << ", " << goal_pos.y;
            } else {
                std::cout << "\nGoal achieved, searching for the next point.\n";
                returning = false;
                searching = true;
                found++;
                continue;
            }

            while(distance_sqr(goal_pos, current_pos) > 0.01*0.01) {
                if(std::abs(angle_rad(current_pos - goal_pos) >= M_PI/4.0)) {
                    goal_heading = current_heading + angle_rad(goal_pos - current_pos) - current_heading;

                    while(std::abs(current_heading - (prev_heading + to_turn)) > M_PI/180.0) {
                        t += TIME_STEP;
                        current_heading = (current_heading + (MAX_TURN * TIME_STEP));

                        if(current_heading > M_PI) {
                            current_heading = current_heading - (2.0 * M_PI);
                        } else if(current_heading < -M_PI) {
                            current_heading = current_heading + (2.0 * M_PI);
                        }

                    }
                }

                t += TIME_STEP;
                current_pos += Vec2D(std::cos(current_heading), std::sin(current_heading)) * (MAX_VELOCITY * TIME_STEP);
            }
        }
    }

    std::cout << "\n\n\nAll 256 points found in " << t << " simulated seconds.\n";

    return 0;
}
