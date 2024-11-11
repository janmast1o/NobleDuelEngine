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

    ModelCollection* modelCollectionContainer_;
    std::list<ModelAndLingerPair> modelList_;
    std::list<ModelAndLingerPair>::iterator it_;
    Hitbox* collisionMeshPtr_;

public:

    ModelCycle(Hitbox* collisionMesh);

    void addModelAndResetIterator(Model model, int lingerOn);

    void setModelCollectionContainer(ModelCollection* newModelCollectionContainer);

    Point* getCurrentOwnerCenterPtr();

    Hitbox& getCurrentCollisionMesh() const;

    Model& getCurrentModel();
    void moveToNextModel();
    Model& moveAndGetModel();
    void reset();

};

#endif