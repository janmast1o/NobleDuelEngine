#include "hitbox.h"
#include "utility_functions.cpp"

#ifndef HITBOX_CPP
#define HITBOX_CPP


Hitbox::Hitbox(Point& center, std::vector<Point>& relative_hull) : 
    center_(center), relative_hull_(relative_hull), active_(false) {
        float leftmost_x = relative_hull_[support(relative_hull_, Point(-1,0))].x;
        float rightmost_x = relative_hull_[support(relative_hull_, Point(1,0))].x;
        float upmost_y = relative_hull_[support(relative_hull_, Point(0,1))].y;
        float downmost_y = relative_hull_[support(relative_hull_, Point(-1,0))].y;
        Point lower_left(leftmost_x, downmost_y);
        Point upper_right(rightmost_x, upmost_y);
        relative_rectangle_ = Rectangle(lower_left, upper_right);
        relative_gentle_slope_top_ = find_gentle_slope_top(relative_hull_);
        relative_top_ = find_top(relative_hull_);
        relative_left_top_ = find_left_top(relative_hull_);
        relative_right_top_ = find_right_top(relative_hull_);
        relative_bottom_ = find_bottom(relative_hull_);
    }


void Hitbox::make_active() {
    active_ = true;
}


void Hitbox::make_inactive() {
    active_ = false;
}


bool Hitbox::collides_with(const Hitbox& other_hitbox) const {
    Rectangle current_rectangle(relative_rectangle_.lower_left+center_, relative_rectangle_.upper_right+center_);
    Rectangle other_current_rectangle(other_hitbox.relative_rectangle_.lower_left+other_hitbox.center_,other_hitbox.relative_rectangle_.upper_right+other_hitbox.center_); // dont apply vector translation to other
    if (!(current_rectangle.collides_with(other_current_rectangle))) {
        return false;
    }
    std::vector<Point> current_hitbox_hull(relative_hull_.size());
    std::copy(std::begin(relative_hull_), std::end(relative_hull_), std::begin(current_hitbox_hull));
    for (int i=0; i<current_hitbox_hull.size(); i++) {
        current_hitbox_hull[i] = current_hitbox_hull[i]+center_;
    }
    std::vector<Point> other_current_hitbox_hull(other_hitbox.relative_hull_.size());
    std::copy(std::begin(other_hitbox.relative_hull_), std::end(other_hitbox.relative_hull_), std::begin(other_current_hitbox_hull));
    for (int i=0; i<other_current_hitbox_hull.size(); i++) {
        other_current_hitbox_hull[i] = other_current_hitbox_hull[i]+other_hitbox.center_;
    }
    return GJK(current_hitbox_hull, other_current_hitbox_hull);
}


Point& Hitbox::get_center() {
    return center_;
} 


std::vector<Point> Hitbox::get_current_hull() {
    std::vector<Point> current_hull(relative_hull_.size());
    for (int i=0; i<relative_hull_.size(); i++) {
        current_hull[i] = relative_hull_[i]+center_;
    }
    return current_hull;
}


bool Hitbox::collides_with_after_vector_translation(const Hitbox& other_hitbox, const Point& translation_vector) const {
    Rectangle current_rectangle(relative_rectangle_.lower_left+center_+translation_vector, relative_rectangle_.upper_right+center_+translation_vector);
    Rectangle other_current_rectangle(other_hitbox.relative_rectangle_.lower_left+other_hitbox.center_,other_hitbox.relative_rectangle_.upper_right+other_hitbox.center_); // dont apply vector translation to other
    if (!(current_rectangle.collides_with(other_current_rectangle))) {
        return false;
    }
    std::vector<Point> current_hitbox_hull(relative_hull_.size());
    for (int i=0; i<current_hitbox_hull.size(); i++) {
        current_hitbox_hull[i] = relative_hull_[i]+center_+translation_vector;
    }
    std::vector<Point> other_current_hitbox_hull(other_hitbox.relative_hull_.size());
    for (int i=0; i<other_current_hitbox_hull.size(); i++) {
        other_current_hitbox_hull[i] = other_hitbox.relative_hull_[i]+other_hitbox.center_;
    }
    return GJK(current_hitbox_hull, other_current_hitbox_hull);
}


