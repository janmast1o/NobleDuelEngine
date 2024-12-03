#include "creature.h"
#include "constants.h"


Creature::Creature(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, ObjectMap& objectMap, float mass, int health) :
    MobileObject(renderer, center, modelCollection, objectMap, mass) {
        setHealth(health);
    }


void Creature::adjustAccAndVForRegular() {
    objectSpecificPhysicsChar_.horizontalAcc = creatureSpecificPhysicsChar_.regularHorizontalAcc;
    objectSpecificPhysicsChar_.maxSRHorizontalV = creatureSpecificPhysicsChar_.regularMaxHorizontalV;
}


void Creature::adjustAccAndVForSprint() {
    objectSpecificPhysicsChar_.horizontalAcc = creatureSpecificPhysicsChar_.sprintHorizontalAcc;
    objectSpecificPhysicsChar_.maxSRHorizontalV = creatureSpecificPhysicsChar_.sprintMaxHorizontalV;
}


void Creature::adjustAccAndVForSlowWalk() {
    objectSpecificPhysicsChar_.horizontalAcc = creatureSpecificPhysicsChar_.slowWalkHorizontalAcc;
    objectSpecificPhysicsChar_.maxSRHorizontalV = creatureSpecificPhysicsChar_.slowWalkMaxHorizontalV;
}  


void Creature::handleBePushedHorizontally(HandleParams handleParams) {
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
    std::list<MobileObject*> foundMobileDirectlyAbove;
    Object* alphaTempObjectCurrentlyUnderneath;
    Object* gammaTempObjectCurrentlyUnderneath;

    horizontalMovementMainBody(svec, potentiallyColliding, alpha, beta, gamma, 
                               collisionDetected, groundUnderneathFound, changingSlopes,
                               false, foundMobileDirectlyAbove,
                               alphaTempObjectCurrentlyUnderneath,
                               gammaTempObjectCurrentlyUnderneath);

    if (!collisionDetected && !changingSlopes) {
        if (moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec)) {
            translateObjectByVector(svec);
        }

        if (!groundUnderneathFound) {
            acceleration_.horizontalAcceleration = 0;
            airborneGhostHorizontalVelocity_.horizontalVelocity = velocity_.horizontalVelocity;
            velocity_.horizontalVelocity = 0;
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_FREEFALL);
        } else {
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                slopeInclineDirectlyUnderneath_ = alpha;
                objectCurrentlyUnderneath_ = alphaTempObjectCurrentlyUnderneath;
            }
            setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY); // previously no retry
        }

    } else if (!collisionDetected && changingSlopes) {
        slopeInclineDirectlyUnderneath_ = gamma;
        objectCurrentlyUnderneath_ = gammaTempObjectCurrentlyUnderneath;
        if (moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec+Point(0,-beta))) {
            translateObjectByVector(svec+Point(0,-beta));
        }

        if (std::abs(beta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
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
        setNewState(PUSHED_LEFT);
    } else if (svec.x > 0) {
        setNewState(PUSHED_RIGHT);
    }
}


void Creature::handleMoveHorizontally() {
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
    std::list<MobileObject*> foundMobileDirectlyAbove;
    Object* alphaTempObjectCurrentlyUnderneath;
    Object* gammaTempObjectCurrentlyUnderneath;

    horizontalMovementMainBody(svec, potentiallyColliding, alpha, beta, gamma, 
                               collisionDetected, groundUnderneathFound, changingSlopes, 
                               true, foundMobileDirectlyAbove,
                               alphaTempObjectCurrentlyUnderneath,
                               gammaTempObjectCurrentlyUnderneath);

    if (!collisionDetected && !changingSlopes) {
        if (moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec)) {
            translateObjectByVector(svec);
        }

        if (!groundUnderneathFound) {
            acceleration_.horizontalAcceleration = 0;
            airborneGhostHorizontalVelocity_.horizontalVelocity = velocity_.horizontalVelocity;
            velocity_.horizontalVelocity = 0;
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_FREEFALL);
        } else {
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                slopeInclineDirectlyUnderneath_ = alpha;
                objectCurrentlyUnderneath_ = alphaTempObjectCurrentlyUnderneath;
            }
            clearScheduled();
        }

    } else if (!collisionDetected && changingSlopes) {
        slopeInclineDirectlyUnderneath_ = gamma;
        objectCurrentlyUnderneath_ = gammaTempObjectCurrentlyUnderneath;
        if (moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec+Point(0,-beta))) {
            translateObjectByVector(svec+Point(0,-beta));
        }

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
        if (std::abs(acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.slowWalkHorizontalAcc) <= ERROR_EPS) {
            setNewState(SLOWLY_M_LEFT);
        }
        else if (std::abs(acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.regularHorizontalAcc) <= ERROR_EPS) {
            setNewState(MOVING_LEFT);
        }
        else if (std::abs(acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.sprintHorizontalAcc) <= ERROR_EPS) {
            setNewState(QUICKLY_M_LEFT);
        }
    }
    else if (svec.x > 0) {
        if (std::abs(acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.slowWalkHorizontalAcc) <= ERROR_EPS) {
            setNewState(SLOWLY_M_RIGHT);
        }
        else if (std::abs(acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.regularHorizontalAcc) <= ERROR_EPS) {
            setNewState(MOVING_RIGHT);
        }
        else if (std::abs(acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.sprintHorizontalAcc) <= ERROR_EPS) {
            setNewState(QUICKLY_M_RIGHT);
        }
    }
}


