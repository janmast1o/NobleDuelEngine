#include "structs_and_enums.h"
#include "compilation_time_constants.h"
#include <vector>
#include <cmath>

#ifndef DOT
#define DOT

float dot (const Point& a, const Point& b) {
    return a.x*b.x + a.y*b.y;
}

#endif

#ifndef TRIPLE_PROD
#define TRIPLE_PROD

Point triple_prod (const Point& a, const Point&b, const Point& c) {
    float alpha = a.x*b.y - a.y*b.x;
    return Point(-alpha*c.y, alpha*c.x);
}

#endif

#ifndef SUPPORT
#define SUPPORT

int support(const std::vector<Point>& S, const Point& vector_d) {
    int sought = 0;
    float sought_dot_product = dot(S[0],vector_d);
    Point v;
    float dot_product;
    for (int i=1; i<S.size(); i++) {
        v = S[i];
        dot_product = v.x*vector_d.x + v.y*vector_d.y;
        if (dot_product > sought_dot_product) {
            sought = i;
            sought_dot_product = dot_product;
        }
    }
    return sought;
}

#endif

#ifndef FIND_SLOPE_COEFFICIENT
#define FIND_SLOPE_COEFFICIENT

float find_slope_coefficient(const Point& A, const Point& B) {
    if (std::abs(A.x - B.x) < ERROR_EPS) {
        return INFINITY;
    }
    else {
        return (A.y-B.y) / (A.x-B.x);
    }
}

#endif

#ifndef FIND_GENTLE_SLOPE_TOP
#define FIND_GENTLE_SLOPE_TOP

std::vector<Point> find_gentle_slope_top(const std::vector<Point>& polygon) {
    float max_gentle_slope_coefficient = MAXIMUM_GENTLE_SLOPE_COEFFICIENT;
    int n = polygon.size();
    int leftmost_index, rightmost_index;
    leftmost_index = support(polygon, Point(-1,0));
    rightmost_index = support(polygon, Point(1,0));
    int i = rightmost_index+1;
    int start, end;
    while (true) {
        if (std::abs(find_slope_coefficient(polygon[i%n], polygon[(i-1)%n])) <= max_gentle_slope_coefficient) {
            break;
        }
        else if (i%n == leftmost_index) {
            i++;
            break;
        }
        else {
            i++;
        }
    }
    start = (i-1)%n;
    while (true) {
        if (std::abs(find_slope_coefficient(polygon[i%n], polygon[(i-1)%n])) > max_gentle_slope_coefficient) {
            break;
        }
        else if (i%n == leftmost_index) {
            i++;
            break;
        }
        else {
            i++;
        }
    }
    end = (i-1)%n;
    if (start == end) {
        return std::vector<Point>(); // maybe should be removed
    } 
    else {
        std::vector<Point> gentle_slope_top;
        i = start;
        while (true) {
            gentle_slope_top.push_back(polygon[i%n]);
            if (i%n == end) {
                break;
            }
            else {
                i++;
            }
        }
        return gentle_slope_top;
    }
} 

#endif

#ifndef FIND_TOP
#define FIND_TOP

std::vector<Point> find_top(const std::vector<Point>& polygon) {
    int n = polygon.size();
    int leftmost_index = 0;
    int rightmost_index = 0;
    for (int i=1; i<n; i++) {
        if (polygon[i].x < polygon[leftmost_index].x) {
            leftmost_index = i;
        }
        else if (polygon[i].x == polygon[leftmost_index].x && polygon[i].y > polygon[leftmost_index].y) {
            leftmost_index = i;
        }
        if (polygon[i].x > polygon[rightmost_index].x) {
            rightmost_index = i;
        }
        else if (polygon[i].x == polygon[rightmost_index].x && polygon[i].y > polygon[rightmost_index].y) {
            rightmost_index = i;
        }
    }
    std::vector<Point> top;
    int i = rightmost_index;
    while (true) {
        top.push_back(polygon[i%n]);
        if (i%n == leftmost_index) {
            break;
        }
        else {
            i++;
        }
    }
    return top;
}

#endif

#ifndef FIND_LEFT_TOP
#define FIND_LEFT_TOP

std::vector<Point> find_left_top(const std::vector<Point>& polygon) {
    int n = polygon.size();
    int topmost_index, leftmost_index;
    topmost_index = support(polygon, Point(0,1));
    leftmost_index = support(polygon, Point(-1,0));
    std::vector<Point> left_top;
    int i = topmost_index;
    while (true) {
        left_top.push_back(polygon[i%n]);
        if (i%n == leftmost_index) {
            break;
        }
        else {
            i++;
        }
    }
    return left_top;
}

