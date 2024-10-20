#ifndef CREATURE_CPP
#define CREATURE_CPP

#include "creature.h"
#include "item.h"

Creature::Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, ItemManager& item_manager) :
    MobileObject(renderer, center, model_collection, matter, object_map), item_manager_(item_manager) {
        item_loadout_.reserve(ITEM_LOADOUT_SIZE);
        for (int i=0; i<ITEM_LOADOUT_SIZE; i++) {
            item_loadout_[i] = nullptr;
        }
        item_loadout_index_ = 0;
        jumping_v_ = 345.0;
        set_health(health);
        regular_horizontal_acc_ = 215.0;
        sprint_horizontal_acc_ = 285.0;
        slow_walk_horizontal_acc_ = 40.0;
        regular_max_horizontal_v_ = 225.0;
        sprint_max_horizontal_v_ = 275.0;
        slow_walk_max_horizontal_v_ = 45.0;
        item_manager_ = item_manager;
        charge_up_frames_accumulated = 0;
    }


Creature::Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v, ItemManager& item_manager) :
    MobileObject(renderer, center, model_collection, matter, object_map), item_manager_(item_manager) {
        item_loadout_.reserve(ITEM_LOADOUT_SIZE);
        for (int i=0; i<ITEM_LOADOUT_SIZE; i++) {
            item_loadout_[i] = nullptr;
        }
        item_loadout_index_ = 0;
        jumping_v_ = 345.0;
        set_health(health);
        regular_horizontal_acc_ = 215.0;
        sprint_horizontal_acc_ = 355.0;
        slow_walk_horizontal_acc_ = 40.0;
        regular_max_horizontal_v_ = 225.0;
        sprint_max_horizontal_v_ = 425.0;
        slow_walk_max_horizontal_v_ = 45.0;
        item_manager_ = item_manager;
        charge_up_frames_accumulated = 0;
    }


void Creature::translate_object_by_vector(const Point& translation_vector) {
    set_center(get_center()+translation_vector);
    if (item_loadout_[item_loadout_index_] != nullptr) {
        if (!is_anything_scheduled_for_item()) {
            item_loadout_[item_loadout_index_]->set_center(get_center()+translation_vector);
            if (translation_vector.x < 0) {
                item_loadout_[item_loadout_index_]->set_new_state(OWNED_LEFT);
            }
            else if (translation_vector.x > 0) {
                item_loadout_[item_loadout_index_]->set_new_state(OWNED_RIGHT);
            }
        }
        else {
            item_loadout_[item_loadout_index_]->set_center(item_loadout_[item_loadout_index_]->get_center()+translation_vector);
        }
    }
}


void Creature::redraw_object() {
    SDL_FRect dest_rect;
    Model* current_model = get_proper_model();
    if (current_model != nullptr) {
        dest_rect.w = current_model->get_model_width();
        dest_rect.h = current_model->get_model_height();
        dest_rect.x = get_center().x + current_model->get_relative_ul().x;
        dest_rect.y = -(get_center().y + current_model->get_relative_ul().y);
        SDL_RenderCopyF(get_renderer(), current_model->get_texture(), NULL, &dest_rect);
        std::vector<Point> hull = get_current_hitbox().get_current_hull();
        for (int i=0; i<hull.size(); i++) {
            DrawLargePoint(get_renderer(), hull[i].x, -hull[i].y, 5);
        }
        if (item_loadout_[item_loadout_index_] != nullptr) {
            item_loadout_[item_loadout_index_]->set_dependency_state(INDEPENDENT);
            item_loadout_[item_loadout_index_]->redraw_object();
            item_loadout_[item_loadout_index_]->set_dependency_state(DEPENDENT);
        }
    } 
}


void Creature::adjust_acc_and_max_v_for_regular() {
    object_specific_physics_char_.horizontal_acc = regular_horizontal_acc_;
    object_specific_physics_char_.max_horizontal_v = regular_max_horizontal_v_;
}


void Creature::adjust_acc_and_max_v_for_sprint() {
    object_specific_physics_char_.horizontal_acc = sprint_horizontal_acc_;
    object_specific_physics_char_.max_horizontal_v = sprint_max_horizontal_v_;
}


void Creature::adjust_acc_and_max_v_for_slow_walk() {
    object_specific_physics_char_.horizontal_acc = slow_walk_horizontal_acc_;
    object_specific_physics_char_.max_horizontal_v = slow_walk_max_horizontal_v_;
}


