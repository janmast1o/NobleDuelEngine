#include "mobile_object.h"
#include "constants.h"

MobileObject::MobileObject(SDL_Renderer* renderer, Point center, ModelCollection modelCollection) :
    Object(renderer, center, modelCollection) {
        acceleration_.horizontalAcceleration = -GRAVITATIONAL_PULL;
        slopeInclineDirectlyUnderneath_ = 0;
        scheduled_ = HANDLE_FREEFALL;
        previouslyScheduled_ = NOTHING;
    }


float MobileObject::getMaxVerticalV() const {
    return objectSpecificPhysicsChar_.maxVerticalV;
}


void MobileObject::setMaxVerticalV(float newMaxVerticalV) {
    objectSpecificPhysicsChar_.maxVerticalV = newMaxVerticalV;
}


float MobileObject::getMaxHorizontalV() const {
    return objectSpecificPhysicsChar_.maxHorizontalV;
}


void MobileObject::setMaxHorizontalV(float newMaxHorizontalV) {
    objectSpecificPhysicsChar_.maxHorizontalV = newMaxHorizontalV;
}


float MobileObject::getHorizontalAcc() const {
    return objectSpecificPhysicsChar_.horizontalAcc;
}


void MobileObject::setHorizontalAcc(float newHorizontalAcc) {
    objectSpecificPhysicsChar_.horizontalAcc = newHorizontalAcc;
}


int MobileObject::getMaxAirborneAccelerableFrames() const {
    return objectSpecificPhysicsChar_.maxAirborneAccelerableFrames;
}


void MobileObject::setMaxAirborneAccelerableFrames(int newMaxAirborneAccelerableFrames) {
    objectSpecificPhysicsChar_.maxAirborneAccelerableFrames = newMaxAirborneAccelerableFrames;
}


void MobileObject::newHorizontalAcceleration(Direction direction) {
    if (direction == LEFT) {
        acceleration_.horizontalAcceleration = -objectSpecificPhysicsChar_.horizontalAcc;
    } else if (direction == RIGHT) {
        acceleration_.horizontalAcceleration = objectSpecificPhysicsChar_.horizontalAcc;
    }
}


void MobileObject::newVelocity() {}


int MobileObject::getFacedSideAsInt() const {} 


void MobileObject::handleMoveHorizontally() {
    singleStatePersistenceTimer_.movingHorizontallyTimer++;
    singleStatePersistenceTimer_.airborneTimer = 0;
    singleStatePersistenceTimer_.freefallTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;

    previouslyScheduled_ = scheduled_;

    newVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS);
    float sy = sx*slopeInclineDirectlyUnderneath_;
    Point svec(sx, sy);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    float alpha = -INFINITY;

    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                }
            }
        }
    } 
}