#include "mobile_object.cpp"

#ifndef CREATURE
#define CREATURE

namespace GameElement {
    class Creature;
}

class Creature : public MobileObject {

private:

    // std::vector<std::optional<Item*>> item_loadout_;
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