bool Hitbox::is_directly_above(const Hitbox& other_hitbox) const {
    Point slight_raise_vector(0, SLIGHT_RAISE);
    Rectangle pushed_down_current_rectangle(relative_rectangle_.lower_left+center_-slight_raise_vector, relative_rectangle_.upper_right+center_-slight_raise_vector);
    Rectangle other_current_rectangle(other_hitbox.relative_rectangle_.lower_left+other_hitbox.center_, other_hitbox.relative_rectangle_.upper_right+other_hitbox.center_);
    if (!pushed_down_current_rectangle.collides_with(other_current_rectangle)) {
        return false;
    }
    std::vector<Point> current_hitbox_hull(relative_hull_.size());
    for (int i=0; i<current_hitbox_hull.size(); i++) {
        current_hitbox_hull[i] = relative_hull_[i]+center_+slight_raise_vector;
    }
    std::vector<Point> other_current_hitbox_top(other_hitbox.relative_top_.size());
    for (int i=0; i<other_current_hitbox_top.size(); i++) {
        other_current_hitbox_top[i] = other_hitbox.relative_top_[i]+other_hitbox.center_;
    }
    if (!GJK(current_hitbox_hull, other_current_hitbox_top)) {
        for (int i=0; i<current_hitbox_hull.size(); i++) {
            current_hitbox_hull[i] = current_hitbox_hull[i]-slight_raise_vector-slight_raise_vector;
        }
        if (GJK(current_hitbox_hull, other_current_hitbox_top)) {
            return true;
        }
    }
    return false;
}


bool Hitbox::is_directly_above_after_vector_translation(const Hitbox& other_hitbox, const Point& translation_vector) const {
    Point slight_raise_vector(0, SLIGHT_RAISE);
    Rectangle pushed_down_current_rectangle(relative_rectangle_.lower_left+center_+translation_vector-slight_raise_vector, relative_rectangle_.upper_right+center_+translation_vector-slight_raise_vector);
    Rectangle other_current_rectangle(other_hitbox.relative_rectangle_.lower_left+other_hitbox.center_, other_hitbox.relative_rectangle_.upper_right+other_hitbox.center_);
    if (!pushed_down_current_rectangle.collides_with(other_current_rectangle)) {
        return false;
    }
    std::vector<Point> current_hitbox_hull(relative_hull_.size());
    for (int i=0; i<current_hitbox_hull.size(); i++) {
        current_hitbox_hull[i] = relative_hull_[i]+center_+translation_vector+slight_raise_vector;
    }
    std::vector<Point> other_current_hitbox_top(other_hitbox.relative_top_.size());
    for (int i=0; i<other_current_hitbox_top.size(); i++) {
        other_current_hitbox_top[i] = other_hitbox.relative_top_[i]+other_hitbox.center_;
    }
    if (!GJK(current_hitbox_hull, other_current_hitbox_top)) {
        for (int i=0; i<current_hitbox_hull.size(); i++) {
            current_hitbox_hull[i] = current_hitbox_hull[i]-slight_raise_vector-slight_raise_vector;
        }
        if (GJK(current_hitbox_hull, other_current_hitbox_top)) {
            return true;
        }
    }
    return false; 
}