void Creature::handleJump() {
    previouslyScheduled_ = scheduled_;
    velocity_.verticalVelocity = creatureSpecificPhysicsChar_.jumpingV;
    removeGroundReactionAcceleration();
    airborneGhostHorizontalVelocity_.horizontalVelocity = velocity_.horizontalVelocity;
    setScheduled(HANDLE_AIRBORNE);
}


void Creature::updateTargetedPoint(const Point& newTargetedPoint) {
    targetedPoint_.x = newTargetedPoint.x;
    targetedPoint_.y = newTargetedPoint.y;
}



float Creature::getJumpingV() const {
    return creatureSpecificPhysicsChar_.jumpingV;
}


float Creature::getRegularHorizontalAcc() const {
    return creatureSpecificPhysicsChar_.regularHorizontalAcc;
}


float Creature::getRegularMaxHorizontalV() const {
    return creatureSpecificPhysicsChar_.regularMaxHorizontalV;
}


float Creature::getSprintHorizontalAcc() const {
    return creatureSpecificPhysicsChar_.sprintHorizontalAcc;
}


float Creature::getSprintMaxHorizontalV() const {
    return creatureSpecificPhysicsChar_.sprintMaxHorizontalV;
}


float Creature::getSlowWalkHorizontalAcc() const {
    return creatureSpecificPhysicsChar_.slowWalkHorizontalAcc;
}


float Creature::getSlowWalkMaxHorizontalV() const {
    return creatureSpecificPhysicsChar_.slowWalkMaxHorizontalV;
}


void Creature::setJumpingV(float newJumpingV) {
    creatureSpecificPhysicsChar_.jumpingV = newJumpingV;
}


void Creature::setRegularHorizontalAcc(float newRegularHorizontalAcc) {
    creatureSpecificPhysicsChar_.regularHorizontalAcc = newRegularHorizontalAcc;
}


void Creature::setRegularMaxHorizontalV(float newRegularMaxHorizontalV) {
    creatureSpecificPhysicsChar_.regularMaxHorizontalV = newRegularMaxHorizontalV;
}


void Creature::setSprintHorizontalAcc(float newSprintHorizontalAcc) {
    creatureSpecificPhysicsChar_.sprintHorizontalAcc = newSprintHorizontalAcc;
}


void Creature::setSprintMaxHorizontalV(float newSprintMaxHorizontalV) {
    creatureSpecificPhysicsChar_.sprintMaxHorizontalV = newSprintMaxHorizontalV;
}


void Creature::setSlowWalkHorizontalAcc(float newSlowWalkHorizontalAcc) {
    creatureSpecificPhysicsChar_.slowWalkHorizontalAcc = newSlowWalkHorizontalAcc;
}


void Creature::setSlowWalkMaxHorizontalV(float newSlowWalkMaxHorizontalV) {
    creatureSpecificPhysicsChar_.slowWalkMaxHorizontalV = newSlowWalkMaxHorizontalV;
}


bool Creature::canHaveOtherOnTop() const {
    return false;
}


void Creature::runScheduled() {
    if (!currentMomentumDictated_.isEmpty() && 
        !shouldIgnoreOutsideMomentumFunction_(mass_, objectSpecificPhysicsChar_.maxTrueHorizontalV, std::abs(currentMomentumDictated_.cumultativeReceivedMomentum))) {
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
            case HANDLE_JUMP:
                handleJump();
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
    } else {
        handleCheckForGroundDirectlyUnderneath();
        currentMomentumDictated_.clear();
    }
}