void Creature::update_targeted_point(Point new_targeted_point) {
    targeted_point_ = new_targeted_point;
}


void Creature::handle_moving_horizontally() {
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
        if (std::abs(acceleration_.a_left-slow_walk_horizontal_acc_) < ERROR_EPS) {
            set_new_state(SLOWLY_M_LEFT);
        }
        else if (std::abs(acceleration_.a_left-regular_horizontal_acc_) < ERROR_EPS) {
            set_new_state(MOVING_LEFT);
        }
        else if (std::abs(acceleration_.a_left-slow_walk_horizontal_acc_) < ERROR_EPS) {
            set_new_state(QUICKLY_M_LEFT);
        }
    }
    else if (s_vector.x > 0) {
        if (std::abs(acceleration_.a_right-slow_walk_horizontal_acc_) < ERROR_EPS) {
            set_new_state(SLOWLY_M_RIGHT);
        }
        else if (std::abs(acceleration_.a_right-regular_horizontal_acc_) < ERROR_EPS) {
            set_new_state(MOVING_RIGHT);
        }
        else if (std::abs(acceleration_.a_right-slow_walk_horizontal_acc_) < ERROR_EPS) {
            set_new_state(QUICKLY_M_RIGHT);
        }
    } 
}


void Creature::handle_jump() {
    previously_scheduled_ = HANDLE_JUMP;
    velocity_.v_up = jumping_v_;
    set_scheduled(HANDLE_AIRBORNE);
}


void Creature::handle_drop_item() {
    if (item_loadout_[item_loadout_index_] != nullptr) {
        item_loadout_[item_loadout_index_]->discard_owner();
        item_loadout_[item_loadout_index_] = nullptr;
    }
    clear_scheduled();
}


void Creature::handle_pick_up_item() {
    std::vector<Item*> items_nearby = item_manager_.get_nearby_items(targeted_point_);
    for (Item* p : items_nearby) {
        if (std::sqrt(std::pow(p->get_center().x-get_center().x, 2)+std::pow(p->get_center().y-get_center().y, 2)) <= MAX_ITEM_GRAB_RANGE) {
            if (std::sqrt(std::pow(p->get_center().x-targeted_point_.x, 2)+std::pow(p->get_center().y-targeted_point_.y, 2)) <= MAX_ITEM_GRAB_RANGE) {
                if (item_loadout_[item_loadout_index_] != nullptr) {
                    item_loadout_[item_loadout_index_]->discard_owner();
                }
                p->change_owner(this);
                item_loadout_[item_loadout_index_] = p;
                break;
            }
        }
    }
    clear_scheduled();
}


void Creature::handle_switch_to_next_item() {
    item_loadout_index_ = (item_loadout_index_+1)%ITEM_LOADOUT_SIZE;
    clear_scheduled();
}


void Creature::set_regular_horizontal_acc(float regular_horizontal_acc) {
    regular_horizontal_acc_ = regular_horizontal_acc;
}


void Creature::set_regular_sprint_horizontal_acc(float sprint_horizontal_acc) {
    sprint_horizontal_acc_ = sprint_horizontal_acc;
}


void Creature::set_slow_walk_horizontal_acc(float slow_walk_horizontal_acc) {
    slow_walk_horizontal_acc_ = slow_walk_horizontal_acc;
}


void Creature::set_regular_max_horizontal_v(float regular_max_horizontal_v) {
    regular_max_horizontal_v_ = regular_max_horizontal_v;
}


void Creature::set_sprint_max_horizontal_v(float sprint_max_horizontal_v) {
    sprint_max_horizontal_v_ = sprint_max_horizontal_v;
}


void Creature::set_slow_walk_max_horizontal_v(float slow_walk_max_horizontal_v) {
    slow_walk_max_horizontal_v_ = slow_walk_max_horizontal_v;
}


void Creature::run_scheduled() {
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
        else if (scheduled_ == HANDLE_JUMP) {
            handle_jump();
        }
        else if (scheduled_ == HANDLE_PICK_UP_ITEM) {
            handle_pick_up_item();
        }
        else if (scheduled_ == HANDLE_DROP_ITEM) {
            handle_drop_item();
        }
        else if (scheduled_ == HANDLE_SWITCH_TO_NEXT_ITEM) {
            handle_switch_to_next_item();
        }
    }
}


bool Creature::is_anything_scheduled_for_item() {
    return (item_loadout_[item_loadout_index_] != nullptr && item_loadout_[item_loadout_index_]->is_anything_scheduled());
}

#endif