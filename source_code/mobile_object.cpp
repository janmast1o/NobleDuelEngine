#include "mobile_object.h"
#include "object.h"
#include "constants.h"
#include "utility_functions.h"
#include <cmath>
#include <unordered_set>
#include <typeinfo>


const float MobileObject::defaultMTPIgnoreCoeff = 0.75;
const float MobileObject::defaultMTPBeNudgedCoeff = 0.95;
const float MobileObject::defaultMTPBeVecTranslatedCoeff = 1.25;

const float MobileObject::defaultCanCarryWeightOnTopCoeff = 1.4;
const float MobileObject::defaultShouldIgnoreOtherSlidingOffTopCoeff = 1.5;

const float MobileObject::defaultParamSxVecMomentumTransferLoss = 0.15;


MobileObject::MobileObject(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass) :
    Object(renderer, center, modelCollection, sessionEngineClock), objectMap_(objectMap), mass_(mass),
    momentumTransferProtcol_(*this), onlyParamSxMomentumTransferLoss_(defaultParamSxVecMomentumTransferLoss) {

        previousFrameCenter_ = {-INFINITY, -INFINITY};
        acceleration_.verticalAcceleration = -GRAVITATIONAL_PULL;
        slopeInclineDirectlyUnderneath_ = 0;
        participatingInMomentum_ = true;
        scheduled_ = HANDLE_FREEFALL;
        previouslyScheduled_ = NOTHING;
        
        momentumTransferProtcol_.setIgnoreUpperThreshold(defaultMTPIgnoreCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
        momentumTransferProtcol_.setBeNudgedUpperThreshold(defaultMTPBeNudgedCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
        momentumTransferProtcol_.setBeVecTranslatedUpperThreshold(defaultMTPBeVecTranslatedCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
        
        canCarryWeightOnTopFunction_ = [](float mass, float cumulativeWeight) {
            return defaultCanCarryWeightOnTopCoeff*mass > cumulativeWeight;
        };
        shouldIgnoreOtherSlidingOffTopFunction_ = [](float mass, float otherMass) {
            return defaultShouldIgnoreOtherSlidingOffTopCoeff*mass > otherMass;
        };
    }


bool MobileObject::isMobile() const {
    return true;
}    


void MobileObject::updatePreviousFrameCenter() {
    previousFrameCenter_ = getCenter();
}


void MobileObject::setCenter(const Point& newCenter) {
    Object::setCenter(newCenter);
    objectMap_.updateObjectPosition(*this);
}


float MobileObject::getCurrentHVelocity() const {
    return velocity_.horizontalVelocity;
}


float MobileObject::getMass() const {
    return mass_;
}


void MobileObject::setMass(float newMass, bool updateMomentumTransferProtocol) {
    mass_ = newMass;
    if (updateMomentumTransferProtocol) {
        momentumTransferProtcol_.setIgnoreUpperThreshold(defaultMTPIgnoreCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
        momentumTransferProtcol_.setBeNudgedUpperThreshold(defaultMTPBeNudgedCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
        momentumTransferProtcol_.setBeVecTranslatedUpperThreshold(defaultMTPBeVecTranslatedCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
    }
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
    objectSpecificPhysicsChar_.maxTrueHorizontalV = newTrueMaxVerticalV;
} 


float MobileObject::getTrueMaxHorizontalV() const {
    return objectSpecificPhysicsChar_.maxTrueHorizontalV;
}


void MobileObject::setTrueMaxHorizontalV(float newTrueMaxHorizontalV, bool updateMomentumTransferProtocol) {
    objectSpecificPhysicsChar_.maxTrueHorizontalV = newTrueMaxHorizontalV;
    if (updateMomentumTransferProtocol) {
        momentumTransferProtcol_.setIgnoreUpperThreshold(defaultMTPIgnoreCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
        momentumTransferProtcol_.setBeNudgedUpperThreshold(defaultMTPBeNudgedCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
        momentumTransferProtcol_.setBeVecTranslatedUpperThreshold(defaultMTPBeVecTranslatedCoeff*mass_*objectSpecificPhysicsChar_.maxTrueHorizontalV);
    }
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


float MobileObject::getShouldMTPIgnoreUpperThreshold() const {
    return momentumTransferProtcol_.getIgnoreUpperThreshold();
}


float MobileObject::getShouldMTPBeNudgedThreshold() const {
    return momentumTransferProtcol_.getBeNudgedUpperThreshold();
}


float MobileObject::getShouldMTPBeVecTranslatedThreshold() const {
    return momentumTransferProtcol_.getBeVecTranslatedUpperThreshold();
} 


void MobileObject::setShouldMTPIgnoreUpperThreshold(float newIgnoreUpperThreshold) {
    momentumTransferProtcol_.setIgnoreUpperThreshold(newIgnoreUpperThreshold);
}


void MobileObject::setShouldMTPBeNudgedUpperThreshold(float newBeNudgedUpperThreshold) {
    momentumTransferProtcol_.setBeNudgedUpperThreshold(newBeNudgedUpperThreshold);
}


void MobileObject::setShouldMTPBeVecTranslatedUpperThreshold(float newBeVecTranslatedUpperTheshold) {
    momentumTransferProtcol_.setBeVecTranslatedUpperThreshold(newBeVecTranslatedUpperTheshold);
}


float MobileObject::getParamSxMomentumTransferLoss() const {
    return onlyParamSxMomentumTransferLoss_;
}


void MobileObject::setOnlyParamSxMomentumTransferLoss(float newOnlyVecMomentumTransferLoss) {
    onlyParamSxMomentumTransferLoss_ = newOnlyVecMomentumTransferLoss;
}


void MobileObject::setCanCarryWeightOnTopFunction(std::function<bool(float, float)>& newCanCarryWeightOnTopFunction) {
    canCarryWeightOnTopFunction_ = newCanCarryWeightOnTopFunction;
}


void MobileObject::setShouldIgnoreOtherSlidingOffTopFunction(std::function<bool(float, float)>& newShouldIgnoreOtherSlidingOffTopFunction) {
    shouldIgnoreOtherSlidingOffTopFunction_ = newShouldIgnoreOtherSlidingOffTopFunction;
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
        velocity_.horizontalVelocity -= currentSign*FRICTION_ACC*(1.0/FPS);
        if (currentSign*getSign(velocity_.horizontalVelocity) < 0) {
            velocity_.horizontalVelocity = 0;
        }
    }
}


bool MobileObject::fixReceivedVelocityIfNeccessary(float& receivedHVelocity, float& receivedHTranslation) const {
    if (std::abs(receivedHVelocity) < CALC_ERROR_EPS) {
        receivedHVelocity = receivedHTranslation*FPS;
        return true;
    }

    return false;
}


void MobileObject::prepareNextSlideOffTopScheduled(const Object& objectUnderneath) {
    float slideDirAsFloat;
    if (objectUnderneath.getCenter().x < getCenter().x) {
        slideDirAsFloat = 1;
    } else {
        slideDirAsFloat = -1;
    }
    
    participatingInMomentum_ = false;
    addGroundReactionAcceleration();
    zeroAirborneGhostHorizontalVelocity();
    zeroVelocity();
    velocity_.horizontalVelocity = slideDirAsFloat*objectSpecificPhysicsChar_.slideV;
    setScheduled(HANDLE_SLIDE_OFF_TOP);
    if (objectUnderneath.isMobile()) {
        dynamic_cast<MobileObject&>(const_cast<Object&>(objectUnderneath)).prepareNextEscapeScheduled(-slideDirAsFloat, *this);
    }
}


void MobileObject::prepareNextEscapeScheduled(float escapeDirection, MobileObject& escapingFrom) {
    if (scheduled_ == HANDLE_AIRBORNE || scheduled_ == HANDLE_FREEFALL || 
        scheduled_ == HANDLE_SLIDE_DOWN_NO_RETRY || scheduled_ == HANDLE_SLIDE_DOWN_WITH_RETRY) {
            return;
    } else if (shouldIgnoreOtherSlidingOffTopFunction_(mass_, escapingFrom.getMass())) {
        return;
    }

    zeroVelocity();
    velocity_.horizontalVelocity = escapeDirection*objectSpecificPhysicsChar_.slideV;
    auxDistanceToCover_ = findEscapeDisAlongXAxis(escapingFrom, escapeDirection);
    auxDistanceCoveredSoFar_ = 0;
    setScheduled(HANDLE_ESCAPE_WITH_RETRY);    
}


Point MobileObject::smoothOutForDisplay(Point currentlyObservedRectangleUpperLeft, Point calcDisplayUpperLeft) {
    if (currentlyObservedRectangleUpperLeft == previousObservedRectangleUpperLeft_) calcDisplayUpperLeft;
    else if (previousDisplayUpperLeft_.x == -INFINITY && previousDisplayUpperLeft_.y == -INFINITY) {
        return calcDisplayUpperLeft;
    }

    Point displayUpperLeftAfterSmoothOut = calcDisplayUpperLeft;
    if (previousFrameCenter_.x == getCenter().x) {
        if (currentlyObservedRectangleUpperLeft.x > previousObservedRectangleUpperLeft_.x) {
            displayUpperLeftAfterSmoothOut.x = std::min(previousDisplayUpperLeft_.x, calcDisplayUpperLeft.x);
        } else if (currentlyObservedRectangleUpperLeft.x < previousObservedRectangleUpperLeft_.x) {
            displayUpperLeftAfterSmoothOut.x = std::max(previousDisplayUpperLeft_.x, calcDisplayUpperLeft.x);
        }
    } 

    if (previousFrameCenter_.y == getCenter().y) {
        if (currentlyObservedRectangleUpperLeft.y > previousObservedRectangleUpperLeft_.y) {
            displayUpperLeftAfterSmoothOut.y = std::min(previousDisplayUpperLeft_.y, calcDisplayUpperLeft.y);
        } else if (currentlyObservedRectangleUpperLeft.y < previousObservedRectangleUpperLeft_.y) {
            displayUpperLeftAfterSmoothOut.y = std::max(previousDisplayUpperLeft_.y, calcDisplayUpperLeft.y);
        }
    }

    return displayUpperLeftAfterSmoothOut;
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
                                              float& alpha, float& beta, float& gamma, float& dis, 
                                              bool& collisionDetected, 
                                              bool& groundUnderneathFound, bool& changingSlopes, bool moveHorizontallyCurrentlyHandled,
                                              std::list<MobileObject*>* foundMobileDirectlyAbove, 
                                              Object*& alphaTempObjectCurrentlyUnderneath, Object*& gammaTempObjectCurrentlyUnderneath) {                                            
    
    float localAlpha;
    
    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {;
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    groundUnderneathFound = true;
                    changingSlopes = false;

                } else {
                    localAlpha = isCollisionAfterVectorTranslationCausedByGentleSlope(*p, svec);
                    if (std::abs(localAlpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                        alpha = localAlpha;
                        alphaTempObjectCurrentlyUnderneath = p;
                        svec.y = alpha*svec.x;
                        groundUnderneathFound = true;
                        changingSlopes = false;

                    } else {
                        if (p->isMobile()) {
                            MobileObject* mop = dynamic_cast<MobileObject*>(p);
                            if (mop->isDirectlyAbove(*this) && foundMobileDirectlyAbove != nullptr) {
                                foundMobileDirectlyAbove->push_back(mop);
                            } else {
                                collisionDetected = true;
                                dis = std::min(dis, std::abs(findMinDistanceAlongTheLine(*p, svec)));

                                if (mop->isParticipatingInMomentum()) {
                                    mop->registerBeingAffectedByOutsideMomentum(mass_, velocity_.horizontalVelocity, svec.x);
                                    if (!moveHorizontallyCurrentlyHandled && getSign(mop->getCurrentHVelocity())*svec.x >= 0) {
                                        registerBeingAffectedByOutsideMomentum(mop->getMass(), mop->getCurrentHVelocity(), 0);
                                    }
                                }
                            }
                        } else {
                            collisionDetected = true;
                            dis = std::min(dis, std::abs(findMinDistanceAlongTheLine(*p, svec)));
                        }
                    }
                }

            } else if (!groundUnderneathFound || changingSlopes) {
                if (isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                    changingSlopes = false;
                    groundUnderneathFound = true;
                } else if (couldBeChangingSlopesAfterVectorTranslation(*p, svec)) { // add checking if it even makes sense to calculate delta
                    float delta = findMinVertDistanceFromTopAfterVectorTranslation(*p, svec);
                    
                    if (delta > 0 && delta < 2*MAXIMUM_GENTLE_SLOPE_COEFFICIENT*std::abs(svec.x)) {
                        gamma = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
                        gammaTempObjectCurrentlyUnderneath = p;
                        groundUnderneathFound = true;
                        changingSlopes = true;
                        beta = delta;
                    }    
                }
            }
        }
    }
}


void MobileObject::freefallMainBody(Point& svec, const std::list<Object*>& potentiallyUnderneath,
                                    float& alpha, float& delta, bool& groundUnderneathFound,
                                    Object*& alphaTempObjectCurrentlyUnderneath) {
    float tempAlpha, tempDelta;
    
    for (Object* p : potentiallyUnderneath) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithTopAfterVectorTranslation(*p, svec)) {
                groundUnderneathFound = true;
                tempAlpha = findSlopeCoefficientDirectlyBelowAfterVectorTranslation(*p, svec);
                tempDelta = findMinVertDistanceFromTop(*p);
                if (tempDelta > 0 && (tempDelta < delta || delta < 0)) {
                    alpha = tempAlpha;
                    delta = tempDelta;
                    alphaTempObjectCurrentlyUnderneath = p;
                } else if (delta < 0) {
                    alpha = tempAlpha;
                    alphaTempObjectCurrentlyUnderneath = p;
                }
            }
        }
    }
}


void MobileObject::adjustSVec(Point& svec) {
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


bool MobileObject::moveMobileDirectlyAbove(std::list<MobileObject*>& mobileDirectlyAbove, const Point& translationVector) {
    float sum = 0;
    for (MobileObject* m : mobileDirectlyAbove) {
        if (m->getScheduled() != HANDLE_SLIDE_OFF_TOP) {
            sum += m->getMass();
        }
    }
    if (!canCarryWeightOnTopFunction_(mass_, sum)) {
        return false;
    }
    std::unordered_set<MobileObject*> allDirectlyAboveSet(mobileDirectlyAbove.begin(), mobileDirectlyAbove.end());
    for (MobileObject* m : mobileDirectlyAbove) {
        std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*m, translationVector);
        bool collisionDetected = false;
        for (Object* p : potentiallyColliding) {
            if (p != this && p != m && m->collideableWith(*p) && m->collidesWithTopAfterVectorTranslation(*p, translationVector)) {
                collisionDetected = true;
            }
        }
        if (!collisionDetected) {
            m->translateObjectByVector(translationVector);
        }
    }
    return true;
}


void MobileObject::handleCheckForGroundDirectlyUnderneath() {
    std::list<Object*> potentiallyUnderneath = objectMap_.getPotentiallyColliding(*this);
    bool groundUnderneathFound = false;
    for (Object* p : potentiallyUnderneath) {
        if (p != this && collideableWith(*p)) {
            if (isDirectlyAbove(*p)) {
                groundUnderneathFound = true;
                break;
            }
        }
    }

    if (groundUnderneathFound) {
        clearScheduled();
    } else {
        removeGroundReactionAcceleration();
        setScheduled(HANDLE_FREEFALL);
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
    if (std::abs(sx) < CALC_ERROR_EPS) {
        clearScheduled();
        return;
    }

    float sy = sx*slopeInclineDirectlyUnderneath_;
    Point svec(sx, sy);
    adjustSVec(svec);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this, svec);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    bool changingSlopes = false;
    float alpha = -INFINITY;
    float beta = INFINITY;
    float gamma = -INFINITY;
    float dis = INFINITY;
    std::list<MobileObject*> foundMobileDirectlyAbove;
    Object* alphaTempObjectCurrentlyUnderneath = nullptr;
    Object* gammaTempObjectCurrentlyUnderneath = nullptr;

    horizontalMovementMainBody(svec, potentiallyColliding, alpha, beta, gamma, dis,
                               collisionDetected, groundUnderneathFound, changingSlopes, 
                               false, &foundMobileDirectlyAbove,
                               alphaTempObjectCurrentlyUnderneath,
                               gammaTempObjectCurrentlyUnderneath);

    
    if (!collisionDetected && !changingSlopes) {
        moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec);
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

            if (alphaTempObjectCurrentlyUnderneath != nullptr && !(alphaTempObjectCurrentlyUnderneath->canHaveOtherOnTop())) {
                prepareNextSlideOffTopScheduled(*alphaTempObjectCurrentlyUnderneath);
            }
            else {
                setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY);
            }

        }

    } else if (!collisionDetected && changingSlopes) {
        slopeInclineDirectlyUnderneath_ = gamma;

        moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec+Point(0,-beta));
        translateObjectByVector(svec+Point(0,-beta));
        // moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec);
        // translateObjectByVector(svec);

        if (!(gammaTempObjectCurrentlyUnderneath->canHaveOtherOnTop())) {
            prepareNextSlideOffTopScheduled(*gammaTempObjectCurrentlyUnderneath);
        }
        else if (std::abs(gamma)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
        } else {
            setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY);
        }

    } else {
        svec *= dis / svec.asVectorLength();
        moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec);
        translateObjectByVector(svec);

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


