#include "item.cpp"

#ifndef THRUSTING_WEAPON_H
#define THRUSTING_WEAPON_H

namespace GameElement {
    class ThrustingWeapon;
}

class ThrustingWeapon : public Item {

private:

    int damage_;
    int charge_damage_;
    float extend_range_;
    int attack_frame_duration_;
    int charge_atttack_frame_duration_;
    int current_frame_counter_;
    int int_faced_side_during_start_up_;
    std::unordered_set<Object*> hit_registry_;

protected:

    void handle_revert_to_default();
    void handle_attack();
    void handle_charge_attack();

public:

    ThrustingWeapon(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int uses, int cooldown, int damage, int charge_damage, float extend_range, int attack_frame_duration, int charge_attack_frame_duration);
    ThrustingWeapon(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, DependencyState dependency_state, Creature* creature, int uses, int cooldown, int damage, int charge_damage, float extend_range, int attack_frame_duration, int charge_attack_frame_duration);
    void use() override;
    void charge_use() override;
    void run_scheduled() override;

};

#endif