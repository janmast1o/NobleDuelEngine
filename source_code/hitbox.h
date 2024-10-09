#include <vector>
#include <algorithm>
#include <optional>
#include "structs_and_enums.h"
#include "utility_functions.cpp"
#include "compilation_time_constants.h"

#ifndef HITBOX_H
#define HITBOX_H

namespace GameSystem {
    class Hitbox;
}

class Hitbox {

private:

    Point& center_;
    bool active_;
    std::vector<Point> relative_hull_;
    Rectangle relative_rectangle_;
    std::vector<Point> relative_gentle_slope_top_;
    std::vector<Point> relative_top_;
    std::vector<Point> relative_left_top_;
    std::vector<Point> relative_right_top_;
    std::vector<Point> relative_bottom_;

public:

    Hitbox(Point& center, std::vector<Point>& relative_hull);
    void make_active();
    void make_inactive();
    Point& get_center();
    std::vector<Point> get_current_hull();
    bool collides_with(const Hitbox& other_hitbox) const ;
    bool collides_with_after_vector_translation(const Hitbox& other_hitbox, const Point& translation_vector) const;
    bool is_directly_above(const Hitbox& other_hitbox) const;
    bool is_directly_above_after_vector_translation(const Hitbox& other_hitbox, const Point& translation_vector) const;
    bool collides_with_top(const Hitbox& other_hitbox, const Point& translation_vector) const;
    float is_collision_after_vector_translation_caused_by_gentle_slope(const Hitbox& other_hitbox, const Point& translation_vector) const;
    float find_slope_coefficient_directly_below(const Hitbox& other_hitbox, const Point& translation_vector) const;
    float is_collision_after_vector_translation_caused_by_steep_slope(const Hitbox& other_hitbox, const Point& translation_vector) const;
};

#endif