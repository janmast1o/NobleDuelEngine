#include "object_map.h"


ObjectMap::ObjectMap(Rectangle& celledRectangle, int numOfRows, int numOfCols) : objectGridOrganizer_(celledRectangle, numOfRows, numOfCols) {;}


// ObjectMap::ObjectMap() {}


void ObjectMap::addNewObject(Object& newObject) {
    objectGridOrganizer_.emplaceInGrid(&newObject, newObject.getLargestRectangle());
    // allObjects_.push_back(&newObject);
}


void ObjectMap::updateObjectPosition(Object& object) {
    objectGridOrganizer_.emplaceInGrid(&object, object.getLargestRectangle());
}


void ObjectMap::removeObject(Object& object) {
    objectGridOrganizer_.removeFromGrid(&object);
    // allObjects_.remove(&object);
}


std::list<Object*> ObjectMap::getPotentiallyColliding(const Object& object) {
    // std::list<Object*> allCollidingObjects;
    // for (Object* p : allObjects_) {
    //     if (object.collideableWith(*p)) {
    //         allCollidingObjects.push_back(p);
    //     }
    // }
    // return allCollidingObjects;

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