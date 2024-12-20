#include "object_map.h"

ObjectMap::ObjectMap() {}


void ObjectMap::addNewObject(Object& newObject) {
    allObjects_.push_back(&newObject);
}


void ObjectMap::removeObject(Object& object) {
    allObjects_.remove(&object);
}


std::list<Object*> ObjectMap::getPotentiallyColliding(const Object& object) const {
    std::list<Object*> allCollidingObjects;
    for (Object* p : allObjects_) {
        if (object.collideableWith(*p)) {
            allCollidingObjects.push_back(p);
        }
    }
    return allCollidingObjects;
}


std::list<Object*> ObjectMap::getPotentiallyUnderneath(const Object& object) const {
    std::list<Object*> allUnderneathObjects;
    for (Object* p : allObjects_) {
        if (object.collideableWith(*p)) {
            allUnderneathObjects.push_back(p);
        }
    }
    return allUnderneathObjects;
}