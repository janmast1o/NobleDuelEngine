#ifndef OBJECT_MAP
#define OBJECT_MAP

#include "object.h"

class ObjectMap { // temporary implementation
    
private:

    std::list<Object*> allObjects_;

public:

    ObjectMap();

    void addNewObject(Object& newObject);
    void removeObject(Object& object);

    std::list<Object*> getPotentiallyColliding(const Object& object) const;
    std::list<Object*> getPotentiallyUnderneath(const Object& object) const;

};

#endif