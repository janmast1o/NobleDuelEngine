#include "mobile_object.h"
#include "object.h"
#include "constants.h"
#include "utility_functions.h"
#include <cmath>

MobileObject::MobileObject(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, ObjectMap& objectMap, float mass) :
    Object(renderer, center, modelCollection), objectMap_(objectMap), mass_(mass) {
        acceleration_.verticalAcceleration = -GRAVITATIONAL_PULL;
        slopeInclineDirectlyUnderneath_ = 0;
        scheduled_ = HANDLE_FREEFALL;
        previouslyScheduled_ = NOTHING;
        shouldIgnoreOutsideMomentumFunction_ = [](float mass, float otherObjectMass, float otherObjectV){
            return 3.1*mass > otherObjectMass*otherObjectV;
        };
        shouldOnlyBeMovedSlightlyByOutsideMomentmFunction_ = [](float mass, float otherObjectMass, float otherObjectV){
            return 4.3*mass > otherObjectMass*otherObjectV;
        };
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
        float currentSign = getSign(velocity_.horizontalVelocity);
        velocity_.horizontalVelocity -= getSign(velocity_.horizontalVelocity)*AIR_RES_ACC*(1.0/FPS);
        if (currentSign*getSign(velocity_.horizontalVelocity) < 0) {
            velocity_.horizontalVelocity = 0;
        }
    }
    if (getSign(velocity_.horizontalVelocity)*slopeInclineDirectlyUnderneath_ > 0) {
        float currentSign = getSign(velocity_.horizontalVelocity);
        velocity_.horizontalVelocity -= getSign(velocity_.horizontalVelocity)*SLOPE_RES_COEFF*GRAVITATIONAL_PULL*std::sin(std::atan(std::abs(slopeInclineDirectlyUnderneath_)))*(1.0/FPS);
        if (currentSign*getSign(velocity_.horizontalVelocity) < 0) {
            velocity_.horizontalVelocity = 0;
        }
    }
    if (std::abs(velocity_.horizontalVelocity) < objectSpecificPhysicsChar_.maxHorizontalV) {
        velocity_.horizontalVelocity += acceleration_.horizontalAcceleration*(1.0/FPS);
    }
    
    if (std::abs(velocity_.verticalVelocity) < objectSpecificPhysicsChar_.maxVerticalV) {
        velocity_.verticalVelocity += acceleration_.verticalAcceleration*(1.0/FPS);
    }
    
    if ((scheduled_ == HANDLE_AIRBORNE && singleStatePersistenceTimer_.airborneTimer < MAX_AIRBORNE_ACCELERABLE) || 
        (scheduled_ == HANDLE_FREEFALL && singleStatePersistenceTimer_.freefallTimer < MAX_AIRBORNE_ACCELERABLE)) {
            if (std::abs(airborneGhostHorizontalVelocity_.horizontalVelocity) > LOWEST_V_UNAFFECTED_BY_AIR_RES) {
                airborneGhostHorizontalVelocity_.horizontalVelocity -= getSign(airborneGhostHorizontalVelocity_.horizontalVelocity)*AIR_RES_ACC;
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
    acceleration_.verticalAcceleration = -GRAVITATIONAL_PULL;
}


void MobileObject::addGroundReactionAcceleration() {
    acceleration_.verticalAcceleration = 0;
}


void MobileObject::zeroVelocity() {
    velocity_.horizontalVelocity = 0;
    velocity_.verticalVelocity = 0;
}


void MobileObject::zeroAirborneGhostHorizontalVelocity() {
    airborneGhostHorizontalVelocity_.horizontalVelocity = 0;
}


void MobileObject::horizontalMovementMainBody(Point& svec, const std::list<Object*>& potentiallyColliding, 
                                              float& alpha, float& beta, float& gamma, bool& collisionDetected, 
                                              bool& groundUnderneathFound, bool& changingSlopes) {
    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                    changingSlopes = false;
                } else {
                    alpha = isCollisionAfterVectorTranslationCausedByGentleSlope(*p, svec);
                    if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                        svec.y = alpha*svec.x;
                        groundUnderneathFound = true;
                        changingSlopes = false;
                    } else {
                        collisionDetected = true;
                    }
                }
            } else if (!groundUnderneathFound) {
                float delta;
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                } else {
                    delta = findMinVertDistanceFromTopAfterVectorTranslation(*p, svec);
                    if (delta > 0 && delta < 2*MAXIMUM_GENTLE_SLOPE_COEFFICIENT*std::abs(svec.x)) {
                        gamma = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
                        groundUnderneathFound = true;
                        changingSlopes = true;
                        beta = delta;
                    }
                }
            }
        }
    }
}


