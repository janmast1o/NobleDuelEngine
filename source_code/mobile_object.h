#include "object.cpp"
#include "object_map.cpp"
#include <unordered_set>
#include <cmath>

#ifndef MOBILE_OBJECT_H
#define MOBILE_OBJECT_H

constexpr float reduced_horizontal_acc = 12.5;
constexpr float jumping_v = 345.0;
constexpr float excessive_up_acc_loss = 1.0;
constexpr float residual_horizontal_velocity_loss_rate = 0.01;

namespace GameElement {
    class MobileObject;
}

class MobileObject : public Object {

protected:

    float slope_incline_directly_underneath_;
    Velocity velocity_;
    Acceleration acceleration_;
    Velocity airborne_ghost_horizontal_velocity_;
    ObjectSpecificPhysicsChar object_specific_physics_char_;
    ObjectMap& object_map_;
    ScheduledInstruction scheduled_;
    ScheduledInstruction previously_scheduled_;
    SingleStatePersistance single_state_persistence_timer_;
    std::list<MobileObject*> spawned_objects_;

    void handle_airborne();
    void handle_freefall();
    void handle_stop();
    void handle_moving_horizontally();
    void handle_slide_down();

    void remove_ground_reaction_acceleration();
    void add_ground_reaction_acceleration();

    bool is_anything_scheduled();
    ScheduledInstruction get_previosuly_scheduled();
    void set_scheduled(ScheduledInstruction new_scheduled);
    void clear_scheduled();

    void zero_velocity();
    void zero_airborne_ghost_horizontal_velocity();

    bool collides_with_after_vector_translation(Object& other_object, const Point& translation_vector);
    bool is_directly_above_after_vector_translation(Object& other_object, const Point& translation_vector);
    bool collides_with_top(Object& other_object, const Point& translation_vector);
    float is_collision_after_vector_translation_caused_by_gentle_slope(Object& other_object, const Point& translation_vector); 
    float find_slope_coefficient_directly_below(Object& other_object, const Point& translation_vector);
    void translate_object_by_vector(const Point& translation_vector);

public:

    MobileObject(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map);
    
    void new_horizontal_acceleration(Direction direction);
    void new_horizontal_reduced_acceleration(Direction Direction);
    void new_velocity();
    
    virtual void run_scheduled();

};

#endif