#include "item.h"
#include "creature.h"
#include "constants.h"
#include "utility_functions.h"

Item::Item(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
           ItemDependencyState initialDependencyState, Creature* initialOwner) :
           Object(renderer, center, modelCollection, sessionEngineClock),
           MobileObject(renderer, center, modelCollection, sessionEngineClock, objectMap, mass),
           Interactable(renderer, center, modelCollection, sessionEngineClock),
           cooldown_(FPS/2),
           staminaDrainedOnUse_(0),
           staminaDrainedOnAlternativeUse_(0) {
    setMatter(LIGHT_PHANTOM);
    dependencyState_ = initialDependencyState;
    if (dependencyState_ == DEPENDENT && initialOwner != nullptr) {
        owner_ = initialOwner;
        setMatter(PHANTOM);
    } 
}


void Item::temporarilySetIndependentForRedrawing() {
    dependencyState_ = TEMP_INDEPENDENT;
}


bool Item::isParticipatingInCollisions() const {
    return dependencyState_ == INDEPENDENT;
}


void Item::setNewState(State newState) {
    if (dependencyState_ == DEPENDENT && isAnythingScheduled()) return;
    Object::setNewState(newState);
}


void Item::updateLastInteraction() {;}


bool Item::getCurrentlyAvailable() const {
    return dependencyState_ == INDEPENDENT;
}


void Item::updateLastUse() {
    lastUse_.first = sessionEngineClock_.cycles;
    lastUse_.second = sessionEngineClock_.framesInCurrentCycle;
}


bool Item::isAvailableForNextUse() const {
    return (sessionEngineClock_.getCurrentTimeInFrames() - (lastUse_.first*FPS + lastUse_.second) > cooldown_);
}


int Item::getStaminaDrainedOnUse() const {
    return staminaDrainedOnUse_;
}


void Item::setStaminaDrainedOnUse(int newStaminaDrainedOnUse) {
    staminaDrainedOnUse_ = newStaminaDrainedOnUse;
}


int Item::getStaminaDrainedOnAlternativeUse() const {
    return staminaDrainedOnAlternativeUse_;
}


void Item::setStaminaDrainedOnAlternativeUse(int newStaminaDrainedOnAlternativeUse) {
    staminaDrainedOnAlternativeUse_ = newStaminaDrainedOnAlternativeUse;
}


unsigned int Item::getCooldown() const {
    return cooldown_;
}


void Item::setCooldown(unsigned int newCooldown) {
    cooldown_ = newCooldown;
}


Creature* Item::getOwner() const {
    return owner_;
}


void Item::performOnInteraction(void* interactionArgsVoidPtr) {
    if (dependencyState_ == INDEPENDENT) {
        Creature* creaturePtr = (Creature*) interactionArgsVoidPtr;
        dependencyState_ = DEPENDENT;
        owner_ = creaturePtr; 
        setMatter(PHANTOM);
        if (creaturePtr != nullptr) {
            creaturePtr->placeItemInItemList(*this);
            setCenter(creaturePtr->getCurrentItemGripPoint());
            if (isLeftFacing(creaturePtr->getState())) {
                setNewState(OWNED_LEFT);
            } else {
                setNewState(OWNED_RIGHT);
            }
        }
    }
}


void Item::ridOfOwner() {
    dependencyState_ = INDEPENDENT;
    owner_ = nullptr;
    setMatter(LIGHT_PHANTOM);
}


bool Item::collideableWith(const Object& otherObject) const {
    if (!isParticipatingInCollisions()) return false;
    return Object::collideableWith(otherObject);
}


bool Item::collideableWith(const Object& otherObject) {
    return static_cast<const Item*>(this)->collideableWith(otherObject);
}


void Item::redrawObject() {
    if (dependencyState_ == INDEPENDENT) {
        Object::redrawObject();
    } else if (dependencyState_ == TEMP_INDEPENDENT) {
        Object::redrawObject();
        dependencyState_ = DEPENDENT;
    }
}


void Item::redrawObject(bool drawHitboxes, float pointSize) {
    if (dependencyState_ == INDEPENDENT) {
        Object::redrawObject(drawHitboxes, pointSize);
    } else if (dependencyState_ == TEMP_INDEPENDENT) {
        Object::redrawObject(drawHitboxes, pointSize);
        dependencyState_ = DEPENDENT;
    }
}


void Item::redrawObject(const Rectangle& currentlyObservedRectangle) {
    if (dependencyState_ == INDEPENDENT) {
        Object::redrawObject(currentlyObservedRectangle);
    } else if (dependencyState_ == TEMP_INDEPENDENT) {
        Object::redrawObject(currentlyObservedRectangle);
        dependencyState_ = DEPENDENT;
    }
}