void MobileObject::handleBePushedHorizontally() {
    ++singleStatePersistenceTimer_.movingHorizontallyTimer;
    singleStatePersistenceTimer_.airborneTimer = 0;
    singleStatePersistenceTimer_.freefallTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;

    previouslyScheduled_ = scheduled_;

    newVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS);

    if (std::abs(sx) < ERROR_EPS) {
        clearScheduled();
        return;
    }

    float sy = sx*slopeInclineDirectlyUnderneath_;
    Point svec(sx, sy);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    bool changingSlopes = false;
    float alpha = -INFINITY;
    float beta = INFINITY;
    float gamma = -INFINITY;

    // for (Object* p : potentiallyColliding) {
    //     if (p != this && collideableWith(*p)) {
    //         if (collidesWithAfterVectorTranslation(*p, svec)) {
    //             if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
    //                 groundUnderneathFound = true;
    //                 changingSlopes = false;
    //             } else {
    //                 alpha = isCollisionAfterVectorTranslationCausedByGentleSlope(*p, svec);
    //                 if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
    //                     svec.y = alpha*sx;
    //                     groundUnderneathFound = true;
    //                     changingSlopes = false;
    //                 } else {
    //                     collisionDetected = true;
    //                 }
    //             }
    //         } else if (!groundUnderneathFound) {
    //             float delta;
    //             if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
    //                 groundUnderneathFound = true;
    //             } else {
    //                 delta = findMinVertDistanceFromTopAfterVectorTranslation(*p, svec);
    //                 if (delta > 0 && delta < 2*MAXIMUM_GENTLE_SLOPE_COEFFICIENT*std::abs(svec.x)) {
    //                     gamma = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
    //                     groundUnderneathFound = true;
    //                     changingSlopes = true;
    //                     beta = delta;
    //                 }
    //             }
    //         }
    //     }
    // }

    horizontalMovementMainBody(svec, potentiallyColliding, alpha, beta, gamma, 
                               collisionDetected, groundUnderneathFound, changingSlopes);

    if (!collisionDetected && !changingSlopes) {
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
            }
            clearScheduled();
        }
    } else if (!collisionDetected && changingSlopes) {
        slopeInclineDirectlyUnderneath_ = gamma;
        translateObjectByVector(svec+Point(0,-beta));
        if (std::abs(gamma)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN);
        } else {
            setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY);
        }
    } else {
        setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY);
    }

    if (svec.x < 0) {
        setNewState(MOVING_LEFT);
    } else if (svec.x > 0) {
        setNewState(MOVING_RIGHT);
    }
}


