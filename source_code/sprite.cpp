#include "sprite.h"

#ifndef SPRITE_CPP
#define SPRITE_CPP

Sprite::Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health) :
    Creature(renderer, center, model_collection, matter, object_map, health) {}


Sprite::Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, std::vector<Uint8> bufferables) :
    Creature(renderer, center, model_collection, matter, object_map, health) {
        for (Uint8 bufferable : bufferables) {
            buffered_inputs_[bufferable] = 0;
        }
    }


Sprite::Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v) :
    Creature(renderer, center, model_collection, matter, object_map, health, jumping_v) {}


Sprite::Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v, std::vector<Uint8> bufferables) :
    Creature(renderer, center, model_collection, matter, object_map, health, jumping_v) {
        for (Uint8 bufferable : bufferables) {
            buffered_inputs_[bufferable] = 0;
        }
    }    


void Sprite::add_bufferable(const Uint8 new_bufferable) {
    buffered_inputs_[new_bufferable] = 0;
}


void Sprite::remove_bufferable(const Uint8 bufferable) {
    if (buffered_inputs_.find(bufferable) != buffered_inputs_.end()) {
        buffered_inputs_.erase(bufferable);
    }
}


void Sprite::buffer_inputs(const Uint8* new_buffer) {
    for (auto pair : buffered_inputs_) {
        if (new_buffer[pair.first]) {
            buffered_inputs_[pair.first] = MAX_BUFFER_TIME+1;
        }
    }
}


void Sprite::decrement_buffer_timers() {
    for (auto pair : buffered_inputs_) {
        buffered_inputs_[pair.first]--;
        if (buffered_inputs_[pair.first] < 0) {
            buffered_inputs_[pair.first] = 0;
        }
    }
}


bool Sprite::is_requested(const Uint8 potentially_requested, const Uint8* keyboard_state) {
    if (keyboard_state[potentially_requested]) {
        if (buffered_inputs_.find(potentially_requested) != buffered_inputs_.end()) {
            buffered_inputs_[potentially_requested] = 0;
        }
        return true;
    }
    else {
        if (buffered_inputs_.find(potentially_requested) != buffered_inputs_.end() && buffered_inputs_[potentially_requested] > 0) {
            buffered_inputs_[potentially_requested] = 0;
            return true;
        }
        else {
            return false;
        }
    }
}


void Sprite::read_inputs() {
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    if (!is_anything_scheduled()) {
        if (is_requested(SDL_SCANCODE_A, keyboard_state)) {
            if (!is_requested(SDL_SCANCODE_D, keyboard_state)) {
                adjust_acc_and_max_v_for_regular();
                if (is_requested(SDL_SCANCODE_V, keyboard_state)) {
                    adjust_acc_and_max_v_for_slow_walk();
                }
                new_horizontal_acceleration(LEFT);
                if (is_requested(SDL_SCANCODE_SPACE, keyboard_state)) {
                    set_scheduled(HANDLE_JUMP);
                }
                else {
                    set_scheduled(HANDLE_MOVING_HORIZONTALLY);
                }
            }
            else {
                set_scheduled(HANDLE_STOP);
            }
        }
        else if (is_requested(SDL_SCANCODE_D, keyboard_state)) {
            if (!is_requested(SDL_SCANCODE_A, keyboard_state)) {
                adjust_acc_and_max_v_for_regular();
                if (is_requested(SDL_SCANCODE_V, keyboard_state)) {
                    adjust_acc_and_max_v_for_slow_walk();
                }
                new_horizontal_acceleration(RIGHT);
                if (is_requested(SDL_SCANCODE_SPACE, keyboard_state)) {
                    set_scheduled(HANDLE_JUMP);
                }
                else {
                    set_scheduled(HANDLE_MOVING_HORIZONTALLY);
                }
            }
            else {
                set_scheduled(HANDLE_STOP);
            }
        }
        else if (is_requested(SDL_SCANCODE_SPACE, keyboard_state)) {
            set_scheduled(HANDLE_JUMP);
        }
        else if (get_previosuly_scheduled() != HANDLE_STOP && !is_requested(SDL_SCANCODE_A, keyboard_state) && !is_requested(SDL_SCANCODE_D, keyboard_state)) {
            set_scheduled(HANDLE_STOP);
        }
    }
    else {
        buffer_inputs(keyboard_state);
    }
    decrement_buffer_timers();
}    

#endif