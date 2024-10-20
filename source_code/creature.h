#ifndef CREATURE_H
#define CREATURE_H

#include "mobile_object.cpp"
#include "item_manager.cpp"

class Item;

class Creature : public MobileObject {

private:
    
    Point targeted_point_;
    float jumping_v_;
    float regular_horizontal_acc_;
    float sprint_horizontal_acc_;
    float slow_walk_horizontal_acc_;
    float regular_max_horizontal_v_;
    float sprint_max_horizontal_v_;
    float slow_walk_max_horizontal_v_;
    ItemManager& item_manager_;

protected:
    
    std::vector<Item*> item_loadout_;
    int item_loadout_index_;
    int charge_up_frames_accumulated;
    
    void handle_moving_horizontally() override;
    void handle_jump();
    void handle_pick_up_item();
    void handle_drop_item();
    void handle_switch_to_next_item();
    void adjust_acc_and_max_v_for_regular();
    void adjust_acc_and_max_v_for_sprint();
    void adjust_acc_and_max_v_for_slow_walk();

public:
        
    Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, ItemManager& item_manager);
    Creature(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v, ItemManager& item_manager);
    void redraw_object() override;
    void update_targeted_point(Point new_targeted_point);
    void set_regular_horizontal_acc(float regular_horizontal_acc);
    void set_regular_sprint_horizontal_acc(float sprint_horizontal_acc);
    void set_slow_walk_horizontal_acc(float slow_walk_horizontal_acc);
    void set_regular_max_horizontal_v(float regular_max_horizontal_v);
    void set_sprint_max_horizontal_v(float sprint_max_horizontal_v);
    void set_slow_walk_max_horizontal_v(float slow_walk_max_horizontal_v);
    void run_scheduled() override;
    void translate_object_by_vector(const Point& translation_vector) override;
    bool is_anything_scheduled_for_item();

};

#endif
