#ifndef MODEL_CYCLE_H
#define MODEL_CYCLE_H

#include <list>
#include "model.h"

class ModelCollection;

class ModelCycle {

private:

    struct ModelAndLingerPair {
        Model model;
        int lingerOn;
        int currentLingerCounter;

        ModelAndLingerPair(Model model, int lingerOn) : model(model), lingerOn(lingerOn), currentLingerCounter(0) {}
    };

    ModelCollection* modelCollectionContainer_;
    std::list<ModelAndLingerPair> modelList_;
    std::list<ModelAndLingerPair>::iterator it_;

public:

    ModelCycle();

    void addModelAndResetIterator(Model model, int lingerOn);

    Model& getCurrentModel();
    void moveToNextModel();
    Model& moveAndGetModel();
    void reset();

};

#endif