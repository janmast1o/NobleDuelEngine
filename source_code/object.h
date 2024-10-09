#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <functional>
#include "structs_and_enums.h"
#include "model_collection.cpp"
#include "global_game_clock.h"

#ifndef OBJECT_H
#define OBJECT_H

namespace GameElement {
    class Object;
}

class Object {

private:

    int health_;
    SDL_Renderer* renderer_;
    Point& center_;
    ModelCollection& model_collection_;
    Matter matter_;
    State state_;
    State previous_state_;
    DamageRecState damage_rec_state_;
    DependencyState dependency_state_;

public:

    Object(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter);
    bool is_alive();
    int get_health();
    void set_health(int new_health);
    void reduce_health(int reduce_amount);
    void redraw_object();
    DependencyState get_dependency_state();
    void set_dependency_state(DependencyState new_dependency_state);
    Hitbox& get_current_hitbox();
    State get_state();
    Matter get_matter();
    void set_matter(Matter matter);
    Model& get_current_model();
    Point& get_center();
    void set_center(const Point& new_center);
    SDL_Renderer* get_renderer();

};

#endif