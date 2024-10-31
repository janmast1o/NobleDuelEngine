#include "model_cycle.h"

ModelCycle::ModelCycle() {
    modelCollectionContainer_ = nullptr;
    it_ = modelList_.begin();
}


void ModelCycle::addModelAndResetIterator(Model model, int lingerOn) {
    modelList_.push_back(ModelAndLingerPair(model, lingerOn));
    it_ = modelList_.begin();
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