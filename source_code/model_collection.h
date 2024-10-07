#include "model_cycle.cpp"
#include <unordered_map>

#ifndef MODEL_COLLECTION_H
#define MODEL_COLLECTION_H

namespace GameSystem {
    class ModelCollection;
}

class ModelCollection {

private:

    std::unordered_map<State, ModelCycle&>& cycles_for_states_;

public:

    ModelCollection(std::unordered_map<State, ModelCycle&>& cycles_for_states);
    Model& get_current_model_for_state(State state);
    Model& get_new_model_for_state(State state);
    Model& get_first_model_for_state(State state);

};


#endif