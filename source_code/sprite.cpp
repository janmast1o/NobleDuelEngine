#include "sprite.h"

#ifndef SPRITE_CPP
#define SPRITE_CPP

Sprite::Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, ItemManager& item_manager) :
    Creature(renderer, center, model_collection, matter, object_map, health, item_manager) {
        previously_item_usage_requested_ = false;
        item_usage_buffer_ = 0;
    }


Sprite::Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, ItemManager& item_manager, std::vector<Uint8> bufferables) :
    Creature(renderer, center, model_collection, matter, object_map, health, item_manager) {
        previously_item_usage_requested_ = 0;
        item_usage_buffer_ = 0;
        for (Uint8 bufferable : bufferables) {
            buffered_inputs_[bufferable] = 0;
        }
    }


Sprite::Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v, ItemManager& item_manager) :
    Creature(renderer, center, model_collection, matter, object_map, health, jumping_v, item_manager) {}


Sprite::Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v, ItemManager& item_manager, std::vector<Uint8> bufferables) :
    Creature(renderer, center, model_collection, matter, object_map, health, jumping_v, item_manager) {
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


void Sprite::buffer_inputs(const Uint8* new_buffer, const bool new_item_usage_buffer) {
    if (new_item_usage_buffer) {
        item_usage_buffer_ = MAX_BUFFER_TIME+1;
    }
    for (auto pair : buffered_inputs_) {
        if (new_buffer[pair.first]) {
            buffered_inputs_[pair.first] = MAX_BUFFER_TIME+1;
        }
    }
}


void Sprite::decrement_buffer_timers() {
    item_usage_buffer_--;
    if (item_usage_buffer_ < 0) {
        item_usage_buffer_ = 0;
    }
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


bool Sprite::is_requested_on_release(const Uint8 potentially_requested, const Uint8* keyboard_state) {
    if (!keyboard_state[potentially_requested] && previous_keyboard_state_[potentially_requested]) {
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


bool Sprite::is_item_usage_requested_on_release(const bool item_usage_requested) {
    if (!item_usage_requested) {
        if (previously_item_usage_requested_) {
            item_usage_buffer_ = 0;
            return true;
        }
        else if (item_usage_buffer_ > 0) {
            item_usage_buffer_ = 0;
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}


void Sprite::read_inputs() {
    const bool item_usage_requested = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT); 
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    if (!is_anything_scheduled()) {
        if (!is_anything_scheduled_for_item()) {
            if (item_loadout_[item_loadout_index_] != nullptr && is_item_usage_requested_on_release(item_usage_requested)) {
                if (charge_up_frames_accumulated == UNIVERSAL_CHARGE_UP_TIME) {
                    item_loadout_[item_loadout_index_]->charge_use();
                }
                else {
                    item_loadout_[item_loadout_index_]->use();
                }
                charge_up_frames_accumulated = 0;
            }
            else if (is_requested_on_release(SDL_SCANCODE_E, keyboard_state)) {
                set_scheduled(HANDLE_PICK_UP_ITEM);
            }
            else if (is_requested_on_release(SDL_SCANCODE_R, keyboard_state)) {
                set_scheduled(HANDLE_DROP_ITEM);
            }
            else if (is_requested_on_release(SDL_SCANCODE_C, keyboard_state)) {
                set_scheduled(HANDLE_SWITCH_TO_NEXT_ITEM);
            }
        }
        if (is_requested(SDL_SCANCODE_A, keyboard_state)) {
            if (!is_requested(SDL_SCANCODE_D, keyboard_state)) {
                adjust_acc_and_max_v_for_regular();
                if (is_requested(SDL_SCANCODE_V, keyboard_state)) {
                    adjust_acc_and_max_v_for_slow_walk();
                }
                else if (is_requested(SDL_SCANCODE_LSHIFT, keyboard_state)) {
                    adjust_acc_and_max_v_for_sprint();
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
                else if (is_requested(SDL_SCANCODE_LSHIFT, keyboard_state)) {
                    adjust_acc_and_max_v_for_sprint();
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
    previous_keyboard_state_[SDL_SCANCODE_E] = keyboard_state[SDL_SCANCODE_E];
    previous_keyboard_state_[SDL_SCANCODE_R] = keyboard_state[SDL_SCANCODE_R];    
    previous_keyboard_state_[SDL_SCANCODE_C] = keyboard_state[SDL_SCANCODE_C];  
    previously_item_usage_requested_ = item_usage_requested; 
    if (previously_item_usage_requested_) {
        charge_up_frames_accumulated++;
        if (charge_up_frames_accumulated > UNIVERSAL_CHARGE_UP_TIME) {
            charge_up_frames_accumulated = UNIVERSAL_CHARGE_UP_TIME;
        }
    }
    else {
        charge_up_frames_accumulated = 0;
    }
    }
    else {
        charge_up_frames_accumulated = 0;
        previously_item_usage_requested_ = false;
        previous_keyboard_state_[SDL_SCANCODE_E] = 0;
        previous_keyboard_state_[SDL_SCANCODE_R] = 0;
        previous_keyboard_state_[SDL_SCANCODE_C] = 0;
        buffer_inputs(keyboard_state, item_usage_requested);
    }
    decrement_buffer_timers();
}    

#endif