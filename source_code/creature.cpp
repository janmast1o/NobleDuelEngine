#include "creature.h"

#ifndef CREATURE_CPP
#define CREATURE_CPP

Creature::Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health) :
    MobileObject(renderer, center, model_collection, matter, object_map) {
        // item_loadout_ = std::vector<std::optional<Item*>>(ITEM_LOADOUT_SIZE);
        jumping_v_ = 345.0;
        set_health(health);
        regular_horizontal_acc_ = 215.0;
        sprint_horizontal_acc_ = 285.0;
        slow_walk_horizontal_acc_ = 40.0;
        regular_max_horizontal_v_ = 225.0;
        sprint_max_horizontal_v_ = 275.0;
        slow_walk_max_horizontal_v_ = 45.0;
    }


Creature::Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v) :
    MobileObject(renderer, center, model_collection, matter, object_map) {
        // item_loadout_ = std::vector<std::optional<Item*>>(ITEM_LOADOUT_SIZE);
        jumping_v_ = jumping_v;
        set_health(health);
        regular_horizontal_acc_ = 215.0;
        sprint_horizontal_acc_ = 285.0;
        slow_walk_horizontal_acc_ = 40.0;
        regular_max_horizontal_v_ = 225.0;
        sprint_max_horizontal_v_ = 275.0;
        slow_walk_max_horizontal_v_ = 45.0;
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


void Creature::handle_jump() {
    previously_scheduled_ = HANDLE_JUMP;
    velocity_.v_up = jumping_v_;
    set_scheduled(HANDLE_AIRBORNE);
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
    }
}


#endif