#include "model_collection.h"
#include "object.h"
#include <unordered_set>

ModelCollection::ModelCollection() {}


ModelCollection::ModelCollection(const ModelCollection& otherModelCollection) :
    cyclesForStates_(otherModelCollection.cyclesForStates_) {}



void ModelCollection::registerOwnerCenterPtrForHitboxes(Point* ownerCenterPtr) {
    std::unordered_map<int, Hitbox*> allTiedHitboxesMap;
    for (auto& p : cyclesForStates_) {
        for (auto& r : p.second) {
            Hitbox* hitboxPtr = r.model.getHitboxPtr();
            if (hitboxPtr != nullptr) {
                allTiedHitboxesMap.emplace(r.model.getHitboxPtr()->getId(), hitboxPtr);
            }         
        }
        allTiedHitboxesMap.emplace(p.second.getCurrentCollisionMesh().getId(), &p.second.getCurrentCollisionMesh());
    }
    for (auto& p : allTiedHitboxesMap) {
        p.second->setOwnerCenterPtr(ownerCenterPtr);
    }
}


void ModelCollection::addModelCycleForState(State state, ModelCycle newModelCycle) {
    cyclesForStates_.emplace(state, newModelCycle);
}


Hitbox& ModelCollection::getCurrentCollisionMesh(State state) const {
    auto x = cyclesForStates_.find(state);
    if (x != cyclesForStates_.end()) {
        return x->second.getCurrentCollisionMesh();
    } else {
        x = cyclesForStates_.find(IDLE);
        if (x != cyclesForStates_.end()) {
            return x->second.getCurrentCollisionMesh();
        }    
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


std::list<int> ModelCollection::getAllTiedHitboxes() const {
    std::unordered_set<int> allTiedHitboxesSet;
    for (auto& p : cyclesForStates_) {
        for (auto& r : p.second) {
            Hitbox* hitboxPtr = r.model.getHitboxPtr();
            if (hitboxPtr != nullptr) {
                allTiedHitboxesSet.emplace(r.model.getHitboxPtr()->getId());
            }         
        }
        allTiedHitboxesSet.emplace(p.second.getCurrentCollisionMesh().getId());
    }
    return std::list<int>(allTiedHitboxesSet.begin(), allTiedHitboxesSet.end());
}