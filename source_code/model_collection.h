#ifndef MODEL_COLLECTION_H
#define MODEL_COLLECTION_H

#include <unordered_map>
#include "structs_and_enums.cpp"

class Model;
class ModelCycle;
class Object;

class ModelCollection {

private:

    std::unordered_map<State, ModelCycle> cyclesForStates_;

public:

    ModelCollection();

    Model* getCurrentModelPtrForState(State state);
    Model* getNewModelPtrForState(State state);
    Model* getFirstModelPtrForState(State state);

};

#endif