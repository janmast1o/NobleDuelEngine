#include "mobile_object.h"
#include "compilation_time_constants.h" // temporary

#ifndef MOBILE_OBJECT_CPP
#define MOBILE_OBJECT_CPP

MobileObject::MobileObject(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map) :
    Object(renderer, center, model_collection, matter), object_map_(object_map) {
        acceleration_.a_down = GRAVITATIONAL_PULL;
        slope_incline_directly_underneath_ = 0.0;
        scheduled_ = HANDLE_FREEFALL;
        previously_scheduled_ = NOTHING;
    }


int MobileObject::get_faced_side_as_int() {
    if (is_left_facing(get_state())) {
        return -1;
    }
    else {
        return 1;
    }
}    


void MobileObject::zero_velocity() {
    velocity_.v_up = 0.0;
    velocity_.v_down = 0.0;
    velocity_.v_left = 0.0;
    velocity_.v_right = 0.0;
}


void MobileObject::zero_airborne_ghost_horizontal_velocity() {
    airborne_ghost_horizontal_velocity_.v_left = 0.0;
    airborne_ghost_horizontal_velocity_.v_right = 0.0;
}


void MobileObject::handle_stop() {
    previously_scheduled_ = HANDLE_STOP;
    acceleration_.a_left = 0;
    acceleration_.a_right = 0;
    velocity_.v_left = 0;
    velocity_.v_right = 0;
    clear_scheduled();
}


void MobileObject::new_velocity() {
    if (acceleration_.a_left != acceleration_.a_right) {
        if (velocity_.v_left > SMALLEST_V_UNAFFECTED_BY_AR) {
            velocity_.v_left -= AIR_RESISTANCE_ACC*(1.0/FPS);
        }
        velocity_.v_left += acceleration_.a_left*(1.0/FPS);
        if (velocity_.v_left > object_specific_physics_char_.max_horizontal_v) {
            velocity_.v_left = object_specific_physics_char_.max_horizontal_v;
        }
        if (velocity_.v_right > SMALLEST_V_UNAFFECTED_BY_AR) {
            velocity_.v_right -= AIR_RESISTANCE_ACC*(1.0/FPS);
        }
        velocity_.v_right += acceleration_.a_right*(1.0/FPS);
        if (velocity_.v_right > object_specific_physics_char_.max_horizontal_v) {
            velocity_.v_right = object_specific_physics_char_.max_horizontal_v;
        }
    }    
    if (acceleration_.a_down == acceleration_.a_up) {
        velocity_.v_down = 0.0;
        velocity_.v_up = 0.0;
    }
    else {
        if (velocity_.v_up > SMALLEST_V_UNAFFECTED_BY_AR) {
            velocity_.v_up -= AIR_RESISTANCE_ACC*(1.0/FPS);
        }
        if (velocity_.v_up < object_specific_physics_char_.max_upward_v) {
            velocity_.v_up += acceleration_.a_up*(1.0/FPS);
        }    
        if (velocity_.v_down > SMALLEST_V_UNAFFECTED_BY_AR) {
            velocity_.v_down -= AIR_RESISTANCE_ACC*(1.0/FPS);
        }
        if (velocity_.v_down < object_specific_physics_char_.max_downward_v) {
            velocity_.v_down += acceleration_.a_down*(1.0/FPS);
        }
    }
    if (single_state_persistence_timer_.airborne_timer < object_specific_physics_char_.max_airborne_accelerable_period 
            && single_state_persistence_timer_.freefall_timer < object_specific_physics_char_.max_airborne_accelerable_period) {
                if (airborne_ghost_horizontal_velocity_.v_left > 0) {
                    if (airborne_ghost_horizontal_velocity_.v_left > SMALLEST_V_UNAFFECTED_BY_AR) {
                        airborne_ghost_horizontal_velocity_.v_left -= AIR_RESISTANCE_ACC*(1.0/FPS);
                    }
                    airborne_ghost_horizontal_velocity_.v_left += object_specific_physics_char_.horizontal_acc*(1.0/FPS);
                    if (airborne_ghost_horizontal_velocity_.v_left > object_specific_physics_char_.max_horizontal_v) {
                        airborne_ghost_horizontal_velocity_.v_left = object_specific_physics_char_.max_horizontal_v;
                    }
                }
                else if (airborne_ghost_horizontal_velocity_.v_right > 0) {
                    if (airborne_ghost_horizontal_velocity_.v_right > SMALLEST_V_UNAFFECTED_BY_AR) {
                        airborne_ghost_horizontal_velocity_.v_right -= AIR_RESISTANCE_ACC*(1.0/FPS);
                    }
                    airborne_ghost_horizontal_velocity_.v_right += object_specific_physics_char_.horizontal_acc*(1.0/FPS);
                    if (airborne_ghost_horizontal_velocity_.v_right > object_specific_physics_char_.max_horizontal_v) {
                        airborne_ghost_horizontal_velocity_.v_right = object_specific_physics_char_.max_horizontal_v;
                    }
                } 
            }
}


