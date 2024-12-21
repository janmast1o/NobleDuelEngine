#include "consumable.h"
#include "creature.h"

Consumable::Consumable(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
                       std::function<void(void*)> onConsumptionFunction, float raiseRange,
                       unsigned short approachEffectFrames, unsigned short recoveryFrames,
                       ItemDependencyState initialDependencyState, Creature* initialOwner) : 
                       Object(renderer, center, modelCollection, sessionEngineClock),
                       Item(renderer, center, modelCollection, sessionEngineClock, objectMap, mass, initialDependencyState, initialOwner),
                       onConsumptionFunction_(onConsumptionFunction), 
                       raiseRange_(raiseRange),
                       approachEffectFrames_(approachEffectFrames),
                       recoveryFrames_(recoveryFrames) {}


void Consumable::handleConsume() {
    previouslyScheduled_ = scheduled_;
    float sy = raiseRange_ / ((float) (approachEffectFrames_));
    currentFrameCounter_++;
    translateObjectByVector({0, sy});
    if (currentFrameCounter_ == approachEffectFrames_) {
        currentFrameCounter_ = 0;
        onConsumptionFunction_((void*) owner_);
        setScheduled(HANDLE_RECOVER);
    } else {
        setScheduled(HANDLE_USE);
    }

    setNewState(IN_USE);
}


void Consumable::handleRecover() {
    previouslyScheduled_ = scheduled_;
    float sy = -raiseRange_ / ((float) recoveryFrames_);
    currentFrameCounter_++;
    translateObjectByVector({0, -sy});
    if (currentFrameCounter_ == recoveryFrames_) {
        setCenter(owner_->getCurrentItemGripPoint());
        currentFrameCounter_ = 0;
        clearScheduled();
    } else {
        setScheduled(HANDLE_RECOVER);
    }

    setNewState(IN_USE);
}



void Consumable::use() {
    if (isAvailableForNextUse()) {
        updateLastUse();
        setScheduled(HANDLE_USE);
    }        
}


void Consumable::alternativeUse() {;}


void Consumable::runScheduled() {
    if (dependencyState_ == INDEPENDENT) {
        MobileObject::runScheduled();
    } else if (isAnythingScheduled()) {
        switch (scheduled_) {
            case HANDLE_USE:
                handleConsume();
                break;
            case HANDLE_RECOVER:
                handleRecover();
                break;
            default:
                clearScheduled();
                break;        
        }
    }
}