#include "mobile_object.h"
#include "object.h"
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
        if (velocity_.horizontalVelocity > 0) {
            velocity_.horizontalVelocity = 0;
        }
        acceleration_.horizontalAcceleration = -objectSpecificPhysicsChar_.horizontalAcc;
    } else if (direction == RIGHT) {
        if (velocity_.horizontalVelocity < 0) {
            velocity_.horizontalVelocity = 0;
        }
        acceleration_.horizontalAcceleration = objectSpecificPhysicsChar_.horizontalAcc;
    }
}


void MobileObject::newVelocity() {
    if (std::abs(velocity_.horizontalVelocity) > LOWEST_V_UNAFFECTED_BY_AIR_RES) {
        velocity_.horizontalVelocity -= getSign(velocity_.horizontalVelocity)*AIR_RES_COEFFICIENT*std::pow(velocity_.horizontalVelocity, 2);
    }
    if (std::abs(velocity_.horizontalVelocity) < objectSpecificPhysicsChar_.maxHorizontalV) {
        velocity_.horizontalVelocity += acceleration_.horizontalAcceleration*(1.0/FPS);
    }
    if (std::abs(velocity_.verticalVelocity) > LOWEST_V_UNAFFECTED_BY_AIR_RES) {
        velocity_.verticalVelocity -= getSign(velocity_.verticalVelocity)*AIR_RES_COEFFICIENT*std::pow(velocity_.verticalVelocity, 2);
    }
    if (std::abs(velocity_.verticalVelocity) < objectSpecificPhysicsChar_.maxVerticalV) {
        velocity_.verticalVelocity += acceleration_.veticalAcceleration*(1.0/FPS);
    }
    if ((scheduled_ == HANDLE_AIRBORNE && singleStatePersistenceTimer_.airborneTimer < MAX_AIRBORNE_ACCELERABLE) || 
        (scheduled_ == HANDLE_FREEFALL && singleStatePersistenceTimer_.freefallTimer < MAX_AIRBORNE_ACCELERABLE)) {
            if (std::abs(airborneGhostHorizontalVelocity_.horizontalVelocity) > LOWEST_V_UNAFFECTED_BY_AIR_RES) {
                airborneGhostHorizontalVelocity_.horizontalVelocity -= getSign(airborneGhostHorizontalVelocity_.horizontalVelocity)
                                                                               *AIR_RES_COEFFICIENT
                                                                               *std::pow(airborneGhostHorizontalVelocity_.horizontalVelocity, 2);
            }
            if (std::abs(airborneGhostHorizontalVelocity_.horizontalVelocity) < objectSpecificPhysicsChar_.maxHorizontalV) {
                airborneGhostHorizontalVelocity_.horizontalVelocity += getSign(airborneGhostHorizontalVelocity_.horizontalVelocity)*objectSpecificPhysicsChar_.horizontalAcc*(1.0/FPS);
            }
        } else if (scheduled_ == HANDLE_AIRBORNE || scheduled_ == HANDLE_FREEFALL) {
            airborneGhostHorizontalVelocity_.horizontalVelocity = 0;
        }

}


int MobileObject::getFacedSideAsInt() const {
    if (isLeftFacing(getState())) {
        return -1;
    } else {
        return 1;
    }
} 


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
            } else if (!groundUnderneathFound && isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                groundUnderneathFound = true;
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
            if (svec.y > 0) {
                zeroVelocity();
            } else {
                velocity_.horizontalVelocity = 0;
            }
            zeroAirborneGhostHorizontalVelocity();
            setScheduled(HANDLE_FREEFALL);
        }
    }
    if (svec.x < 0) {
        setNewState(AIRBORNE_LEFT);
    } else if (svec.x > 0) {
        setNewState(AIRBORNE_RIGHT);
    }

}


void MobileObject::handleFreefall() {
    singleStatePersistenceTimer_.freefallTimer++;
    singleStatePersistenceTimer_.movingHorizontallyTimer = 0;
    singleStatePersistenceTimer_.airborneTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;

    previouslyScheduled_ = scheduled_;

    newVelocity();
    float sy = velocity_.verticalVelocity*(1.0/FPS);
    Point svec(0, sy);
    std::list<Object*> potentiallyUndeneath = objectMap_.getPotentiallyUnderneath(this);
    float groundUnderneathFound = false;
    float alpha = -INFINITY;

    for (Object* p : potentiallyUndeneath) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                groundUnderneathFound = true;
                alpha = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
                break;
            }
        }
    }
    
    if (groundUnderneathFound) {
        zeroVelocity();
        velocity_.horizontalVelocity = airborneGhostHorizontalVelocity_.horizontalVelocity;
        zeroAirborneGhostHorizontalVelocity();
        addGroundReactionAcceleration();
        slopeInclineDirectlyUnderneath_ = alpha;
        if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT >= -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN);
        } else {
            clearScheduled();
        }
    } else {
        setScheduled(HANDLE_FREEFALL);
    }
    if (isLeftFacing(getPreviousState())) {
        setNewState(FREEFALL_LEFT);
    } else {
        setNewState(FREEFALL_RIGHT);
    }
}


void MobileObject::handleStop() {
    singleStatePersistenceTimer_.movingHorizontallyTimer = 0;
    singleStatePersistenceTimer_.airborneTimer = 0;
    singleStatePersistenceTimer_.freefallTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;

    acceleration_.horizontalAcceleration = 0;
    velocity_.horizontalVelocity = 0;
    previouslyScheduled_ = scheduled_;
    clearScheduled();

    if (isLeftFacing(getPreviousState())) {
        setNewState(IDLE_LEFT);
    } else {
        setNewState(IDLE_RIGHT);
    }
}

bool MobileObject::collidesWithAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().collidesWithAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
}


bool MobileObject::isDirectlyAboveAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().isDirectlyAboveAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
}


bool MobileObject::collidesWithTopAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().collidesWithTopAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
}


float MobileObject::isCollisionAfterVectorTranslationCausedByGentleSlope(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().isCollisionAfterVectorTranslationCausedByGentleSlope(otherObject.getCurrentCollisionMesh(), translationVector);
}


float MobileObject::findSlopeCoefficientDirectlyBelowAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().findSlopeCoefficientDirectlyBelowAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
}


void MobileObject::translateObjectByVector(const Point& translationVector) {
    setCenter(getCenter()+translationVector);
}


ScheduledInstruction MobileObject::getScheduled() const {
    return scheduled_;
}


ScheduledInstruction MobileObject::getPreviouslyScheduled() const {
    return previouslyScheduled_;
}


void MobileObject::setScheduled(ScheduledInstruction newScheduled) {
    scheduled_ = newScheduled;
}


void MobileObject::clearScheduled() {
    scheduled_ = NOTHING;
}


bool MobileObject::isAnythingScheduled() const {
    return scheduled_ != NOTHING;
}


void MobileObject::runScheduled() {
    if (isAnythingScheduled()) {
        switch (scheduled_) {
            case HANDLE_MOVE_HORIZONTALLY:
                handleMoveHorizontally();
                break;
            case HANDLE_SLIDE_DOWN:
                handleSlideDown();
                break;
            case HANDLE_AIRBORNE:
                handleAirborne();
                break;
            case HANDLE_FREEFALL:
                handleFreefall();
                break;
            case HANDLE_STOP:
                handleStop();
                break;
            default:
                break;                
        }
    }
}
