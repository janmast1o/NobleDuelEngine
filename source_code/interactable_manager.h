#ifndef INTERACTABLE_MANAGER_H
#define INTERACTABLE_MANAGER_H

#include "interactable.h"
#include "grid_organizer.h"

class Creature;

class InteractableManager {

private:

    GridOrganizer<Interactable*> interactableGridOrganizer_;
    std::list<Interactable*> allInteractables_;

public:

    InteractableManager(Rectangle& celledRectangle, int numOfRows, int numOfCols);
    // InteractableManager();

    void addNewInteractable(Interactable& newInteractable);
    // void updateInteractablePosition(Interactable& newInteractable);
    void removeInteractable(Interactable& interactable);

    std::list<Interactable*> getAllAvailableInteractables(const Point& interactionPoint, Creature& interactor);

};

#endif