void MobileObject::handleMoveHorizontally() {
    ++singleStatePersistenceTimer_.movingHorizontallyTimer;
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
    bool changingSlopes = false;
    float alpha = -INFINITY;
    float beta = INFINITY;
    float gamma = -INFINITY;

    // for (Object* p : potentiallyColliding) {
    //     if (p != this && collideableWith(*p)) {
    //         if (collidesWithAfterVectorTranslation(*p, svec)) {
    //             if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
    //                 groundUnderneathFound = true;
    //                 changingSlopes = false;
    //             } else {
    //                 alpha = isCollisionAfterVectorTranslationCausedByGentleSlope(*p, svec);
    //                 if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
    //                     svec.y = alpha*sx;
    //                     groundUnderneathFound = true;
    //                     changingSlopes = false;
    //                 } else {
    //                     collisionDetected = true;
    //                 }
    //             }
    //         } else if (!groundUnderneathFound) {
    //             float delta;
    //             if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
    //                 groundUnderneathFound = true;
    //             } else {
    //                 delta = findMinVertDistanceFromTopAfterVectorTranslation(*p, svec);
    //                 if (delta > 0 && delta < 2*MAXIMUM_GENTLE_SLOPE_COEFFICIENT*std::abs(svec.x)) {
    //                     gamma = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
    //                     groundUnderneathFound = true;
    //                     changingSlopes = true;
    //                     beta = delta;
    //                 }
    //             }
    //         }
    //     }
    // }

    horizontalMovementMainBody(svec, potentiallyColliding, alpha, beta, gamma, 
                               collisionDetected, groundUnderneathFound, changingSlopes);

    if (!collisionDetected && !changingSlopes) {
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
            }
            clearScheduled();
        }
    } else if (!collisionDetected && changingSlopes) {
        slopeInclineDirectlyUnderneath_ = gamma;
        translateObjectByVector(svec+Point(0,-beta));
        if (std::abs(gamma)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN);
        } else {
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
    ++singleStatePersistenceTimer_.slideDownTimer;
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
    bool changingSlopes = false;
    float alpha = -INFINITY;

    for (Object* p : potentiallyCollding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                    changingSlopes = false;
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
        // std::cout << svec << std::endl;
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
                translateObjectByVector({0, SLIGHT_RAISE});
                clearScheduled();
            } else {
                setScheduled(HANDLE_SLIDE_DOWN);
            }
        }
    } else {
        // std::cout << Point(0, SLIGHT_RAISE) << std::endl;
        translateObjectByVector({0, std::abs(svec.y)});
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
    ++singleStatePersistenceTimer_ .airborneTimer;
    singleStatePersistenceTimer_.movingHorizontallyTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;
    singleStatePersistenceTimer_.freefallTimer = 0;

    previouslyScheduled_ = scheduled_;

    slopeInclineDirectlyUnderneath_ = 0;
    newVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS);
    float sy = velocity_.verticalVelocity*(1.0/FPS);
    Point svec(sx, sy);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this);
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
            // addGroundReactionAcceleration();
            slopeInclineDirectlyUnderneath_ = alpha;
            // std::cout << slopeInclineDirectlyUnderneath_ << std::endl;
            zeroVelocity();
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT >= -ERROR_EPS) {
                // removeGroundReactionAcceleration();
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
    ++singleStatePersistenceTimer_.freefallTimer;
    singleStatePersistenceTimer_.movingHorizontallyTimer = 0;
    singleStatePersistenceTimer_.airborneTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;

    previouslyScheduled_ = scheduled_;

    // std::cout << acceleration_.verticalAcceleration << std::endl;
    newVelocity();
    float sy = velocity_.verticalVelocity*(1.0/FPS);
    Point svec(0, sy);
    std::list<Object*> potentiallyUndeneath = objectMap_.getPotentiallyUnderneath(*this);
    float groundUnderneathFound = false;
    float alpha = -INFINITY;
    float tempDelta;
    float delta = -1;

    for (Object* p : potentiallyUndeneath) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithTopAfterVectorTranslation(*p, svec)) {
                groundUnderneathFound = true;
                alpha = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
                if ((tempDelta = findMinVertDistanceFromTop(*p)) > delta) {
                    delta = tempDelta;
                }
            }
        }
    }
    
    if (groundUnderneathFound) {
        zeroVelocity();
        velocity_.horizontalVelocity = airborneGhostHorizontalVelocity_.horizontalVelocity;
        zeroAirborneGhostHorizontalVelocity();
        addGroundReactionAcceleration();
        slopeInclineDirectlyUnderneath_ = alpha;
        // std::cout << slopeInclineDirectlyUnderneath_ << std::endl;
        if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT >= -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN);
        } else {
            clearScheduled();
        }
        if (delta > 0) {
            // std::cout << Point(0, -delta) << std::endl;
            translateObjectByVector({0, -delta});
            // clearScheduled();
        }
    } else {
        translateObjectByVector(svec);
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


float MobileObject::findMinVertDistanceFromTop(Object& otherObject) const {
    return getCurrentCollisionMesh().findMinVertDistanceFromTop(otherObject.getCurrentCollisionMesh());
}


float MobileObject::findMinVertDistanceFromTopAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().findMinVertDistanceFromTopAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
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
            case HANDLE_BE_PUSHED_HORIZONTALLY:
                handleBePushedHorizontally();
                break;
            case HANDLE_MOVE_HORIZONTALLY:
                // std::cout << "HMH" << std::endl;
                handleMoveHorizontally();
                break;
            case HANDLE_SLIDE_DOWN:
                // std::cout << "HSD" << std::endl;
                handleSlideDown();
                break;
            case HANDLE_AIRBORNE:
                // std::cout << "HA" << std::endl;
                handleAirborne();
                break;
            case HANDLE_FREEFALL:
                // std::cout << "HF" << std::endl;
                handleFreefall();
                break;
            case HANDLE_STOP:
                // std::cout << "HS" << std::endl;
                handleStop();
                break;
            default:
                clearScheduled();
                break;                
        }
    }
}


bool MobileObject::canBeMovedByOtherObject(float otherObjectMass, float otherObjectHVelocity) const {
    return !shouldIgnoreOutsideMomentumFunction_(mass_, otherObjectMass, otherObjectHVelocity);
}


void MobileObject::bePushedByOtherObject(float otherObjectMass, float otherObjectHVelocity, const Point& translationVector) {
    if (!shouldOnlyBeMovedSlightlyByOutsideMomentmFunction_(mass_, otherObjectMass, otherObjectHVelocity)) {
        ;
    } else if (!shouldIgnoreOutsideMomentumFunction_(mass_, otherObjectMass, otherObjectHVelocity)) {
        ;
    }
}
