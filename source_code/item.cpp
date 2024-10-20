#ifndef ITEM_CPP
#define ITEM_CPP

#include "item.h"
#include "creature.h"

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


void Item::redraw_object() {
    if (dependency_state_ == INDEPENDENT) {
        // std::cout << "Y" << std::endl;
        SDL_FRect dest_rect;
        Model* current_model = get_proper_model();
        if (current_model != nullptr) {
            // std::cout << "X" << std::endl;
            dest_rect.w = current_model->get_model_width();
            dest_rect.h = current_model->get_model_height();
            dest_rect.x = get_center().x + current_model->get_relative_ul().x;
            dest_rect.y = -(get_center().y + current_model->get_relative_ul().y);
            SDL_RenderCopyF(get_renderer(), current_model->get_texture(), NULL, &dest_rect);
            std::vector<Point> hull = get_current_hitbox().get_current_hull();
            for (int i=0; i<hull.size(); i++) {
                DrawLargePoint(get_renderer(), hull[i].x, -hull[i].y, 5);
            }
        }
    }
}    


void Item::change_owner(Creature* new_owner) {
    owner_ = new_owner;
    dependency_state_ = DEPENDENT;
    set_matter(PHANTOM);
    if (owner_ != nullptr && is_left_facing(owner_->get_state())) {
        set_new_state(OWNED_LEFT);
    }
    else {
        set_new_state(OWNED_RIGHT);
    }
}        


void Item::discard_owner() {
    owner_ = nullptr;
    dependency_state_ = INDEPENDENT;
    set_matter(LIGHT_PHANTOM);
    set_scheduled(HANDLE_FREEFALL);
    set_new_state(IDLE);
}


bool Item::available_for_use() {
    return ((current_session_global_game_clock.cycles-last_use_.first)*FPS + (current_session_global_game_clock.frames_in_cycle-last_use_.second) >= cooldown_);
}


const DependencyState Item::get_dependency_state() const {
    return dependency_state_;
}


void Item::set_dependency_state(const DependencyState new_dependency_state) {
    dependency_state_ = new_dependency_state;
}


void Item::use() {;}


void Item::charge_use() {;}


void Item::update_last_use() {
    last_use_.first = current_session_global_game_clock.cycles;
    last_use_.second = current_session_global_game_clock.frames_in_cycle;
}


void Item::run_scheduled() {;} 


#endif