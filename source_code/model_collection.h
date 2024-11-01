#ifndef MODEL_COLLECTION_H
#define MODEL_COLLECTION_H

#include <unordered_map>
#include "structs_and_enums.cpp"

class Model;
class ModelCycle;
class Object;

class ModelCollection {

private:

    Object* owner_;
    std::unordered_map<State, ModelCycle> cyclesForStates_;

public:

    ModelCollection();
    ModelCollection(const ModelCollection& otherModelCollection);

    void addModelCycleForState(State state, ModelCycle newModelCycle);
    
    void setOwner(Object* newOwner);

    Model* getCurrentModelPtrForState(State state);
    Model* getNewModelPtrForState(State state);
    Model* getFirstModelPtrForState(State state);

};

#endif