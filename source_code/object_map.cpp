#include "object_map.h"

ObjectMap::ObjectMap() {}


void ObjectMap::addNewObject(Object& newObject) {
    allObjects_.push_back(&newObject);
}


void ObjectMap::removeObject(Object& object) {
    allObjects_.remove(&object);
}


std::list<Object*> ObjectMap::getPotentiallyColliding(const Object& object) const {
    return allObjects_;
}


std::list<Object*> ObjectMap::getPotentiallyUnderneath(const Object& object) const {
    return allObjects_;
}