#ifndef HITBOX_CONTAINER
#define HITBOX_CONTAINER

#include "structs_and_enums.h"

class HitboxContainer {

public:

    virtual Point* getCurrentOwnerCenterPtr() = 0;

};

#endif