void MobileObject::new_horizontal_acceleration(Direction direction) {
    if (direction == LEFT) {
        acceleration_.a_left = object_specific_physics_char_.horizontal_acc;
    }
    else if (direction == RIGHT) {
        acceleration_.a_right = object_specific_physics_char_.horizontal_acc;
    }
}


void MobileObject::handle_moving_horizontally() {
    single_state_persistence_timer_.moving_horizontally_timer++;
    single_state_persistence_timer_.freefall_timer = 0;
    single_state_persistence_timer_.airborne_timer = 0;
    previously_scheduled_ = HANDLE_MOVING_HORIZONTALLY;
    new_velocity();
    float s_x = (velocity_.v_right - velocity_.v_left)*(1.0/FPS);
    Point s_vector(s_x, s_x*slope_incline_directly_underneath_);
    std::vector<Object*> potentially_colliding_objects = object_map_.get_potentially_colliding(*this);
    bool collision_detected = false;
    bool ground_underneath_found = false;
    float alpha = -INFINITY;
    for (Object* p : potentially_colliding_objects) {
        if (p != this && collideable_with(*p)) {
            if (collides_with_after_vector_translation(*p,s_vector)) {
                if (is_directly_above_after_vector_translation(*p,s_vector)) {
                    ground_underneath_found = true;
                }
                else if (is_directly_above_after_vector_translation(*p,Point(s_vector.x,0))) {
                    alpha = 0.0;
                    s_vector.y = SLIGHT_RAISE;
                    ground_underneath_found = true;
                    break;
                }
                else {
                    ground_underneath_found = true;
                    alpha = is_collision_after_vector_translation_caused_by_gentle_slope(*p,Point(s_vector.x,0));
                    if (alpha > 0) {
                        s_vector.y = std::abs(s_vector.x*alpha);
                    }
                    else {
                        collision_detected = true;
                        break;
                    }
                }
            }
            else if (!ground_underneath_found && is_directly_above_after_vector_translation(*p,s_vector)) {
                ground_underneath_found = true;
            }
        }
    }
    if (!collision_detected) {
        translate_object_by_vector(s_vector);
        if (!ground_underneath_found) {
            acceleration_.a_left = 0.0;
            acceleration_.a_right = 0.0;
            if (s_vector.x < 0) {
                airborne_ghost_horizontal_velocity_.v_left = velocity_.v_left;
                airborne_ghost_horizontal_velocity_.v_right = 0.0;
            }
            else if (s_vector.x > 0) {
                airborne_ghost_horizontal_velocity_.v_left = 0.0;
                airborne_ghost_horizontal_velocity_.v_right = velocity_.v_right;
            }
            velocity_.v_left = 0.0;
            velocity_.v_right = 0.0;
            remove_ground_reaction_acceleration();
            set_scheduled(HANDLE_FREEFALL);
        }
        else {
            if (alpha >= 0.0) {
                if (s_vector.x < 0) {
                    slope_incline_directly_underneath_ = -alpha;
                }
                else if (s_vector.x > 0) {
                    slope_incline_directly_underneath_ = alpha;
                }
            }
            clear_scheduled();
        }
    }
    else {
        clear_scheduled();
    }
    if (s_vector.x < 0) {
        set_new_state(MOVING_LEFT);
    }
    else if (s_vector.x > 0) {
        set_new_state(MOVING_RIGHT);
    } 
}


