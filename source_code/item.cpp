#ifndef ITEM_CPP
#define ITEM_CPP

#include "item.h"
#include "creature.cpp"

Item::Item(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int uses, int cooldown) : 
    MobileObject(renderer, center, model_collection, matter, object_map), uses_(uses), cooldown_(cooldown) {
        set_matter(LIGHT_PHANTOM);
        owner_ = nullptr;
        dependency_state_ = INDEPENDENT;
        last_use_= {0, 0};
    }


Item::Item(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, DependencyState dependency_state, Creature* owner, int uses, int cooldown) : 
    MobileObject(renderer, center, model_collection, matter, object_map), dependency_state_(dependency_state), owner_(owner), uses_(uses), cooldown_(cooldown) {
        set_matter(LIGHT_PHANTOM);
        last_use_ = {0, 0};
    }


void Item::change_owner(Creature* new_owner) {
    owner_ = new_owner;
    dependency_state_ = DEPENDENT;
    set_matter(PHANTOM);
}        


void Item::discard_owner() {
    owner_ = nullptr;
    dependency_state_ = INDEPENDENT;
    set_matter(LIGHT_PHANTOM);
    set_scheduled(HANDLE_FREEFALL);
}


bool Item::available_for_use() {
    return ((current_session_global_game_clock.cycles-last_use_.first)*FPS + (current_session_global_game_clock.frames_in_cycle-last_use_.second) >= cooldown_);
}


void Item::use() {;}


void Item::charge_use() {;}


void Item::update_last_use() {
    last_use_.first = current_session_global_game_clock.cycles;
    last_use_.second = current_session_global_game_clock.frames_in_cycle;
}

#endif