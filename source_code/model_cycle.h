#include "model.cpp"
#include <list>

#ifndef MODEL_CYCLE_H
#define MODEL_CYCLE_H

namespace GameSystem {
    class ModelCycle;
}

class ModelCycle {

private:

    std::list<Model>& model_list_;
    std::list<Model>::iterator it_;

public:

    ModelCycle(std::list<Model>& model_list);
    Model& get_current_model();
    void move_to_next_model();
    Model& move_and_get_model();
    void reset_model_cycle();

};

#endif