void MobileObject::handle_slide_down() {
    single_state_persistence_timer_.moving_horizontally_timer++;
    single_state_persistence_timer_.airborne_timer = 0;
    single_state_persistence_timer_.freefall_timer = 0;
    previously_scheduled_ = HANDLE_SLIDE_DOWN;
    new_velocity();
    float s_x = (velocity_.v_right - velocity_.v_left)*(1.0/FPS);
    Point s_vector(s_x, -(std::abs(s_x*slope_incline_directly_underneath_)));
    std::vector<Object*> potentially_colliding_objects = object_map_.get_potentially_colliding(*this);
    bool collision_detected = false;
    bool ground_underneath_found = false;
    for (Object* p : potentially_colliding_objects) {
        if (p != this && collideable_with(*p)) {    
            if (collides_with_after_vector_translation(*p,s_vector)) {
                if (is_directly_above_after_vector_translation(*p,s_vector)) {
                    ground_underneath_found = true;
                }
                else {
                    collision_detected = true;
                    ground_underneath_found = true;
                    break;
                }
            }
            else if (!ground_underneath_found && is_directly_above_after_vector_translation(*p,s_vector)) {
                ground_underneath_found = true;
            }
        }    
    }
    if (!collision_detected) {
        translate_object_by_vector(s_vector);
        if (!ground_underneath_found) {
            slope_incline_directly_underneath_ = 0.0;
            acceleration_.a_left = 0.0;
            acceleration_.a_right = 0.0;
            acceleration_.a_up = 0.0;
            if (s_vector.x < 0) {
                airborne_ghost_horizontal_velocity_.v_left = velocity_.v_left;
                airborne_ghost_horizontal_velocity_.v_right = 0.0;
            }
            else if (s_vector.x > 0) {
                airborne_ghost_horizontal_velocity_.v_left = 0.0;
                airborne_ghost_horizontal_velocity_.v_right = velocity_.v_right;
            }
            velocity_.v_left = 0.0;
            velocity_.v_right = 0.0;
            remove_ground_reaction_acceleration();
            set_scheduled(HANDLE_FREEFALL);
        }
        else {
            set_scheduled(HANDLE_SLIDE_DOWN);
        }
    }
    else {
        slope_incline_directly_underneath_ = 0.0;
        clear_scheduled();
    }
    if (s_vector.x < 0) {
        set_new_state(SLIDE_DOWN_LEFT);
    }
    else if (s_vector.x > 0) {
        set_new_state(SLIDE_DOWN_RIGHT);
    }
}


void MobileObject::handle_airborne() {
    single_state_persistence_timer_.airborne_timer++;
    single_state_persistence_timer_.moving_horizontally_timer = 0;
    single_state_persistence_timer_.freefall_timer = 0;
    previously_scheduled_ = HANDLE_AIRBORNE;
    remove_ground_reaction_acceleration();
    new_velocity();
    float s_x = (velocity_.v_right - velocity_.v_left)*(1.0/FPS);
    float s_y = (velocity_.v_up - velocity_.v_down)*(1.0/FPS);
    Point s_vector(s_x, s_y);
    std::vector<Object*> potentially_colliding_objects = object_map_.get_potentially_colliding(*this);
    bool collision_detected = false;
    bool ground_underneath_found = false;
    float alpha = -INFINITY;
    for (Object* p : potentially_colliding_objects) {
        if (p != this && collideable_with(*p)) {
            if (collides_with_after_vector_translation(*p,s_vector)) {
                collision_detected = true;
                if (!ground_underneath_found && collides_with_top(*p,s_vector)) {
                    ground_underneath_found = true;
                    alpha = find_slope_coefficient_directly_below(*p,s_vector);
                    break; 
                }         
            }
        }
    }
    if (!collision_detected) {
        translate_object_by_vector(s_vector);
        set_scheduled(HANDLE_AIRBORNE);
    }
    else {
        acceleration_.a_up = 0.0;
        acceleration_.a_left = 0.0;
        acceleration_.a_right = 0.0;
        if (s_y > 0) {
            zero_velocity();
            zero_airborne_ghost_horizontal_velocity();
        }
        else {
            velocity_.v_left = airborne_ghost_horizontal_velocity_.v_left;
            velocity_.v_right = airborne_ghost_horizontal_velocity_.v_right;
            zero_airborne_ghost_horizontal_velocity();
            velocity_.v_up = 0.0;
            velocity_.v_down = 0.0;
        }
        if (ground_underneath_found) {
            add_ground_reaction_acceleration();
            if (alpha > -INFINITY) {
                slope_incline_directly_underneath_ = alpha;
                if (std::abs(slope_incline_directly_underneath_) > MAXIMUM_GENTLE_SLOPE_COEFFICIENT) {
                    if (slope_incline_directly_underneath_ > 0) {
                        acceleration_.a_left = object_specific_physics_char_.horizontal_acc;
                    }
                    else {
                        acceleration_.a_right = object_specific_physics_char_.horizontal_acc;
                    }
                    set_scheduled(HANDLE_SLIDE_DOWN);
                }
                else {
                    // clear_scheduled();
                    set_scheduled(HANDLE_FREEFALL);
                }
            }
            else {
                //clear_scheduled();
                set_scheduled(HANDLE_FREEFALL);
            }           
        }  
        else {
            set_scheduled(HANDLE_FREEFALL);
        }
    }
    if (s_vector.x < 0) {
        set_new_state(AIRBORNE_LEFT);
    }
    else if (s_vector.x > 0) {
        set_new_state(AIRBORNE_RIGHT);
    }
}