void MobileObject::handleEscapeFromUnderneathObjectOnTop(HandleParams handleParams) {
    previouslyScheduled_ = scheduled_;
    if (std::abs(auxDistanceCoveredSoFar_) >= std::abs(auxDistanceToCover_)) {
        zeroVelocity();
        clearScheduled();
        return;
    }

    newVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS);
    float sy = sx*slopeInclineDirectlyUnderneath_;
    Point svec(sx, sy);
    adjustSVec(svec);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this, svec);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    bool changingSlopes = false;
    float alpha = -INFINITY;
    float beta = INFINITY;
    float gamma = -INFINITY;
    float dis = INFINITY;
    std::list<MobileObject*> foundMobileDirectlyAbove;
    Object* alphaTempObjectCurrentlyUnderneath = nullptr;
    Object* gammaTempObjectCurrentlyUnderneath = nullptr;

    horizontalMovementMainBody(svec, potentiallyColliding, alpha, beta, gamma, dis,
                               collisionDetected, groundUnderneathFound, changingSlopes, 
                               false, &foundMobileDirectlyAbove,
                               alphaTempObjectCurrentlyUnderneath,
                               gammaTempObjectCurrentlyUnderneath);


    if (!collisionDetected && !changingSlopes) {
        moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec);
        auxDistanceCoveredSoFar_ += svec.x;
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

            if (alphaTempObjectCurrentlyUnderneath != nullptr && !(alphaTempObjectCurrentlyUnderneath->canHaveOtherOnTop())) {
                prepareNextSlideOffTopScheduled(*alphaTempObjectCurrentlyUnderneath);
            }
            else {
                setScheduled(HANDLE_ESCAPE_WITH_RETRY);
            }

        }

    } else if (!collisionDetected && changingSlopes) {
        slopeInclineDirectlyUnderneath_ = gamma;

        moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec+Point(0,-beta));
        translateObjectByVector(svec+Point(0,-beta));
        moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec);
        translateObjectByVector(svec);
        auxDistanceCoveredSoFar_ += svec.x;
        
        if (!(gammaTempObjectCurrentlyUnderneath->canHaveOtherOnTop())) {
            prepareNextSlideOffTopScheduled(*gammaTempObjectCurrentlyUnderneath);
        }
        else if (std::abs(gamma)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
        } else {
            setScheduled(HANDLE_ESCAPE_WITH_RETRY);
        }

    } else {
        svec *= dis/svec.asVectorLength();
        translateObjectByVector(svec);
        moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec);

        if (handleParams.retry) {
            setScheduled(HANDLE_ESCAPE_NO_RETRY);
        } else {
            clearScheduled();
        }

    }

    if (sx < 0) {
        setNewState(PUSHED_LEFT);
    } else {
        setNewState(PUSHED_RIGHT);
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
    adjustSVec(svec);
    std::list<Object*> potentiallyCollding = objectMap_.getPotentiallyColliding(*this, svec);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    float alpha = -INFINITY;
    float dis = INFINITY;
    Object* alphaTempObjectCurrentlyUnderneath = nullptr;

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
                            alphaTempObjectCurrentlyUnderneath = p;
                            if (std::abs(delta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT < ERROR_EPS) {
                                dis = std::min(dis, std::abs(findMinDistanceAlongTheLine(*p, svec)));
                                collisionDetected = true;
                            } else if (std::abs(delta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
                                svec.y = dvec.y;
                            }
                        }
                    } else {
                        dis = std::min(dis, std::abs(findMinDistanceAlongTheLine(*p, svec)));    
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
            slopeInclineDirectlyUnderneath_ = 0;
            acceleration_.horizontalAcceleration = 0;
            removeGroundReactionAcceleration();
            airborneGhostHorizontalVelocity_.horizontalVelocity = velocity_.horizontalVelocity;
            velocity_.horizontalVelocity = 0;
            setScheduled(HANDLE_FREEFALL);

        } else {
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                slopeInclineDirectlyUnderneath_ = alpha;
                // objectCurrentlyUnderneath_ = alphaTempObjectCurrentlyUnderneath;
                translateObjectByVector({0, SLIGHT_RAISE});
                
                if (!alphaTempObjectCurrentlyUnderneath->canHaveOtherOnTop()) {
                    prepareNextSlideOffTopScheduled(*alphaTempObjectCurrentlyUnderneath);
                } else {
                    clearScheduled();
                }

            } else {
                setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
            }
        }

    } else {
        svec *= dis/svec.asVectorLength();
        translateObjectByVector(svec);

        if (handleParams.retry) {
            setScheduled(HANDLE_SLIDE_DOWN_NO_RETRY);
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


void MobileObject::handleSlideOffTop() {
    previouslyScheduled_ = scheduled_;

    newVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS);
    float sy = -std::abs(slopeInclineDirectlyUnderneath_*sx);
    Point svec(sx, sy);
    adjustSVec(svec);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this, svec);
    bool keepSliding = false;
    bool collisionDetected = false;
    float dis = INFINITY;

    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p) && isDirectlyAbove(*p) && !(p->canHaveOtherOnTop())) {
            keepSliding = true;
        }
    }

    if (!keepSliding) {
        participatingInMomentum_ = true;
        slopeInclineDirectlyUnderneath_ = 0;
        acceleration_.horizontalAcceleration = 0;
        removeGroundReactionAcceleration();
        airborneGhostHorizontalVelocity_.horizontalVelocity = velocity_.horizontalVelocity;
        velocity_.horizontalVelocity = 0;
        setScheduled(HANDLE_FREEFALL);
        return;
    }

    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec) && !isDirectlyAboveAfterVectorTranslation(*p, svec)) {
                collisionDetected = true;
                dis = std::min(dis, std::abs(findMinDistanceAlongTheLine(*p, svec)));
            }
        }
    }

    if (!collisionDetected) {
        translateObjectByVector(svec);
    } else {
        svec *= dis/svec.asVectorLength();
        translateObjectByVector(svec);
    }

    setScheduled(HANDLE_SLIDE_OFF_TOP);

    if (sx < 0) {
        setNewState(FREEFALL_LEFT);
    } else {
        setNewState(FREEFALL_RIGHT);
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
    adjustSVec(svec);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this, svec);
    bool collisionDetected = false;
    bool groundUnderneathFound = false;
    std::list<MobileObject*> foundMobileDirectlyAbove;

    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                if (collidesWithTopAfterVectorTranslation(*p, svec)) {
                    collisionDetected = true;
                    groundUnderneathFound = true;
                } else {
                    if (p->isMobile()) {
                        MobileObject* mop = dynamic_cast<MobileObject*>(p);
                        if (svec.y > 0 && (mop->isDirectlyAbove(*this) || mop->isDirectlyAboveAfterVectorTranslation(*this, -svec))) { // might need to remove that second expr in dysjunction
                            foundMobileDirectlyAbove.push_back(mop);
                        } else {
                            collisionDetected = true;
                            if (mop->isParticipatingInMomentum()) {
                                mop->registerBeingAffectedByOutsideMomentum(mass_, velocity_.horizontalVelocity, svec.x);
                                if (getSign(mop->getCurrentHVelocity())*svec.x >= 0) {
                                    registerBeingAffectedByOutsideMomentum(mop->getMass(), mop->getCurrentHVelocity(), 0);
                                }
                            }
                        }
                    } else {
                        collisionDetected = true;
                    }
                }
            }
        }
    }

    bool mobileOnTopMoved = moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec);
    if (!mobileOnTopMoved) {
        collisionDetected = true;
    }

    if (!collisionDetected) {
        translateObjectByVector(svec);
        setScheduled(HANDLE_AIRBORNE);

    } else {
        if (groundUnderneathFound) {
            zeroVelocity();
        } else {
            if (svec.y > 0) {
                zeroVelocity();
            } else {
                velocity_.horizontalVelocity = 0;
            }
            zeroAirborneGhostHorizontalVelocity();
        }
        setScheduled(HANDLE_FREEFALL);

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
    adjustSVec(svec);
    std::list<Object*> potentiallyUnderneath = objectMap_.getPotentiallyColliding(*this, svec);
    bool groundUnderneathFound = false;
    float tempAlpha;
    float alpha = -INFINITY;
    float tempDelta;
    float delta = -1;
    Object* alphaTempObjectCurrentlyUnderneath = nullptr;

    freefallMainBody(svec, potentiallyUnderneath, 
                     alpha, delta, groundUnderneathFound,
                     alphaTempObjectCurrentlyUnderneath);
    
    if (groundUnderneathFound) {
        zeroVelocity();
        velocity_.horizontalVelocity = airborneGhostHorizontalVelocity_.horizontalVelocity;
        zeroAirborneGhostHorizontalVelocity();
        addGroundReactionAcceleration();
        slopeInclineDirectlyUnderneath_ = alpha;
        // objectCurrentlyUnderneath_ = alphaTempObjectCurrentlyUnderneath;

        if (!(alphaTempObjectCurrentlyUnderneath->canHaveOtherOnTop())) {
            prepareNextSlideOffTopScheduled(*alphaTempObjectCurrentlyUnderneath);
        } else if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT >= -ERROR_EPS) {
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

    if (isLeftFacing(getState())) {
        setNewState(IDLE_LEFT);
    } else {
        setNewState(IDLE_RIGHT);
    }
}

bool MobileObject::collidesWithAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().collidesWithAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
}


