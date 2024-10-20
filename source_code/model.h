#include "structs_and_enums.h"
#include "hitbox.cpp"

#ifndef MODEL_H
#define MODEL_H

namespace GameSystem {
    class Model;
}

class Model {

private:

    // Object& owner_; 
    Point& center_;
    SDL_Texture* texture_;
    Hitbox& hitbox_;
    Rectangle relative_rectangle_;

public:

    Model(Point& center, SDL_Texture* texture, Hitbox& hitbox, float width, float height); // assumes center is in the exact middle of the rectangle
    Model(Point& center, SDL_Texture* texture, Hitbox& hitbox, float width, float height, const Point ul_relative_to_center);
    Point& get_center();
    SDL_Texture* get_texture();
    Hitbox& get_hitbox();
    Point get_relative_ll();
    Point get_relative_ur();
    Point get_relative_lr();
    Point get_relative_ul();
    float get_model_width();
    float get_model_height();

};


#endif