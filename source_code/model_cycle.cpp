#include "model_cycle.h"
#include "model_collection.h"


ModelCycle::ModelCycle() : collisionMeshPtr_(nullptr) {
    it_ = modelList_.begin();
}


ModelCycle::ModelCycle(Hitbox* collisionMeshPtr) : collisionMeshPtr_(collisionMeshPtr) {
    it_ = modelList_.begin();
}


ModelCycle::ModelCycle(const ModelCycle& otherModelCycle) :
    modelList_(otherModelCycle.modelList_),
    collisionMeshPtr_(otherModelCycle.collisionMeshPtr_) {
        it_ = modelList_.begin();
    }


void ModelCycle::addModelAndResetIterator(Model model, int lingerOn) {
    modelList_.emplace_back(model, lingerOn);
    it_ = modelList_.begin();
}


Hitbox& ModelCycle::getCurrentCollisionMesh() const {
    return *collisionMeshPtr_;
}


Model& ModelCycle::getCurrentModel() const {
    return (*it_).model;
}


void ModelCycle::moveToNextModel() {
    ++(*it_).currentLingerCounter;
    if ((*it_).currentLingerCounter == (*it_).lingerOn) {
        (*it_).currentLingerCounter = 0;
        ++it_;
        if (it_ == modelList_.end()) {
            it_ = modelList_.begin();
        }
    }
}


Model& ModelCycle::moveAndGetModel() {
    moveToNextModel();
    return getCurrentModel();
}


void ModelCycle::reset() {
    (*it_).currentLingerCounter = 0;
    it_ = modelList_.begin();
}