bool MobileObject::isDirectlyAbove(Object& otherObject) const {
    return getCurrentCollisionMesh().isDirectlyAbove(otherObject.getCurrentCollisionMesh());
}


bool MobileObject::isDirectlyAboveAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().isDirectlyAboveAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
}


bool MobileObject::collidesWithTopAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().collidesWithTopAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
}


float MobileObject::findMinDistanceAlongTheLine(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().findMinDistanceAlongTheLine(otherObject.getCurrentCollisionMesh(), translationVector);
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


float MobileObject::findSlopeCoefficientDirectlyBelow(Object& otherObject) const {
    return getCurrentCollisionMesh().findSlopeCoefficientDirectlyBelow(otherObject.getCurrentCollisionMesh());
}


float MobileObject::findEscapeDisAlongXAxis(Object& otherObject, float escapeDirection) const {
    return getCurrentCollisionMesh().findEscapeDisAlongXAxis(otherObject.getCurrentCollisionMesh(), escapeDirection);
}


bool MobileObject::couldBeChangingSlopesAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentCollisionMesh().couldBeChangingSlopesAfterVectorTranslation(otherObject.getCurrentCollisionMesh(), translationVector);
}


bool MobileObject::collidesWithHitboxAfterVectorTranslation(Object& otherObject, const Point& translationVector) const {
    return getCurrentHitbox().collidesWithAfterVectorTranslation(otherObject.getCurrentHitbox(), translationVector);
}


