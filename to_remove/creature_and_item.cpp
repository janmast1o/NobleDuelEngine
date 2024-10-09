#ifndef CREATURE_AND_ITEM_CPP
#define CREATURE_AND_ITEM_CPP

#include "creature_and_item.h"

Item::Item(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int uses, int cooldown) : 
    MobileObject(renderer, center, model_collection, matter, object_map), uses_(uses), cooldown_(cooldown) {
        set_matter(LIGHT_PHANTOM);
        owner_ = nullptr;
        dependency_state_ = INDEPENDENT;
        last_use_= {0, 0};
    }


Item::Item(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, DependencyState dependency_state, GameElement::Creature* owner, int uses, int cooldown) : 
    MobileObject(renderer, center, model_collection, matter, object_map), dependency_state_(dependency_state), owner_(owner), uses_(uses), cooldown_(cooldown) {
        set_matter(LIGHT_PHANTOM);
        last_use_ = {0, 0};
    }


void Item::change_owner(GameElement::Creature* new_owner) {
    owner_ = new_owner;
    dependency_state_ = DEPENDENT;
    set_matter(PHANTOM);
}        


void Item::discard_owner() {
    owner_ = nullptr;
    dependency_state_ = INDEPENDENT;
    set_matter(LIGHT_PHANTOM);
}


bool Item::available_for_use() {
    return ((current_session_global_game_clock.cycles-last_use_.first)*FPS + (current_session_global_game_clock.frames_in_cycle-last_use_.second) >= cooldown_);
}


void Item::update_last_use() {
    last_use_.first = current_session_global_game_clock.cycles;
    last_use_.second = current_session_global_game_clock.frames_in_cycle;
}

Creature::Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health) :
    MobileObject(renderer, center, model_collection, matter, object_map) {
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
    }


Creature::Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v) :
    MobileObject(renderer, center, model_collection, matter, object_map) {
        item_loadout_.reserve(ITEM_LOADOUT_SIZE);
        for (int i=0; i<ITEM_LOADOUT_SIZE; i++) {
            item_loadout_[i] = nullptr;
        }
        item_loadout_index_ = 0;
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


void Creature::handle_drop_item() {
    if (item_loadout_[item_loadout_index_] != nullptr) {
        item_loadout_[item_loadout_index_]->discard_owner();
        item_loadout_[item_loadout_index_] = nullptr;
    }
}


void Creature::handle_equip_next_item() {
    item_loadout_index_ = (item_loadout_index_+1)%ITEM_LOADOUT_SIZE;
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