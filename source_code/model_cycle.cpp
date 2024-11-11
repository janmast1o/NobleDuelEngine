#include "model_cycle.h"
#include "model_collection.h"

ModelCycle::ModelCycle(Hitbox* collisionMeshPtr) : collisionMeshPtr_(collisionMeshPtr) {
    modelCollectionContainer_ = nullptr;
    it_ = modelList_.begin();
}


void ModelCycle::addModelAndResetIterator(Model model, int lingerOn) {
    modelList_.push_back(ModelAndLingerPair(model, lingerOn));
    it_ = modelList_.begin();
}


void ModelCycle::setModelCollectionContainer(ModelCollection* newModelCollectionContainer) {
    modelCollectionContainer_ = newModelCollectionContainer;
}


Point* ModelCycle::getCurrentOwnerCenterPtr() {
    if (modelCollectionContainer_ != nullptr) {
        modelCollectionContainer_->getCurrentOwnerCenterPtr();
    } else {
        return nullptr;
    }
}


Hitbox& ModelCycle::getCurrentCollisionMesh() const {
    return *collisionMeshPtr_;
}


Model& ModelCycle::getCurrentModel() {
    return (*it_).model;
}


void ModelCycle::moveToNextModel() {
    (*it_).currentLingerCounter++;
    if ((*it_).currentLingerCounter == (*it_).lingerOn) {
        (*it_).currentLingerCounter = 0;
        it_++;
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