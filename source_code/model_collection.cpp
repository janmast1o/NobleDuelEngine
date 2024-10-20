#include "model_collection.h"

#ifndef MODEL_COLLECTION_CPP
#define MODEL_COLLECTION_CPP

ModelCollection::ModelCollection(std::unordered_map<State, ModelCycle&>& cycles_for_states) :
    cycles_for_states_(cycles_for_states) {}


Model* ModelCollection::get_current_model_for_state (State state) {
    auto x = cycles_for_states_.find(state);
    if (x != cycles_for_states_.end()) {
        return &(x->second.get_current_model());
    }
    else {
        x = cycles_for_states_.find(IDLE);
        if (x != cycles_for_states_.end()) {
            return &(x->second.get_current_model());
        }
        else {
            return nullptr;
        }
    }
}

Model* ModelCollection::get_new_model_for_state (State state) {
    auto x = cycles_for_states_.find(state);
    if (x != cycles_for_states_.end()) {
        return &(x->second.move_and_get_model());
    }
    else {
        x = cycles_for_states_.find(IDLE);
        if (x != cycles_for_states_.end()) {
            return &(x->second.move_and_get_model());
        }
        else {
            return nullptr;
        }
    }
}

Model* ModelCollection::get_first_model_for_state (State state) {
    auto x = cycles_for_states_.find(state);
    if (x != cycles_for_states_.end()) {
        x->second.reset_model_cycle();
        return get_current_model_for_state(state);
    }
    else {
        x = cycles_for_states_.find(IDLE);
        if (x != cycles_for_states_.end()) {
            x->second.reset_model_cycle();
            return get_first_model_for_state(IDLE);
        }
        else {
            return nullptr;
        }
    }
}

#endif