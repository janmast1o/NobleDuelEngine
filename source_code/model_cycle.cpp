#include "model_cycle.h"

#ifndef MODEL_CYCLE_CPP
#define MODEL_CYCLE_CPP

ModelCycle::ModelCycle(std::list<Model>& model_list) : model_list_(model_list) {
    it_ = model_list_.begin();
}

Model& ModelCycle::get_current_model() {
    return *it_;
}

void ModelCycle::move_to_next_model() {
    if (it_ == model_list_.end()) {
        it_ = model_list_.begin();
    }
    else {
        it_++;
    }
}

Model& ModelCycle::move_and_get_model() {
    move_to_next_model();
    return get_current_model();
}


void ModelCycle::reset_model_cycle() {
    it_ = model_list_.begin();
}

#endif