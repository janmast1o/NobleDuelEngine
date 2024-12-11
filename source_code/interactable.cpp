#include "interactable.h"

Interactable::Interactable(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock) :
    Object(renderer, center, modelCollection, sessionEngineClock) {
        currentlyAvailable_ = true;
        setMatter(TRUEST_OF_PHANTOMS);
        cooldown_ = 120;
        lastUse_ = {0, 0};
    }


bool Interactable::getCurrentlyAvailable() const {
    return currentlyAvailable_;
}


void Interactable::setCurrentlyAvailable(bool newCurrentlyAvailable) {
    currentlyAvailable_ = newCurrentlyAvailable;
}


int Interactable::getCooldown() const {
    return cooldown_;
}    


void Interactable::setCooldown(int newCooldown) {
    cooldown_ = newCooldown;
}