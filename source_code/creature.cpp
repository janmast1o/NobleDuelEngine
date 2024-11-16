#include "creature.h"
#include "constants.h"


Creature::Creature(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, ObjectMap& objectMap, int health) :
    MobileObject(renderer, center, modelCollection, objectMap) {
        setHealth(health);
    }


void Creature::adjustAccAndVForRegular() {
    objectSpecificPhysicsChar_.horizontalAcc = creatureSpecificPhysicsChar_.regularHorizontalAcc;
    objectSpecificPhysicsChar_.maxHorizontalV = creatureSpecificPhysicsChar_.regularMaxHorizontalV;
}


void Creature::adjustAccAndVForSprint() {
    objectSpecificPhysicsChar_.horizontalAcc = creatureSpecificPhysicsChar_.sprintHorizontalAcc;
    objectSpecificPhysicsChar_.maxHorizontalV = creatureSpecificPhysicsChar_.sprintMaxHorizontalV;
}


void Creature::adjustAccAndVForSlowWalk() {
    objectSpecificPhysicsChar_.horizontalAcc = creatureSpecificPhysicsChar_.slowWalkHorizontalAcc;
    objectSpecificPhysicsChar_.maxHorizontalV = creatureSpecificPhysicsChar_.slowWalkMaxHorizontalV;
}  


void Creature::handleMoveHorizontally() {
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


void Creature::runScheduled() {
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
            case HANDLE_JUMP:
                handleJump();
                break;    
            default:
                clearScheduled();
                break;                
        }
    }
}