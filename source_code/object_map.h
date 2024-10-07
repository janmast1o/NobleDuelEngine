#include <unordered_set>
#include "object.cpp"

#ifndef OBJECT_MAP_H
#define OBJECT_MAP_H

namespace GameSystem {
    class ObjectMap;
}

class ObjectMap {

private:

    std::vector<Object*> object_set_;

public:

    ObjectMap();
    ObjectMap(std::vector<Object*>& object_set);
    void add_to_map(Object& object);
    std::vector<Object*> get_potentially_colliding(Object& object);
    std::vector<Object*> get_potentially_underneath(Object& object);
    int get_number_of_objects() {return object_set_.size();}

};

#endif