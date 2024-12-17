#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "object.h"

class Interactable : virtual public Object {

private:

    bool currentlyAvailable_;
    int cooldown_;
    std::pair<unsigned int, unsigned int> lastInteraction_;

public:

    Interactable(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock);

    virtual void updateLastInteraction();
    
    virtual bool getCurrentlyAvailable() const;
    void setCurrentlyAvailable(bool newCurrentlyAvailable);

    int getCooldown() const;
    void setCooldown(int newCooldown);

    virtual void performOnInteraction(void* interactionArgsVoidPtr) = 0;

};

#endif