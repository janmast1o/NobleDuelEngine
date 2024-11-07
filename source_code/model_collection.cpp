#include "model_collection.h"
#include "object.h"

ModelCollection::ModelCollection() {
    owner_ = nullptr;
}


ModelCollection::ModelCollection(const ModelCollection& otherModelCollection) :
    cyclesForStates_(otherModelCollection.cyclesForStates_), owner_(otherModelCollection.owner_) {}


void ModelCollection::addModelCycleForState(State state, ModelCycle newModelCycle) {
    cyclesForStates_[state] = newModelCycle;
    cyclesForStates_[state].setModelCollectionContainer(this);
}


void ModelCollection::setOwner(Object* newOwner) {
    owner_ = newOwner;
}


Point* ModelCollection::getCurrentOwnerCenterPtr() {
    if (owner_ != nullptr) {
        return &owner_->getCenter();
    } else {
        return nullptr;
    }
}


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