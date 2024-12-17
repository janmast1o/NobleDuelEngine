#include "interactable_manager.h"
#include "creature.h"
#include "constants.h"


InteractableManager::InteractableManager() {;}


void InteractableManager::addNewInteractable(Interactable& newInteractable) {
    allInteractables_.emplace_back(&newInteractable);
}


void InteractableManager::removeInteractable(Interactable& interactable) {
    allInteractables_.remove(&interactable);
}


std::list<Interactable*> InteractableManager::getAllAvailableInteractables(const Point& interactionPoint, Creature& interactor) {
    std::list<Interactable*> foundAvailableInteractables;
    for (Interactable* i : allInteractables_) {
        // std::cout << i << std::endl;
        if (i->getCurrentlyAvailable() &&
            i->getCenter().distanceFromOther(interactionPoint) < MAX_INTERACTION_POINT_DIS &&
            i->getCenter().distanceFromOther(interactor.getCenter()) < MAX_INTERACTOR_DIS) {
                foundAvailableInteractables.emplace_back(i);
            }
    }

    return foundAvailableInteractables;
}