void MobileObject::translateObjectByVector(const Point& translationVector) {
    setCenter(getCenter()+translationVector);
    // objectMap_.updateObjectPosition(*this);
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


bool MobileObject::isParticipatingInMomentum() const {
    return participatingInMomentum_;
}


void MobileObject::registerBeingAffectedByOutsideMomentum(float otherObjectMass, float otherObjectHVelocity, float hTranslation) {
    bool onlySx = fixReceivedVelocityIfNeccessary(otherObjectHVelocity, hTranslation);
    float alpha = 1;
    if (onlySx) alpha -= onlyParamSxMomentumTransferLoss_;

    currentMomentumDictated_.receivedExplicitHTranslation += alpha*hTranslation*(otherObjectMass/mass_);
    currentMomentumDictated_.receivedHVelocity += alpha*calculateVelocityAfterCollision(mass_, velocity_.horizontalVelocity, otherObjectMass, otherObjectHVelocity);
    currentMomentumDictated_.cumulativeReceivedMomentum += alpha*otherObjectMass*otherObjectHVelocity;
    
    if (hTranslation < 0 && hTranslation < currentMomentumDictated_.maxReceivedNegExplicitHTranslation) {
        currentMomentumDictated_.maxReceivedNegExplicitHTranslation = hTranslation;
    } else if (hTranslation > 0 && hTranslation > currentMomentumDictated_.maxReceivedPosExplicitHTranslation) {
        currentMomentumDictated_.maxReceivedPosExplicitHTranslation = hTranslation;
    }

}


void MobileObject::redrawObject(const Rectangle& currentlyObservedRectangle, bool smoothOut) {
    Model* model = getNextModelPtr();
    if (model != nullptr && model->getTexture() != nullptr &&
        currentlyObservedRectangle.collidesWith(model->getRelativeRectangle()+center_)) {

        SDL_FRect destRect;
        destRect.w = model->getModelTextureWidth();
        destRect.h = model->getModelTextureHeight();
        Point calcUpperLeft = {
            model->getTextureRelativeUL().x + center_.x - currentlyObservedRectangle.upperLeft.x,
            model->getTextureRelativeUL().y + center_.y - currentlyObservedRectangle.upperLeft.y
        };

        if (smoothOut) calcUpperLeft = smoothOutForDisplay(currentlyObservedRectangle.upperLeft, calcUpperLeft);
        destRect.x = calcUpperLeft.x;
        destRect.y = -calcUpperLeft.y;
        previousFrameCenter_ = center_;
        previousObservedRectangleUpperLeft_ = currentlyObservedRectangle.upperLeft;
        previousDisplayUpperLeft_ = calcUpperLeft;

        SDL_RenderCopyF(renderer_, model->getTexture(), NULL, &destRect);
    } 
}


bool MobileObject::isAnythingScheduled() const {
    return scheduled_ != NOTHING;
}


void MobileObject::runScheduled() {
    if (momentumTransferProtcol_.runScheduledCorrespondingToFoundInterval()) return;
    
    if (isAnythingScheduled()) {
        currentMomentumDictated_.clear();
        switch (scheduled_) {
            case HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY:
                handleBePushedHorizontally();
                break;
            case HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY:
                handleBePushedHorizontally({0, false});
                break;     
            case HANDLE_SLIDE_DOWN_WITH_RETRY:
                handleSlideDown();
                break;
            case HANDLE_SLIDE_DOWN_NO_RETRY:
                handleSlideDown({0, false});
                break;
            case HANDLE_ESCAPE_WITH_RETRY:
                handleEscapeFromUnderneathObjectOnTop({0, true});
                break;
            case HANDLE_ESCAPE_NO_RETRY:
                handleEscapeFromUnderneathObjectOnTop({0, false});       
                break; 
            case HANDLE_SLIDE_OFF_TOP:
                handleSlideOffTop();
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
                clearScheduled();
                break;                
        }
    } else {
        handleCheckForGroundDirectlyUnderneath();
        currentMomentumDictated_.clear();
    }
}