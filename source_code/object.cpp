#include "object.h"
#include <iostream>
#include <vector>

#ifndef OBJECT_CPP
#define OBJECT_CPP

void DrawLargePoint(SDL_Renderer* renderer, float x, float y, float pointSize) {
    SDL_FRect rect;
    rect.x = x - pointSize / 2;  // Center the rectangle on the point
    rect.y = y - pointSize / 2;
    rect.w = pointSize;
    rect.h = pointSize;
    
    SDL_RenderFillRectF(renderer, &rect);  // Draw the filled rectangle
}


Object::Object(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter) :
    renderer_(renderer), center_(center), model_collection_(model_collection), matter_(matter) {
        health_ = INFINITY;
        state_ = IDLE;
        previous_state_ = IDLE;
        damage_rec_state_ = VULNERABLE;
        // dependency_state_ = INDEPENDENT;
    }


bool Object::collideable_with(Object& other_object) {
    if (matter_ == SOLID) {
        if (other_object.matter_ == SOLID) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (matter_ == PHANTOM) {
        if (other_object.matter_ == PHANTOM) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (matter_ == LIGHT_PHANTOM) {
        if (other_object.matter_ == PHANTOM) {
            return false;
        }
        else {
            return true;
        }
    }
    return true;
}


bool Object::is_alive() {
    return (health_ > 0);
}


int Object::get_health() {
    return health_;
}


void Object::set_health(int new_health) {
    if (new_health >= 0) {
        health_ = new_health;
    }
}


void Object::reduce_health(int reduce_amount) {
    health_ -= reduce_amount;
    if (health_ < 0) {
        health_ = 0;
    }
}


Model* Object::get_current_model() {
    return model_collection_.get_current_model_for_state(state_);
}


Model* Object::get_proper_model() {
    if (previous_state_ == state_) {
        return model_collection_.get_new_model_for_state(state_);
    }
    else {
        return model_collection_.get_first_model_for_state(state_);
    }
}


void Object::redraw_object() {
    SDL_FRect dest_rect;
    Model* current_model = get_proper_model();
    if (current_model != nullptr) {
        dest_rect.w = current_model->get_model_width();
        dest_rect.h = current_model->get_model_height();
        dest_rect.x = center_.x + current_model->get_relative_ul().x;
        dest_rect.y = -(center_.y + current_model->get_relative_ul().y);
        SDL_RenderCopyF(renderer_, current_model->get_texture(), NULL, &dest_rect);
        std::vector<Point> hull = get_current_hitbox().get_current_hull();
        for (int i=0; i<hull.size(); i++) {
            DrawLargePoint(renderer_, hull[i].x, -hull[i].y, 5);
        }
    }
}


Hitbox& Object::get_current_hitbox() {
    return get_current_model()->get_hitbox();
}


State Object::get_state() {
    return state_;
}


State Object::get_previous_state() {
    return previous_state_;
}


void Object::set_new_state(State new_state) {
    previous_state_ = state_;
    state_ = new_state;
}


Matter Object::get_matter() {
    return matter_;
}


void Object::set_matter(Matter matter) {
    matter_ = matter;
}


Point& Object::get_center() {
    return center_;
}


SDL_Renderer* Object::get_renderer() {
    return renderer_;
}


void Object::set_center(const Point& new_center) {
    center_.x = new_center.x;
    center_.y = new_center.y;
}


#endif