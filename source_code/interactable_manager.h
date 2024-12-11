#ifndef INTERACTABLE_MANAGER_H
#define INTERACTABLE_MANAGER_H

#include "interactable.h"

class Creature;

class InteractableManager {

private:

    std::list<Interactable*> allInteractables_;

public:

    InteractableManager();

    void addNewInteractable(Interactable& newInteractable);
    void removeInteractable(Interactable& interactable);

    std::list<Interactable*> getAllAvailableInteractables(const Point& interactionPoint, Creature& interactor);

};

#endif