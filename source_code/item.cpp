#include "item.h"
#include "creature.h"
#include "constants.h"

Item::Item(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
           ItemDependencyState initialDependencyState, Creature* initialOwner) :
           Object(renderer, center, modelCollection, sessionEngineClock),
           MobileObject(renderer, center, modelCollection, sessionEngineClock, objectMap, mass),
           Interactable(renderer, center, modelCollection, sessionEngineClock),
           cooldown_(FPS) {
    setMatter(LIGHT_PHANTOM);
    dependencyState_ = initialDependencyState;
    if (dependencyState_ == DEPENDENT && initialOwner != nullptr) {
        owner_ = initialOwner;
        setMatter(PHANTOM);
    } 
}


void Item::updateLastInteraction() {;}


bool Item::getCurrentlyAvailable() const {
    return dependencyState_ == INDEPENDENT;
}


void Item::updateLastUse() {
    lastUse_.first = sessionEngineClock_.cycles;
    lastUse_.second = sessionEngineClock_.framesInCycle;
}


bool Item::isAvailableForNextUse() const {
    return (sessionEngineClock_.cycles*FPS + sessionEngineClock_.framesInCycle - (lastUse_.first*FPS + lastUse_.second) > cooldown_);
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
        // std::cout << creaturePtr << std::endl;
        dependencyState_ = DEPENDENT;
        owner_ = creaturePtr; 
        setMatter(PHANTOM);
        if (creaturePtr != nullptr) {
            creaturePtr->placeItemInItemList(*this);
            // add proper state setting based on owners faced side
        }
    }
}


void Item::ridOfOwner() {
    dependencyState_ = INDEPENDENT;
    owner_ = nullptr;
    setMatter(LIGHT_PHANTOM);
}


bool Item::collideableWith(const Object& otherObject) {
    if (dependencyState_ == DEPENDENT) {
        return false;
    } else {
        return Object::collideableWith(otherObject);
    }
}


void Item::redrawObject() {
    if (dependencyState_ == INDEPENDENT) {
        Object::redrawObject();
    }
}


void Item::redrawObject(bool drawHitboxes, float pointSize) {
    if (dependencyState_ == INDEPENDENT) {
        Object::redrawObject(drawHitboxes, pointSize);
    }
}