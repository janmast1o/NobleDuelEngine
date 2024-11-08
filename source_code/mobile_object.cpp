#include "mobile_object.h"
#include "constants.h"
#include "utility_functions.cpp"
#include <cmath>

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


void MobileObject::removeGroundReactionAcceleration() {
    acceleration_.veticalAcceleration = GRAVITATIONAL_PULL;
}


void MobileObject::addGroundReactionAcceleration() {
    acceleration_.veticalAcceleration = 0;
}


void MobileObject::handleMoveHorizontally() { // changes made to try and acheieve smoother slope traversal
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
    float beta = -INFINITY;

    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                } else {
                    groundUnderneathFound = true;
                    alpha = isCollisionAfterVectorTranslationCausedByGentleSlope(*p, svec);
                    if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                        svec.y = alpha*sx;
                    } else {
                        collisionDetected = true;
                    }
                }
            } else if (!groundUnderneathFound) {
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                } else {
                    float d = getVerticalDistanceToObjectBelowAfterVectorTranslation(*p, svec); // attempting to fix awkward freefalls during slope changing while descending
                    if (d <= svec.y+MAXIMUM_GENTLE_SLOPE_COEFFICIENT*sx) {
                        Point dvec(sx, svec.y-d);
                        float delta = isCollisionAfterVectorTranslationCausedByGentleSlope(*p, dvec);
                        if (std::abs(delta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                            if (std::abs(beta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS && std::abs(delta) > std::abs(beta)) { // potentially problematic part
                                groundUnderneathFound = true;
                                beta = delta;
                                svec.y = beta*sx;
                            }
                        }
                    }
                }
            }
        }
    }

    if (!collisionDetected) {
        translateObjectByVector(svec);
        if (!groundUnderneathFound) {
            acceleration_.horizontalAcceleration = 0;
            airborneGhostHorizontalVelocity_.horizontalVelocity = velocity_.horizontalVelocity;
            velocity_.horizontalVelocity = 0;
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_FREEFALL);
        } else {
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                slopeInclineDirectlyUnderneath_ = alpha;
            } else if (std::abs(beta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) { // another potentially problematic
                slopeInclineDirectlyUnderneath_ = beta;
            }
            clearScheduled();
        }
    } else {
        clearScheduled();
    }

    if (svec.x < 0) {
        setNewState(MOVING_LEFT);
    } else if (svec.x > 0) {
        setNewState(MOVING_RIGHT);
    }
}


void MobileObject::handleSlideDown() {
    singleStatePersistenceTimer_.slideDownTimer++;
    singleStatePersistenceTimer_.movingHorizontallyTimer = 0;
    singleStatePersistenceTimer_.airborneTimer = 0;
    singleStatePersistenceTimer_.freefallTimer = 0;

    previouslyScheduled_ = scheduled_;

    newVelocity();
    float a = slopeInclineDirectlyUnderneath_;
    float gamma = std::atan(std::abs(a));
    float b = velocity_.verticalVelocity*(1.0/FPS);
    float sx = getSign(-a)*std::abs(b*std::sin(gamma)*std::cos(gamma));
    float sy = -std::abs(b*std::pow(std::sin(gamma), 2));
    Point svec(sx, sy);
    std::list<Object*> potentiallyCollding = objectMap_.getPotentiallyColliding(*this);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    float alpha = -INFINITY;

    for (Object* p : potentiallyCollding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                } else {
                    float delta = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
                    Point dvec(sx, -std::abs(delta*sx));
                    if (isDirectlyAboveAfterVectorTranslation(*p, dvec)) {
                        groundUnderneathFound = true;
                        if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS && std::abs(delta) < std::abs(alpha)) {
                            svec.y = dvec.y;
                            alpha = delta;
                        }
                    } else {
                        collisionDetected = true;
                        break;
                    }

                }
            } else if (!groundUnderneathFound && isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                groundUnderneathFound = true;
            }
        }
    }

    if (!collisionDetected) {
        translateObjectByVector(svec);
        if (!groundUnderneathFound) {
            slopeInclineDirectlyUnderneath_ = 0;
            acceleration_.horizontalAcceleration = 0;
            removeGroundReactionAcceleration();
            airborneGhostHorizontalVelocity_.horizontalVelocity = velocity_.horizontalVelocity;
            velocity_.horizontalVelocity = 0;
            setScheduled(HANDLE_FREEFALL);
        } else {
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                slopeInclineDirectlyUnderneath_ = alpha;
                clearScheduled();
            } else {
                setScheduled(HANDLE_SLIDE_DOWN);
            }
        }
    } else {
        slopeInclineDirectlyUnderneath_ = 0;
        clearScheduled();
    }
    if (svec.x < 0) {
        setNewState(SLIDE_DOWN_LEFT);
    } else if (svec.x > 0) {
        setNewState(SLIDE_DOWN_RIGHT);
    }
}


void MobileObject::handleAirborne() {
    singleStatePersistenceTimer_ .airborneTimer++;
    singleStatePersistenceTimer_.movingHorizontallyTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;
    singleStatePersistenceTimer_.freefallTimer = 0;

    previouslyScheduled_ = scheduled_;

    newVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS);
    float sy = velocity_.verticalVelocity*(1.0/FPS);
    Point svec(sx, sy);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding();
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    float alpha = -INFINITY;

    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                collisionDetected = true;
                if (!groundUnderneathFound && collidesWithTopAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                    alpha = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
                    break;
                }
            }
        }
    }

    if (!collisionDetected) {
        translateObjectByVector(svec);
        setScheduled(HANDLE_AIRBORNE);
    } else {
        acceleration_.horizontalAcceleration = 0;
        if (svec.y > 0) {
            zeroVelocity();
            zeroAirborneGhostHorizontalVelocity();
        }
        if (groundUnderneathFound) {
            addGroundReactionAcceleration();
            slopeInclineDirectlyUnderneath_ = alpha;
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT >= -ERROR_EPS) {
                removeGroundReactionAcceleration();
                setScheduled(HANDLE_SLIDE_DOWN);
            } else {
                setScheduled(HANDLE_FREEFALL);
            }
        } else {
            setScheduled(HANDLE_FREEFALL);
        }
    }
    if (svec.x < 0) {
        setNewState(AIRBORNE_LEFT);
    } else if (svec.x > 0) {
        setNewState(AIRBORNE_RIGHT);
    }

}