#endif

#ifndef FIND_RIGHT_TOP
#define FIND_RIGHT_TOP

std::vector<Point> find_right_top(const std::vector<Point>& polygon) {
    int n = polygon.size();
    int topmost_index, rightmost_index;
    topmost_index = support(polygon, Point(0,1));
    rightmost_index = support(polygon, Point(1,0));
    std::vector<Point> right_top;
    int i = rightmost_index;
    while (true) {
        right_top.push_back(polygon[i%n]);
        if (i%n == topmost_index) {
            break;
        }
        else {
            i++;
        }
    }
    return right_top;
}

#endif

#ifndef FIND_BOTTOM
#define FIND_BOTTOM

std::vector<Point> find_bottom(const std::vector<Point>& polygon) {
    int n = polygon.size();
    int lowest = support(polygon, Point(0,-1));
    float lowest_y = polygon[lowest].y;
    std::vector<Point> bottom;
    if (lowest == 0) {
        if (polygon[n-1].y == lowest_y) {
            bottom = {polygon[lowest], polygon[n-1]};
        }
        else if (polygon[(lowest+1)%n].y == lowest_y) {
            bottom = {polygon[lowest], polygon[(lowest+1)%n]};
        }
        else {
            bottom = {polygon[lowest]};
        }
    }
    else {
        if (polygon[lowest-1].y == lowest_y) {
            bottom = {polygon[lowest], polygon[lowest-1]};
        }
        else if (polygon[(lowest+1)%n].y == lowest_y) {
            bottom = {polygon[lowest], polygon[(lowest+1)%n]};
        }
        else {
            bottom = {polygon[lowest]};
        }
    }
    if (bottom.size() == 2 && bottom[0].x > bottom[1].x) {
        std::swap(bottom[0], bottom[1]);
    }
    return bottom;
}

#endif

#ifndef GJK_ALGORITHM
#define GJK_ALGORITHM

bool GJK(std::vector<Point>& S, std::vector<Point>& R) {
    if (S.size() == 0 || R.size() == 0) {
        return false;
    }
    Point vector_d(1.0,0.0);
    Point origin(0.0,0.0);
    Point A, B, C, AB, AC, AO, AB_perpendicular, AC_perpendicular;
    A = S[support(S,vector_d)] - R[support(R,-vector_d)];
    vector_d = origin - A;
    std::vector<Point> simplex = {A};

    while (true) {
        A = S[support(S,vector_d)] - R[support(R,-vector_d)];
        if (dot(A,vector_d) < ERROR_EPS) {
            return false;
        }
        else {
            simplex.push_back(A);
            if (simplex.size() == 2) {
                B = simplex[0];
                A = simplex[1];
                AB = B-A;
                AO = origin-A;
                AB_perpendicular = triple_prod(AB,AO,AB);
                vector_d = AB_perpendicular;
            }
            else {
                C = simplex[0];
                B = simplex[1];
                A = simplex[2];
                AO = origin-A;
                AB = B-A;
                AC = C-A;
                AB_perpendicular = triple_prod(AC,AB,AB);
                AC_perpendicular = triple_prod(AB,AC,AC);
                if (dot(AB_perpendicular,AO) > ERROR_EPS) {
                    simplex.erase(simplex.begin());
                    vector_d = AB_perpendicular;
                }
                else if (dot(AC_perpendicular,AO) > ERROR_EPS) {
                    simplex.erase(simplex.begin()+1);
                    vector_d = AC_perpendicular;
                }
                else {
                    return true;
                }
            }
        }
    }
}

#endif

#ifndef IS_LEFT_FACING
#define IS_LEFT_FACING

bool is_left_facing(State state) {
    if (state == IDLE_LEFT || 
        state == MOVING_LEFT ||
        state == AIRBORNE_LEFT ||
        state == FREEFALL_LEFT || 
        state == QUICKLY_M_LEFT ||
        state == QUICKLY_M_LEFT) {
            return true;
        }
    else {
        return false;
    }    
}

#endif

#ifndef IS_RIGHT_FACING
#define IS_RIGHT_FACING

bool is_right_facing(State state) {
    if (state == IDLE_RIGHT || 
        state == MOVING_RIGHT ||
        state == AIRBORNE_RIGHT ||
        state == FREEFALL_RIGHT || 
        state == QUICKLY_M_RIGHT ||
        state == QUICKLY_M_RIGHT) {
            return true;
        }
    else {
        return false;
    }    
}

#endif

