#include "object_map.h"


ObjectMap::ObjectMap(const Rectangle& celledRectangle, int numOfRows, int numOfCols) : objectGridOrganizer_(celledRectangle, numOfRows, numOfCols) {;}


void ObjectMap::addNewObject(Object& newObject) {
    allObjects_.emplace_back(&newObject);
    objectGridOrganizer_.emplaceInGrid(&newObject, newObject.getLargestRectangle());
}


void ObjectMap::updateObjectPosition(Object& object) {
    objectGridOrganizer_.emplaceInGrid(&object, object.getLargestRectangle());
}


void ObjectMap::removeObject(Object& object) {
    // allObjects_.erase(&object);
    objectGridOrganizer_.removeFromGrid(&object);
}


std::list<Object*> ObjectMap::getPotentiallyColliding(const Object& object) {
    std::list<Object*> allCollidingObjects = objectGridOrganizer_.getPotentiallyColliding(object.getLargestRectangle());
    for (auto it = allCollidingObjects.begin(); it != allCollidingObjects.end(); ) {
        if (!object.collideableWith(**it)) {
            it = allCollidingObjects.erase(it);
        }
        else ++it;
    }

    return allCollidingObjects;
}


std::list<Object*> ObjectMap::getPotentiallyColliding(const Object& object, const Point& translationVector) {
    std::list<Object*> allCollidingObjects = objectGridOrganizer_.getPotentiallyColliding(object.getLargestRectangle(), translationVector);
    // std::list<Object*> allCollidingObjects = allObjects_;
    for (auto it = allCollidingObjects.begin(); it != allCollidingObjects.end(); ) {
        if (!object.collideableWith(**it)) {
            it = allCollidingObjects.erase(it);
        }
        else ++it;
    }

    return allCollidingObjects;
}


// std::list<Object*> ObjectMap::getPotentiallyUnderneath(const Object& object) const {
//     std::list<Object*> allUnderneathObjects;
//     for (Object* p : allObjects_) {
//         if (object.collideableWith(*p)) {
//             allUnderneathObjects.push_back(p);
//         }
//     }
//     return allUnderneathObjects;
// }