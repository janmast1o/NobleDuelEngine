#ifndef CREATURE_AND_ITEM_H
#define CREATURE_AND_ITEM_H

#include "mobile_object.h"

namespace GameElement {
    class Item;
    class Creature;
}

class Item : public MobileObject {

protected:

    DependencyState dependency_state_;
    GameElement::Creature* owner_;
    int uses_;
    int cooldown_;
    std::pair<int, int> last_use_;

public:

    Item(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int uses, int cooldown);
    Item(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, DependencyState dependency_state, GameElement::Creature* creature, int uses, int cooldown);
    void change_owner(GameElement::Creature* new_owner);
    void discard_owner();
    bool available_for_use();
    void update_last_use();
    virtual void use();
    virtual void charge_use();

};


class Creature : public MobileObject {

private:

    Point targeted_point;
    std::vector<GameElement::Item*> item_loadout_;
    int item_loadout_index_;
    float jumping_v_;
    float regular_horizontal_acc_;
    float sprint_horizontal_acc_;
    float slow_walk_horizontal_acc_;
    float regular_max_horizontal_v_;
    float sprint_max_horizontal_v_;
    float slow_walk_max_horizontal_v_;

protected:

    // std::list<MobileObject*> spawned_objects_; // add using items
    void handle_jump();
    void handle_pick_up_item();
    void handle_drop_item();
    void handle_switch_to_next_item();
    void adjust_acc_and_max_v_for_regular();
    void adjust_acc_and_max_v_for_sprint();
    void adjust_acc_and_max_v_for_slow_walk();

public:

    Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health);
    Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v);
    void set_regular_horizontal_acc(float regular_horizontal_acc);
    void set_regular_sprint_horizontal_acc(float sprint_horizontal_acc);
    void set_slow_walk_horizontal_acc(float slow_walk_horizontal_acc);
    void set_regular_max_horizontal_v(float regular_max_horizontal_v);
    void set_sprint_max_horizontal_v(float sprint_max_horizontal_v);
    void set_slow_walk_max_horizontal_v(float slow_walk_max_horizontal_v);
    void run_scheduled() override; 

};

#endif