#include "mobile_object.cpp"

#ifndef ITEM_H
#define ITEM_H

class Creature;

class Item : public MobileObject {

protected:
    
    DependencyState dependency_state_;
    Creature* owner_;
    int uses_;
    int cooldown_;
    std::pair<int, int> last_use_;

public:
    
    Item(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int uses, int cooldown);
    Item(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, DependencyState dependency_state, Creature* creature, int uses, int cooldown);
    void redraw_object() override;
    void change_owner(Creature* new_owner);
    void discard_owner();
    const DependencyState get_dependency_state() const;
    void set_dependency_state(const DependencyState new_dependency_state);
    bool available_for_use();
    void update_last_use();
    virtual void use();
    virtual void charge_use();
    void run_scheduled() override;
    ~Item() = default;
};

#endif
