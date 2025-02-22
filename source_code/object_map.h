#ifndef OBJECT_MAP
#define OBJECT_MAP

#include "object.h"
#include "grid_organizer.h"

class ObjectMap { // temporary implementation
    
private:

    GridOrganizer<Object*> objectGridOrganizer_;
    // std::list<Object*> allObjects_;

public:

    ObjectMap(const Rectangle& celledRectangle, int numOfRows, int numOfCols);
    // ObjectMap();

    void addNewObject(Object& newObject);
    void updateObjectPosition(Object& object);
    void removeObject(Object& object);

    std::list<Object*> getPotentiallyColliding(const Object& object);
    std::list<Object*> getPotentiallyColliding(const Object& object, const Point& translationVector);
    // std::list<Object*> getPotentiallyUnderneath(const Object& object) const;

};

#endif