void MobileObject::handle_freefall() {
    single_state_persistence_timer_.freefall_timer++;
    single_state_persistence_timer_.moving_horizontally_timer = 0;
    single_state_persistence_timer_.airborne_timer = 0;
    previously_scheduled_ = HANDLE_FREEFALL;
    remove_ground_reaction_acceleration();
    new_velocity();
    float s_y = (velocity_.v_up - velocity_.v_down)*(1.0/FPS);
    Point s_vector(0.0, s_y);
    std::vector<Object*> objects_potentially_underneath = object_map_.get_potentially_underneath(*this);
    bool ground_underneath_found = false;
    float alpha = -INFINITY;
    for (Object* p : objects_potentially_underneath) {
        if (p != this && collideable_with(*p)) {
            if (collides_with_top(*p,s_vector)) {
                ground_underneath_found = true;
                alpha = find_slope_coefficient_directly_below(*p,s_vector);
                break;
            }
        }
    }
    if (ground_underneath_found) {
        add_ground_reaction_acceleration();
        acceleration_.a_left = 0.0;
        acceleration_.a_right = 0.0;
        velocity_.v_up = 0.0;
        velocity_.v_down = 0.0;
        velocity_.v_left = airborne_ghost_horizontal_velocity_.v_left;
        velocity_.v_right = airborne_ghost_horizontal_velocity_.v_right;
        zero_airborne_ghost_horizontal_velocity();
        if (alpha > -INFINITY) {
            slope_incline_directly_underneath_ = alpha;
            if (std::abs(slope_incline_directly_underneath_) > MAXIMUM_GENTLE_SLOPE_COEFFICIENT) {
                if (slope_incline_directly_underneath_ > 0) {
                    acceleration_.a_left = object_specific_physics_char_.horizontal_acc;
                }
                else {
                    acceleration_.a_right = object_specific_physics_char_.horizontal_acc;
                }
                set_scheduled(HANDLE_SLIDE_DOWN);
            }
            else {
                clear_scheduled();
            }
        }
        else {
            clear_scheduled();
        }
    }
    else {
        translate_object_by_vector(s_vector);
        set_scheduled(HANDLE_FREEFALL);
    }
    determine_new_state_for_freefall();
}


void MobileObject::remove_ground_reaction_acceleration() {
    acceleration_.a_up = 0.0;  
}


void MobileObject::add_ground_reaction_acceleration() {
    acceleration_.a_up = GRAVITATIONAL_PULL;
}


bool MobileObject::collides_with_after_vector_translation(Object& other_object, const Point& translation_vector) {
    return get_current_hitbox().collides_with_after_vector_translation(other_object.get_current_hitbox(), translation_vector);
}


bool MobileObject::is_directly_above_after_vector_translation(Object& other_object, const Point& translation_vector) {
    return get_current_hitbox().is_directly_above_after_vector_translation(other_object.get_current_hitbox(), translation_vector);
}


bool MobileObject::collides_with_top(Object& other_object, const Point& translation_vector) {
    return get_current_hitbox().collides_with_top(other_object.get_current_hitbox(), translation_vector);
}


float MobileObject::is_collision_after_vector_translation_caused_by_gentle_slope(Object& other_object, const Point& translation_vector) {
    return get_current_hitbox().is_collision_after_vector_translation_caused_by_gentle_slope(other_object.get_current_hitbox(), translation_vector);
}


float MobileObject::find_slope_coefficient_directly_below(Object& other_object, const Point& translation_vector) {
    return get_current_hitbox().find_slope_coefficient_directly_below(other_object.get_current_hitbox(), translation_vector);
}


void MobileObject::translate_object_by_vector(const Point& translation_vector) {
    set_center(get_center()+translation_vector);
}


void MobileObject::run_scheduled() {
    if (is_anything_scheduled()) {
        if (scheduled_ == HANDLE_MOVING_HORIZONTALLY) {
            handle_moving_horizontally();
        }
        else if (scheduled_ == HANDLE_STOP) {
            handle_stop();
        }
        else if (scheduled_ == HANDLE_AIRBORNE) {
            handle_airborne();
        }
        else if (scheduled_ == HANDLE_FREEFALL) {
            handle_freefall();
        }
        else if (scheduled_ == HANDLE_SLIDE_DOWN) {
            handle_slide_down();
        }
    }
}


void MobileObject::determine_new_state_for_freefall() {
    if (is_left_facing(get_previous_state())) {
        set_new_state(FREEFALL_LEFT);
    }
    else {
        set_new_state(FREEFALL_RIGHT);
    }
}


void MobileObject::set_scheduled(ScheduledInstruction new_scheduled) {
    scheduled_ = new_scheduled;
}


void MobileObject::clear_scheduled() {
    scheduled_ = NOTHING;
}


bool MobileObject::is_anything_scheduled() {
    return scheduled_ != NOTHING;
}


ScheduledInstruction MobileObject::get_previosuly_scheduled() {
    return previously_scheduled_;
}


#endif

