#include "model_collection.h"
#include "object.h"
#include <unordered_set>

ModelCollection::ModelCollection() {}


ModelCollection::ModelCollection(const ModelCollection& otherModelCollection) :
    cyclesForStates_(otherModelCollection.cyclesForStates_) {}



Rectangle ModelCollection::getLargestRectangle() const {
    return largestRectangle_;
}


void ModelCollection::determineLargestRectangle() {
    Point lowerLeft;
    Point upperRight;

    for (auto& p : cyclesForStates_) {
        for (auto& r : p.second) {
            Hitbox* hitboxPtr = r.model.getHitboxPtr();
            if (hitboxPtr != nullptr) {
                Rectangle hitboxRectangle = hitboxPtr->getRelativeRectangle();
                lowerLeft = lowerLeft.getLowerLeft(hitboxRectangle.lowerLeft);
                upperRight = upperRight.getUpperRight(hitboxRectangle.upperRight);
            }         
        }
        Rectangle collisionMeshRectangle = p.second.getCurrentCollisionMesh().getRelativeRectangle();
        lowerLeft = lowerLeft.getLowerLeft(collisionMeshRectangle.lowerLeft);
        upperRight = upperRight.getUpperRight(collisionMeshRectangle.upperRight);
    }

    largestRectangle_ = {lowerLeft, upperRight};
}


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


Hitbox& ModelCollection::getCurrentHitbox(State state) const {
    auto x = cyclesForStates_.find(state);
    if (x != cyclesForStates_.end()) {
        return x->second.getCurrentHitbox();
    } else {
        x = cyclesForStates_.find(IDLE);
        if (x != cyclesForStates_.end()) {
            return x->second.getCurrentHitbox();
        }
    }
}


Point ModelCollection::getItemGripPointRelativeToCenter(State state) const {
    auto x = cyclesForStates_.find(state);
    if (x != cyclesForStates_.end()) {
        return x->second.getItemGripPointRelativeToCenter();
    } else {
        x = cyclesForStates_.find(IDLE);
        if (x != cyclesForStates_.end()) {
            return x->second.getItemGripPointRelativeToCenter();
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


void ModelCollection::replaceAllHitboxPtrsWithStaticCopies(std::list<Hitbox>& saveContainer, Point* newOwnerCenterPtr) {
    for (auto& p : cyclesForStates_) {
        for (auto& r : p.second) {
            r.model.replaceHitboxWithAStaticCopy(saveContainer, newOwnerCenterPtr);
        }
        p.second.replaceCollisionMeshWithAStaticCopy(saveContainer, newOwnerCenterPtr);
    }
}