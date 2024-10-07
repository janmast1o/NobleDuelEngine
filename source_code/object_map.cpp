#include "object_map.h"

#ifndef OBJECT_MAP_CPP
#define OBJECT_MAP_CPP

ObjectMap::ObjectMap() {
    ;
}


ObjectMap::ObjectMap(std::vector<Object*>& object_set) {
    object_set_ = object_set;
}


void ObjectMap::add_to_map(Object& object) {
    object_set_.push_back(&object);
}


std::vector<Object*> ObjectMap::get_potentially_colliding(Object& object) {
    return object_set_;
}


std::vector<Object*> ObjectMap::get_potentially_underneath(Object& object) {
    return object_set_;
}

#endif