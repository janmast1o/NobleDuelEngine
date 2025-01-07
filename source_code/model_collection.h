#ifndef MODEL_COLLECTION_H
#define MODEL_COLLECTION_H

#include <unordered_map>
#include "structs_and_enums.h"
#include "model_cycle.h"

class Object;

class ModelCollection {

private:

    Rectangle largestRectangle_;
    std::unordered_map<State, ModelCycle> cyclesForStates_;

public:

    ModelCollection();
    ModelCollection(const ModelCollection& otherModelCollection);

    Rectangle getLargestRectangle() const;
    void determineLargestRectangle();
    void registerOwnerCenterPtrForHitboxes(Point* ownerCenterPtr);
    
    void addModelCycleForState(State state, ModelCycle newModelCycle);

    Hitbox& getCurrentCollisionMesh(State state) const;
    Hitbox& getCurrentHitbox(State state) const;

    Point getItemGripPointRelativeToCenter(State state) const;

    Model* getCurrentModelPtrForState(State state);
    Model* getNewModelPtrForState(State state);
    Model* getFirstModelPtrForState(State state);

    std::list<int> getAllTiedHitboxes() const;
    void replaceAllHitboxPtrsWithStaticCopies(std::list<Hitbox>& saveContainer, Point* newOwnerCenterPtr);

};

#endif