float Hitbox::is_collision_after_vector_translation_caused_by_gentle_slope(const Hitbox& other_hitbox, const Point& translation_vector) const {
    if (other_hitbox.relative_gentle_slope_top_.size() == 0) {
        return -INFINITY;
    }
    std::vector<Point> current_hitbox_bottom(relative_bottom_.size());
    for (int i=0; i<current_hitbox_bottom.size(); i++) {
        current_hitbox_bottom[i] = relative_bottom_[i]+center_;
    }
    std::vector<Point> other_current_hitbox_gentle_slope_top(other_hitbox.relative_gentle_slope_top_.size());
    for (int i=0; i<other_current_hitbox_gentle_slope_top.size(); i++) {
        other_current_hitbox_gentle_slope_top[i] = other_hitbox.relative_gentle_slope_top_[i]+other_hitbox.center_;
    }
    float alpha = -INFINITY;
    int current_hitbox_bottom_last = current_hitbox_bottom.size()-1;
    if (translation_vector.x > 0) {
        for (int i=1; i<other_current_hitbox_gentle_slope_top.size(); i++) {
            if (other_current_hitbox_gentle_slope_top[i-1].x - current_hitbox_bottom[current_hitbox_bottom_last].x >= -ERROR_EPS 
                    && current_hitbox_bottom[current_hitbox_bottom_last].x - other_current_hitbox_gentle_slope_top[i].x >= -ERROR_EPS) {
                alpha = find_slope_coefficient(other_current_hitbox_gentle_slope_top[i-1], other_current_hitbox_gentle_slope_top[i]);
                alpha = std::abs(alpha);
                break;
            }
        }
    }
    else if (translation_vector.x < 0) {
        for (int i=1; i<other_current_hitbox_gentle_slope_top.size(); i++) {
            if (other_current_hitbox_gentle_slope_top[i-1].x - current_hitbox_bottom[0].x >= -ERROR_EPS
                    && current_hitbox_bottom[0].x - other_current_hitbox_gentle_slope_top[i].x >= -ERROR_EPS) {
                alpha = find_slope_coefficient(other_current_hitbox_gentle_slope_top[i-1], other_current_hitbox_gentle_slope_top[i]);
                alpha = std::abs(alpha);
                break;
            }
        }
    }
    return alpha;
}


float Hitbox::find_slope_coefficient_directly_below(const Hitbox& other_hitbox, const Point& translation_vector) const {
    std::vector<Point> current_hitbox_bottom(relative_bottom_.size());
    for (int i=0; i<current_hitbox_bottom.size(); i++) {
        current_hitbox_bottom[i] = relative_bottom_[i]+center_+translation_vector;
    }
    std::vector<Point> other_current_hitbox_top(other_hitbox.relative_top_.size());
    for (int i=0; i<other_current_hitbox_top.size(); i++) {
        other_current_hitbox_top[i] = other_hitbox.relative_top_[i]+other_hitbox.center_;
    }
    int current_hitbox_bottom_last = current_hitbox_bottom.size()-1;
    float alpha = 0.0;
    float slope_coefficient;
    for (int i=1; i<other_current_hitbox_top.size(); i++) {
        slope_coefficient = find_slope_coefficient(other_current_hitbox_top[i-1], other_current_hitbox_top[i]);
        if (slope_coefficient < 0) {
            if (other_current_hitbox_top[i-1].x - current_hitbox_bottom[0].x >= -ERROR_EPS
                    && current_hitbox_bottom[0].x - other_current_hitbox_top[i].x >= -ERROR_EPS) {
                        alpha = slope_coefficient;
                        break;
                    }
        }
        else if (slope_coefficient > 0) {
            if (other_current_hitbox_top[i-1].x - current_hitbox_bottom[current_hitbox_bottom_last].x >= -ERROR_EPS
                    && current_hitbox_bottom[current_hitbox_bottom_last].x - other_current_hitbox_top[i].x >= -ERROR_EPS) {
                        alpha = slope_coefficient;
                        break;
                    }
        }
        else {
            if (other_current_hitbox_top[i-1].x - current_hitbox_bottom[0].x >= -ERROR_EPS
                    && current_hitbox_bottom[0].x - other_current_hitbox_top[i].x >= -ERROR_EPS) {
                        alpha = slope_coefficient;
                        break;
                    }
            else if (other_current_hitbox_top[i-1].x - current_hitbox_bottom[current_hitbox_bottom_last].x >= -ERROR_EPS
                    && current_hitbox_bottom[current_hitbox_bottom_last].x - other_current_hitbox_top[i].x >= -ERROR_EPS) {
                        alpha = slope_coefficient;
                        break;
                    }        
        }
    }
    return slope_coefficient;
}


