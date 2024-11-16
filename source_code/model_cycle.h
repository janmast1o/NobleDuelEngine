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
    
        ModelAndLingerPair(const ModelAndLingerPair& otherModelAndLingerPair) :
            model(otherModelAndLingerPair.model), 
            lingerOn(otherModelAndLingerPair.lingerOn), 
            currentLingerCounter(otherModelAndLingerPair.currentLingerCounter) {}

    };

    std::list<ModelAndLingerPair> modelList_;
    std::list<ModelAndLingerPair>::iterator it_;
    Hitbox* collisionMeshPtr_;

public:

    ModelCycle();
    ModelCycle(Hitbox* collisionMesh);
    ModelCycle(const ModelCycle& otherModelCycle);

    auto begin() {return modelList_.begin();}
    auto end() {return modelList_.end();}

    auto begin() const {return modelList_.begin();}
    auto end() const {return modelList_.end();}

    void addModelAndResetIterator(Model model, int lingerOn);

    Hitbox& getCurrentCollisionMesh() const;

    Model& getCurrentModel() const;
    void moveToNextModel();
    Model& moveAndGetModel();
    void reset();

};

#endif