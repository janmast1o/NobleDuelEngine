#include "model.h"

#ifndef MODEL_CPP
#define MODEL_CPP

Model::Model(Point& center, SDL_Texture* texture, Hitbox& hitbox, float width, float height) :
    center_(center), texture_(texture), hitbox_(hitbox) {
        relative_rectangle_ = Rectangle(width, height);
    }


Model::Model(Point& center, SDL_Texture* texture, Hitbox& hitbox, float width, float height, const Point ul_relative_to_center) :
    center_(center), texture_(texture), hitbox_(hitbox) {
        relative_rectangle_ = Rectangle(Point(0,-height)+ul_relative_to_center, Point(width,0)+ul_relative_to_center);
    }


Point& Model::get_center() {
    return center_;
}


SDL_Texture* Model::get_texture() {
    return texture_;
}


Hitbox& Model::get_hitbox() {
    return hitbox_;
}


Point Model::get_relative_ll() {
    return relative_rectangle_.lower_left;
}


Point Model::get_relative_ur() {
    return relative_rectangle_.upper_right;
}


Point Model::get_relative_lr() {
    return relative_rectangle_.lower_right;
}


Point Model::get_relative_ul() {
    return relative_rectangle_.upper_left;
}


float Model::get_model_width() {
    return relative_rectangle_.upper_right.x-relative_rectangle_.lower_left.x;
}


float Model::get_model_height() {
    return relative_rectangle_.upper_right.y-relative_rectangle_.lower_left.y;
} 


#endif