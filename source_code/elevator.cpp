#include "elevator.h"
#include "constants.h"

Elevator::Elevator(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock,
                   ObjectMap& objectMap, float mass, 
                   const std::vector<Velocity>& velocitiesForMovementModes, const std::vector<Point>& bordersForMovementModes) :
                   Object(renderer, center, modelCollection, sessionEngineClock),
                   FloatingPlatform(renderer, center, modelCollection, sessionEngineClock, objectMap, mass, velocitiesForMovementModes, bordersForMovementModes),
                   stopped_(true) {
    maxQueueSize_ = 2;
    perFloorRemain_ = 60;
}


int Elevator::getMaxQueueSize() const {
    return maxQueueSize_;
}


void Elevator::setMaxQueueSize(int newMaxQueueSize) {
    maxQueueSize_ = newMaxQueueSize;
}


int Elevator::getPerFloorRemain() const {
    return perFloorRemain_;
}


void Elevator::setPerFloorRemain(int newPerFloorRemain) {
    perFloorRemain_ = newPerFloorRemain;
}


bool Elevator::requestNewElevatorMovement(int requestedElevatorMoveIndex) {
    if (elevatorReqs_.size() >= maxQueueSize_) {
        return false;
    } else if (requestedElevatorMoveIndex < 0 || requestedElevatorMoveIndex >= velocitiesForMovementModes_.size()) {
        return false;
    } else if (elevatorReqs_.size() > 0 && elevatorReqs_.back() == requestedElevatorMoveIndex) {
        return false;
    } else {
        elevatorReqs_.emplace(requestedElevatorMoveIndex);
        return true;
    }
}


void Elevator::handleMoveAccordingToMode() {
    previouslyScheduled_ = scheduled_;

    bool destReached = false;
    adjustVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS);
    float sy = velocity_.verticalVelocity*(1.0/FPS);
    Point svec(sx, sy);
    if (willBorderBeBreached(svec)) {
        adjustSVec(svec);
        destReached = true;
    }

    const std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this);
    bool collisionDetected = false;
    std::list<MobileObject*> foundMobileDirectlyAbove;
    MobileObject* mop;

    for (Object* p : potentiallyColliding) {
        if (p != this && collideableWith(*p)) {
            if (collidesWithAfterVectorTranslation(*p, svec)) {
                collisionDetected = true;
                if (p->isMobile()) {
                    // collisionDetected = false;
                    mop = dynamic_cast<MobileObject*>(p);
                    if (mop->isDirectlyAbove(*this)) {
                        collisionDetected = false;
                        foundMobileDirectlyAbove.push_back(mop);
                    } else if (mop->isParticipatingInMomentum()) {
                        mop->registerBeingAffectedByOutsideMomentum(mass_, velocity_.horizontalVelocity, svec.x);
                    }    
                    // } else {
                    //     collisionDetected = true;
                    // }
                }    
            } else if (p->isMobile() && (mop = dynamic_cast<MobileObject*>(p))->isDirectlyAbove(*this)) {
                foundMobileDirectlyAbove.push_back(mop);
            }
        }
    }

    if (!collisionDetected && !moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec)) {
        collisionDetected = true;
    }

    if (!collisionDetected) {
        translateObjectByVector(svec);
        if (destReached) {
            stopped_ = true;
        }
    };

    setNewState(IDLE);
}


void Elevator::runScheduled() {
    // add clock and perFloorRemain
    if (stopped_ && !elevatorReqs_.empty()) {
        movementModeIndex_ = elevatorReqs_.front();
        velocity_ = velocitiesForMovementModes_[movementModeIndex_];
        elevatorReqs_.pop();
        stopped_ = false;
    } else if (!stopped_) {
        handleMoveAccordingToMode();
    }
}


