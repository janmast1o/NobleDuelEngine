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
        shouldIgnoreOutsideMomentumFunction_ = [](float mass, float outsideMomentum){
            return 3.1*mass > outsideMomentum;
        };
        shouldOnlyBeMovedSlightlyByOutsideMomentmFunction_ = [](float mass, float outsideMomentum){
            return 4.3*mass > outsideMomentum;
        };
    }


float MobileObject::getCurrentHVelocity() const {
    return velocity_.horizontalVelocity;
}


float MobileObject::getMass() const {
    return mass_;
}


float MobileObject::getMaxSRVerticalV() const {
    return objectSpecificPhysicsChar_.maxSRVerticalV;
}


void MobileObject::setMaxSRVerticalV(float newMaxSRVerticalV) {
    objectSpecificPhysicsChar_.maxSRVerticalV = newMaxSRVerticalV;
}


float MobileObject::getMaxSRHorizontalV() const {
    return objectSpecificPhysicsChar_.maxSRHorizontalV;
}


void MobileObject::setMaxSRHorizontalV(float newMaxSRHorizontalV) {
    objectSpecificPhysicsChar_.maxSRHorizontalV = newMaxSRHorizontalV;
}


float MobileObject::getTrueMaxVerticalV() const {
    return objectSpecificPhysicsChar_.maxTrueVerticalV;
}


void MobileObject::setTrueMaxVerticalV(float newTrueMaxVerticalV) {
    objectSpecificPhysicsChar_.maxTrueHorizontalV;
} 


float MobileObject::getTrueMaxHorizontalV() const {
    return objectSpecificPhysicsChar_.maxTrueHorizontalV;
}


void MobileObject::setTrueMaxHorizontalV(float newTrueMaxHorizontalV) {
    objectSpecificPhysicsChar_.maxTrueHorizontalV = newTrueMaxHorizontalV;
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
        velocity_.horizontalVelocity -= currentSign*AIR_RES_ACC*(1.0/FPS);
        if (currentSign*getSign(velocity_.horizontalVelocity) < 0) {
            velocity_.horizontalVelocity = 0;
        }
    }
    if (getSign(velocity_.horizontalVelocity)*slopeInclineDirectlyUnderneath_ > 0) {
        float currentSign = getSign(velocity_.horizontalVelocity);
        velocity_.horizontalVelocity -= currentSign*SLOPE_RES_COEFF*GRAVITATIONAL_PULL*std::sin(std::atan(std::abs(slopeInclineDirectlyUnderneath_)))*(1.0/FPS);
        if (currentSign*getSign(velocity_.horizontalVelocity) < 0) {
            velocity_.horizontalVelocity = 0;
        }
    }
    if (std::abs(velocity_.horizontalVelocity) < objectSpecificPhysicsChar_.maxSRHorizontalV) {
        velocity_.horizontalVelocity += acceleration_.horizontalAcceleration*(1.0/FPS);
    }
    
    if (std::abs(velocity_.verticalVelocity) < objectSpecificPhysicsChar_.maxSRVerticalV) {
        velocity_.verticalVelocity += acceleration_.verticalAcceleration*(1.0/FPS);
    }
    
    if ((scheduled_ == HANDLE_AIRBORNE && singleStatePersistenceTimer_.airborneTimer < MAX_AIRBORNE_ACCELERABLE) || 
        (scheduled_ == HANDLE_FREEFALL && singleStatePersistenceTimer_.freefallTimer < MAX_AIRBORNE_ACCELERABLE)) {
            if (std::abs(airborneGhostHorizontalVelocity_.horizontalVelocity) > LOWEST_V_UNAFFECTED_BY_AIR_RES) {
                airborneGhostHorizontalVelocity_.horizontalVelocity -= getSign(airborneGhostHorizontalVelocity_.horizontalVelocity)*AIR_RES_ACC;
            }
            if (std::abs(airborneGhostHorizontalVelocity_.horizontalVelocity) < objectSpecificPhysicsChar_.maxSRHorizontalV) {
                airborneGhostHorizontalVelocity_.horizontalVelocity += getSign(airborneGhostHorizontalVelocity_.horizontalVelocity)*objectSpecificPhysicsChar_.horizontalAcc*(1.0/FPS);
            }
        } else if (scheduled_ == HANDLE_AIRBORNE || scheduled_ == HANDLE_FREEFALL) {
            airborneGhostHorizontalVelocity_.horizontalVelocity = 0;
        }

}


