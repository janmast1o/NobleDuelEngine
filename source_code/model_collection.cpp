#include "model_collection.h"
#include "model_cycle.h"

ModelCollection::ModelCollection() {;}


Model* ModelCollection::getCurrentModelPtrForState(State state) {
    auto x = cyclesForStates_.find(state);
    if (x != cyclesForStates_.end()) {
        return &(x->second.getCurrentModel());
    } else {
        x = cyclesForStates_.find(IDLE);
        if (x != cyclesForStates_.end()) {
            return &(x->second.getCurrentModel());
        } else {
            return nullptr;
        }
    }
}


Model* ModelCollection::getNewModelPtrForState(State state) {
    auto x = cyclesForStates_.find(state);
    if (x != cyclesForStates_.end()) {
        return &(x->second.moveAndGetModel());
    } else {
        x = cyclesForStates_.find(IDLE);
        if (x != cyclesForStates_.end()) {
            return &(x->second.moveAndGetModel());
        } else {
            return nullptr;
        }
    }
}


Model* ModelCollection::getFirstModelPtrForState(State state) {
    auto x = cyclesForStates_.find(state);
    if (x != cyclesForStates_.end()) {
        x->second.reset();
        return getCurrentModelPtrForState(state);
    } else {
        x = cyclesForStates_.find(IDLE);
        if (x != cyclesForStates_.end()) {
            x->second.reset();
            return getCurrentModelPtrForState(state);
        } else {
            return nullptr;
        }
    }
}