bool Hitbox::collides_with_top(const Hitbox& other_hitbox, const Point& translation_vector) const {
    Rectangle current_rectangle(relative_rectangle_.lower_left+center_+translation_vector, relative_rectangle_.upper_right+center_+translation_vector);
    Rectangle other_current_rectangle(other_hitbox.relative_rectangle_.lower_left+other_hitbox.center_, other_hitbox.relative_rectangle_.upper_right+other_hitbox.center_);
    if (!current_rectangle.collides_with(other_current_rectangle)) {
        return false;
    }
    std::vector<Point> current_hitbox_hull(relative_hull_.size());
    for (int i=0; i<current_hitbox_hull.size(); i++) {
        current_hitbox_hull[i] = relative_hull_[i]+center_+translation_vector;
    }
    std::vector<Point> other_current_hitbox_top(other_hitbox.relative_top_.size());
    for (int i=0; i<other_current_hitbox_top.size(); i++) {
        other_current_hitbox_top[i] = other_hitbox.relative_top_[i]+other_hitbox.center_;
    }
    if (GJK(current_hitbox_hull, other_current_hitbox_top)) {
        float s_x = translation_vector.x;
        Point horizontal_push_vector;
        if (s_x > 0) {
            horizontal_push_vector = {-SLIGHT_HORIZONTAL_PUSH, 0};
        }
        else if (s_x < 0) {
            horizontal_push_vector = {SLIGHT_HORIZONTAL_PUSH, 0};
        }
        else {
            horizontal_push_vector = {0,0};
        }
        for (int i=0; i<current_hitbox_hull.size(); i++) {
            current_hitbox_hull[i] = current_hitbox_hull[i]+horizontal_push_vector;
        }
        return GJK(current_hitbox_hull, other_current_hitbox_top);
    }
    else {
        return false;
    }
}


float Hitbox::is_collision_after_vector_translation_caused_by_steep_slope(const Hitbox& other_hitbox, const Point& translation_vector) const {
    Point slight_raise_vector(0,SLIGHT_RAISE);
    std::vector<Point> current_hitbox_bottom(2*relative_bottom_.size());
    for (int i=0; i<relative_bottom_.size(); i++) {
        current_hitbox_bottom[i] = relative_bottom_[i]+center_+translation_vector-slight_raise_vector;
    }
    for (int i=0; i<relative_bottom_.size(); i++) { 
        current_hitbox_bottom[i+relative_bottom_.size()] = relative_bottom_[relative_bottom_.size()-i-1]+center_+translation_vector+slight_raise_vector;
    }
    std::vector<Point> other_current_hitbox_gentle_slope_top(other_hitbox.relative_gentle_slope_top_.size());
    for (int i=0; i<other_current_hitbox_gentle_slope_top.size(); i++) {
        other_current_hitbox_gentle_slope_top[i] = other_hitbox.relative_gentle_slope_top_[i]+other_hitbox.center_;
    }
    std::vector<Point> other_current_hitbox_left_top(other_hitbox.relative_left_top_.size());
    for (int i=0; i<other_current_hitbox_left_top.size(); i++) {
        other_current_hitbox_left_top[i] = other_hitbox.relative_left_top_[i]+other_hitbox.center_;
    }
    std::vector<Point> other_current_hitbox_right_top(other_hitbox.relative_right_top_.size());
    for (int i=0; i<other_current_hitbox_right_top.size(); i++) {
        other_current_hitbox_right_top[i] = other_hitbox.relative_right_top_[i]+other_hitbox.center_;
    }
    if (!GJK(current_hitbox_bottom, other_current_hitbox_gentle_slope_top)) {
        if (GJK(current_hitbox_bottom, other_current_hitbox_left_top)) {
            return -1;
        }
        else if (GJK(current_hitbox_bottom, other_current_hitbox_right_top)) {
            return 1;
        }
    }   
    return 0;
}

#endif