void MobileObject::applyFriction() {
    if (velocity_.horizontalVelocity != 0) {
        float currentSign = getSign(velocity_.horizontalVelocity);
        velocity_.horizontalVelocity -= FRICTION_ACC*(1.0/FPS);
        if (currentSign*getSign(velocity_.horizontalVelocity) < 0) {
            velocity_.horizontalVelocity = 0;
        }
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


// void MobileObject::updateAwaitingMomDictScheduledVelocity(float receivedVelocity) { // should add mutex after introducing multithreading (maybe actually not)
//     if (!awaitingMomDictScheduledSpecs_.blocked) {
//         if (awaitingMomDictScheduledSpecs_.isReceivedVelocityEmpty()) {
//             awaitingMomDictScheduledSpecs_.receivedVelocity = receivedVelocity;
//         } else {
//             if (awaitingMomDictScheduledSpecs_.receivedVelocity*receivedVelocity < 0) {
//                 awaitingMomDictScheduledSpecs_.blocked = true;
//             } else if (std::abs(receivedVelocity) > std::abs(awaitingMomDictScheduledSpecs_.receivedVelocity)) {
//                 awaitingMomDictScheduledSpecs_.receivedVelocity = receivedVelocity;
//             }
//         }
//     }
// }


// void MobileObject::updateAwaitingMomDictScheduledExplicitHTransaltion(float receivedExplicitHTranslation) { // blocking might not even be neccessary (only applies to this method) but im leaving it in for now
//     if (!awaitingMomDictScheduledSpecs_.blocked) {
//         if (awaitingMomDictScheduledSpecs_.isReceivedExplicitHTranslationEmpty()) {
//             awaitingMomDictScheduledSpecs_.receivedExplicitHTranslation = receivedExplicitHTranslation;
//         } else {
//             if (awaitingMomDictScheduledSpecs_.receivedExplicitHTranslation*receivedExplicitHTranslation < 0) {
//                 awaitingMomDictScheduledSpecs_.blocked = true;
//             } else if (std::abs(receivedExplicitHTranslation) > std::abs(awaitingMomDictScheduledSpecs_.receivedExplicitHTranslation)) {
//                 awaitingMomDictScheduledSpecs_.receivedExplicitHTranslation = receivedExplicitHTranslation;
//             }
//         }
//     }
// }


// void MobileObject::manageNextHandleBePushedDictByMom() {
//     if (!awaitingMomDictScheduledSpecs_.isReceivedVelocityEmpty()) {
//         velocity_.horizontalVelocity = awaitingMomDictScheduledSpecs_.receivedVelocity;
//         awaitingMomDictScheduledSpecs_.clear();
//         handleBePushedHorizontally();
//     } else if (!awaitingMomDictScheduledSpecs_.isReceivedExplicitHTranslationEmpty()) {
//         float predeterminedSx = awaitingMomDictScheduledSpecs_.receivedExplicitHTranslation;
//         awaitingMomDictScheduledSpecs_.clear();
//         handleBePushedHorizontally(predeterminedSx);
//     }
// }


// void MobileObject::manageNextHandleBePushedDictByMom() {
//     float predeterminedSx = awaitingMomDictScheduledSpecs_.receivedExplicitHTranslation;
//     if (!awaitingMomDictScheduledSpecs_.isReceivedVelocityEmpty()) {
//         velocity_.horizontalVelocity = awaitingMomDictScheduledSpecs_.receivedVelocity;
//     }
//     awaitingMomDictScheduledSpecs_.clear();
//     handleBePushedHorizontally(predeterminedSx);
// }


// void MobileObject::manageNextHandleAirborneDictByMom() {
//     if (!awaitingMomDictScheduledSpecs_.isReceivedVelocityEmpty()) {
//         velocity_.horizontalVelocity = awaitingMomDictScheduledSpecs_.receivedVelocity;
//         awaitingMomDictScheduledSpecs_.clear();
//         handleAirborne();
//     } else if (!awaitingMomDictScheduledSpecs_.isReceivedExplicitHTranslationEmpty()) {
//         float predeterminedSx = awaitingMomDictScheduledSpecs_.receivedExplicitHTranslation;
//         awaitingMomDictScheduledSpecs_.clear();
//         handleAirborne(predeterminedSx);
//     }
// }


// void MobileObject::manageNextHandleAirborneDictByMom() {
//     float predeterminedSx = awaitingMomDictScheduledSpecs_.receivedExplicitHTranslation;
//     if (!awaitingMomDictScheduledSpecs_.isReceivedVelocityEmpty()) {
//         velocity_.horizontalVelocity = awaitingMomDictScheduledSpecs_.receivedVelocity;
//     }
//     awaitingMomDictScheduledSpecs_.clear();
//     handleAirborne(predeterminedSx);
// }


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


void MobileObject::adjustSVecForMaxVReqs(Point& svec) const {
    float currentHSign, currentVSign;
    currentHSign = getSign(svec.x);
    currentVSign = getSign(svec.y);
    if (std::abs(svec.x) > objectSpecificPhysicsChar_.maxTrueHorizontalV*(1.0/FPS)) {
        svec.x = currentHSign*objectSpecificPhysicsChar_.maxTrueHorizontalV*(1.0/FPS);
    }
    if (std::abs(svec.y) > objectSpecificPhysicsChar_.maxTrueVerticalV*(1.0/FPS)) {
        svec.y = currentVSign*objectSpecificPhysicsChar_.maxTrueVerticalV*(1.0/FPS);
    }
}


void MobileObject::handleBePushedHorizontally(HandleParams handleParams) {
    ++singleStatePersistenceTimer_.movingHorizontallyTimer;
    singleStatePersistenceTimer_.airborneTimer = 0;
    singleStatePersistenceTimer_.freefallTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;

    previouslyScheduled_ = scheduled_;

    newVelocity();
    applyFriction();
    float sx = velocity_.horizontalVelocity*(1.0/FPS) + handleParams.paramSx;
    if (std::abs(sx) < ERROR_EPS) {
        clearScheduled();
        return;
    }
    float sy = sx*slopeInclineDirectlyUnderneath_;
    Point svec(sx, sy);
    adjustSVecForMaxVReqs(svec);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    bool changingSlopes = false;
    float alpha = -INFINITY;
    float beta = INFINITY;
    float gamma = -INFINITY;

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
            setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY);
        }
    } else if (!collisionDetected && changingSlopes) {
        slopeInclineDirectlyUnderneath_ = gamma;
        translateObjectByVector(svec+Point(0,-beta));
        if (std::abs(gamma)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
        } else {
            setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY);
        }
    } else {
        if (handleParams.retry) {
            setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY);
        } else {
            clearScheduled();
        }
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
    adjustSVecForMaxVReqs(svec);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    bool changingSlopes = false;
    float alpha = -INFINITY;
    float beta = INFINITY;
    float gamma = -INFINITY;

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
            setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
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


