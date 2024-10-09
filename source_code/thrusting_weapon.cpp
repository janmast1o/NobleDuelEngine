#include "thrusting_weapon.h"

#ifndef THRUSTING_WEAPON_CPP
#define THRUSTING_WEAPON_CPP

ThrustingWeapon::ThrustingWeapon(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int uses, int cooldown, int damage, int charge_damage, float extend_range, int attack_frame_duration, int charge_attack_frame_duration) :
    Item(renderer, center, model_collection, matter, object_map, uses, cooldown), damage_(damage), charge_damage_(charge_damage), extend_range_(extend_range), attack_frame_duration_(attack_frame_duration), charge_atttack_frame_duration_(charge_attack_frame_duration) {
        current_frame_counter_ = 0;
    }


ThrustingWeapon::ThrustingWeapon(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, DependencyState dependency_state, Creature* creature, int uses, int cooldown, int damage, int charge_damage, float extend_range, int attack_frame_duration, int charge_attack_frame_duration) :
    Item(renderer, center, model_collection, matter, object_map, dependency_state, creature, uses, cooldown), damage_(damage), charge_damage_(charge_damage), extend_range_(extend_range), attack_frame_duration_(attack_frame_duration), charge_atttack_frame_duration_(charge_attack_frame_duration) {
        current_frame_counter_ = 0;
    }


void ThrustingWeapon::use() {
    if (available_for_use()) {
        update_last_use();
        int_faced_side_during_start_up_ = owner_->get_faced_side_as_int();
        set_scheduled(HANDLE_ATTACK);
    }
}


void ThrustingWeapon::charge_use() {
    if (available_for_use()) {
        update_last_use();
        int_faced_side_during_start_up_ = owner_->get_faced_side_as_int();
        set_scheduled(HANDLE_CHARGE_ATTACK);
    }
}


void ThrustingWeapon::handle_attack() {
    previously_scheduled_ = HANDLE_ATTACK;
    if (int_faced_side_during_start_up_ == owner_->get_faced_side_as_int()) {
        get_current_hitbox().make_active();
        Point s_vector(int_faced_side_during_start_up_*(extend_range_/(float) attack_frame_duration_), 0);
        std::vector<Object*> potentially_colliding = object_map_.get_potentially_colliding(*this);
        for (Object* p : potentially_colliding) {
            if (p != this && p != owner_) {
                if (collides_with_after_vector_translation(*p,s_vector)) {
                    if (hit_registry_.find(p) != hit_registry_.end()) {
                        hit_registry_.emplace(p);
                        p->reduce_health(damage_);
                    }
                }
            }
        }
        translate_object_by_vector(s_vector);
        current_frame_counter_++;
        if (current_frame_counter_ == attack_frame_duration_) {
            hit_registry_.clear();
            set_scheduled(HANDLE_REVERT_TO_DEFAULT);
        }
        else {
            set_scheduled(HANDLE_ATTACK);
        }
    }
    else {
        hit_registry_.clear();
        set_scheduled(HANDLE_REVERT_TO_DEFAULT);
    }
}


void ThrustingWeapon::handle_revert_to_default() {
    previously_scheduled_ = HANDLE_REVERT_TO_DEFAULT;
    get_current_hitbox().make_inactive();
    if (current_frame_counter_ == 0) {
        set_center(owner_->get_center());
        clear_scheduled();
    }
    else {
        Point s_vector((int_faced_side_during_start_up_)*(extend_range_ /(float) attack_frame_duration_), 0);
        translate_object_by_vector(s_vector);
        current_frame_counter_--;
        set_scheduled(HANDLE_REVERT_TO_DEFAULT);
    }

}


void ThrustingWeapon::handle_charge_attack() {
    previously_scheduled_ = HANDLE_CHARGE_ATTACK;
    if (int_faced_side_during_start_up_ == owner_->get_faced_side_as_int()) {
        get_current_hitbox().make_active();
        Point s_vector(int_faced_side_during_start_up_*(extend_range_/(float) attack_frame_duration_), 0);
        std::vector<Object*> potentially_colliding = object_map_.get_potentially_colliding(*this);
        for (Object* p : potentially_colliding) {
            if (p != this && p != owner_) {
                if (collides_with_after_vector_translation(*p,s_vector)) {
                    if (hit_registry_.find(p) != hit_registry_.end()) {
                        hit_registry_.emplace(p);
                        p->reduce_health(damage_);
                    }
                }
            }
        }
        translate_object_by_vector(s_vector);
        current_frame_counter_++;
        if (current_frame_counter_ == attack_frame_duration_) {
            hit_registry_.clear();
            set_scheduled(HANDLE_REVERT_TO_DEFAULT);
        }
        else {
            set_scheduled(HANDLE_ATTACK);
        }
    }
    else {
        hit_registry_.clear();
        set_scheduled(HANDLE_REVERT_TO_DEFAULT);
    }
}


void ThrustingWeapon::run_scheduled() {
    if (is_anything_scheduled()) {
        if (owner_ != nullptr) {
            if (scheduled_ == HANDLE_REVERT_TO_DEFAULT) {
                handle_revert_to_default();
            }
            else if (scheduled_ == HANDLE_ATTACK) {
                handle_attack();
            }
            else if (scheduled_ == HANDLE_CHARGE_ATTACK) {
                handle_charge_attack();
            }
        }
        else {
            if (scheduled_ == HANDLE_FREEFALL) {
                handle_freefall();
            }
        }    
    }
}

#endif