void MobileObject::handleSlideDown(HandleParams handleParams) {
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
    adjustSVecForMaxVReqs(svec);
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
                        if (std::abs(delta) < std::abs(alpha)) {
                            alpha = delta;
                            if (std::abs(delta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT < ERROR_EPS) {
                                collisionDetected = true;
                                break;
                            } else if (std::abs(delta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
                                svec.y = dvec.y;
                            }
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
                setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
            }
        }
    } else {
        // std::cout << Point(0, SLIGHT_RAISE) << std::endl;
        // translateObjectByVector({0, std::abs(svec.y)});
        if (handleParams.retry) {
            setScheduled(HANDLE_SLIDE_DOWN_NO_RETRY);
            // clearScheduled();
        } else {
            translateObjectByVector({0, std::abs(svec.y)});
            slopeInclineDirectlyUnderneath_ = 0;
            clearScheduled();
        }
    }
    if (svec.x < 0) {
        setNewState(SLIDE_DOWN_LEFT);
    } else if (svec.x > 0) {
        setNewState(SLIDE_DOWN_RIGHT);
    }
}


void MobileObject::handleAirborne(HandleParams handleParams) {
    ++singleStatePersistenceTimer_ .airborneTimer;
    singleStatePersistenceTimer_.movingHorizontallyTimer = 0;
    singleStatePersistenceTimer_.slideDownTimer = 0;
    singleStatePersistenceTimer_.freefallTimer = 0;

    previouslyScheduled_ = scheduled_;

    slopeInclineDirectlyUnderneath_ = 0;
    newVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS) + handleParams.paramSx;
    float sy = velocity_.verticalVelocity*(1.0/FPS);
    Point svec(sx, sy);
    adjustSVecForMaxVReqs(svec);
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
            slopeInclineDirectlyUnderneath_ = alpha;
            zeroVelocity();
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT >= -ERROR_EPS) {
                // removeGroundReactionAcceleration();
                setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
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

    newVelocity();
    float sy = velocity_.verticalVelocity*(1.0/FPS);
    Point svec(0, sy);
    adjustSVecForMaxVReqs(svec);
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
        if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT >= -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
        } else {
            clearScheduled();
        }
        if (delta > 0) {
            translateObjectByVector({0, -delta});
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


bool MobileObject::participatingInMomentum() const {
    return true;
}


void MobileObject::registerBeingAffectedByOutsideMomentum(float otherObjectMass, float otherObjectHVelocity, float hTranslation) {
    currentMomentumDictated_.receivedExplicitHTranslation += hTranslation*(otherObjectMass/mass_);
    currentMomentumDictated_.receivedHVelocity += calculateVelocityAfterCollision(mass_, velocity_.horizontalVelocity, otherObjectMass, otherObjectHVelocity);
    currentMomentumDictated_.cumultativeReceivedMomentum += otherObjectMass*otherObjectHVelocity;
}


bool MobileObject::isAnythingScheduled() const {
    return scheduled_ != NOTHING;
}


void MobileObject::runScheduledForNonEmptyMomentum() {
    // if (!isAnythingScheduled() || scheduled_ == HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY || scheduled_ == HANDLE_MOVE_HORIZONTALLY) {
    //     float paramSx;
    //     if (shouldIgnoreOutsideMomentumFunction_(mass_, currentMomentumDictated_.cumultativeReceivedMomentum)) {
    //         velocity_.horizontalVelocity = currentMomentumDictated_.receivedHVelocity;
    //     } else if (getSign(currentMomentumDictated_.receivedExplicitHTranslation)*velocity_.horizontalVelocity < 0) {
    //         velocity_.horizontalVelocity = 0;
    //     }
    //     paramSx = currentMomentumDictated_.receivedExplicitHTranslation;
    //     currentMomentumDictated_.clear();
    //     handleBePushedHorizontally({paramSx, true});
    // } else if (scheduled_ == HANDLE_FREEFALL || scheduled_ == HANDLE_AIRBORNE) {
    //     float paramSx;
    //     if (shouldIgnoreOutsideMomentumFunction_(mass_, currentMomentumDictated_.cumultativeReceivedMomentum)) {
    //         velocity_.horizontalVelocity = currentMomentumDictated_.receivedHVelocity;
    //     } else if (getSign(currentMomentumDictated_.receivedExplicitHTranslation)*velocity_.horizontalVelocity < 0) {
    //         velocity_.horizontalVelocity = 0;
    //     }
    //     paramSx = currentMomentumDictated_.receivedExplicitHTranslation;
    //     currentMomentumDictated_.clear();
    //     handleAirborne({paramSx, true});
    // }

    float paramSx;
    if (shouldIgnoreOutsideMomentumFunction_(mass_, currentMomentumDictated_.cumultativeReceivedMomentum)) {
        velocity_.horizontalVelocity = currentMomentumDictated_.receivedHVelocity;
    } else if (getSign(currentMomentumDictated_.receivedExplicitHTranslation)*velocity_.horizontalVelocity < 0) {
        velocity_.horizontalVelocity = 0;
    }
    paramSx = currentMomentumDictated_.receivedExplicitHTranslation;
    currentMomentumDictated_.clear();

    if (scheduled_ == HANDLE_FREEFALL || scheduled_ == HANDLE_AIRBORNE) {
        handleAirborne({paramSx});
    } else {
        handleBePushedHorizontally({paramSx, true});
    }

}


void MobileObject::runScheduled() {
    if (!currentMomentumDictated_.isEmpty() && !shouldIgnoreOutsideMomentumFunction_(mass_, currentMomentumDictated_.cumultativeReceivedMomentum)) {
        runScheduledForNonEmptyMomentum();
    }
    else if (isAnythingScheduled()) {
        currentMomentumDictated_.clear();
        switch (scheduled_) {
            case HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY:
                handleBePushedHorizontally();
                break;
            case HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY:
                handleBePushedHorizontally({0, false});
                break;     
            case HANDLE_MOVE_HORIZONTALLY:
                // std::cout << "HMH" << std::endl;
                handleMoveHorizontally();
                break;
            case HANDLE_SLIDE_DOWN_WITH_RETRY:
                // std::cout << "HSD" << std::endl;
                handleSlideDown();
                break;
            case HANDLE_SLIDE_DOWN_NO_RETRY:
                handleSlideDown({0, false});
                break;    
            case HANDLE_AIRBORNE:
                // std::cout << "HA" << std::endl;
                handleAirborne();
                break;
            case HANDLE_FREEFALL:
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