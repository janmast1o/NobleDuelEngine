#include "floating_platform.h"
#include "constants.h"
#include "utility_functions.h"

FloatingPlatform::FloatingPlatform(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, 
                                   ObjectMap& objectMap, float mass, 
                                   const std::vector<Velocity>& velocitiesForMovementModes, const std::vector<Point>& bordersForMovementModes) : 
                                   Object(renderer, center, modelCollection, sessionEngineClock),
                                   MobileObject(renderer, center, modelCollection, sessionEngineClock, objectMap, mass), 
                                   velocitiesForMovementModes_(velocitiesForMovementModes),
                                   bordersForMovementModes_(bordersForMovementModes) {
    
    if (velocitiesForMovementModes_.size() != bordersForMovementModes_.size()) {
        throw std::runtime_error("velocitiesForMovementModes and bordersForMovementModes vector must have identical sizes");
    }
    movementModeIndex_ = 0;
    startToSlowDownDistance_ = 0;
    slowDownPerSecondRate_ = 0;
    minVeloctiesForMovementModes_ = std::vector<Velocity>(velocitiesForMovementModes_.size());
    for (size_t i=0; i<velocitiesForMovementModes_.size(); i++) {
        minVeloctiesForMovementModes_[i] = {std::abs(velocitiesForMovementModes_[i].horizontalVelocity/2), 
                                            std::abs(velocitiesForMovementModes_[i].verticalVelocity/2)};
    }
}


std::vector<Velocity> FloatingPlatform::getVelocitiesForMovementModes() const {
    return velocitiesForMovementModes_;
}


void FloatingPlatform::setVelocitiesForMovementModes(std::vector<Velocity>& newVelocitiesForMovementModes) {
    velocitiesForMovementModes_ = newVelocitiesForMovementModes;
}


std::vector<Point> FloatingPlatform::getBordersForMovementModes() const {
    return bordersForMovementModes_;
}


void FloatingPlatform::setBordersForMovementModes(std::vector<Point>& newBordersForMovementModes) {
    bordersForMovementModes_ = newBordersForMovementModes;
}


std::vector<Velocity> FloatingPlatform::getMinVelocitiesForMovementModes() const {
    return minVeloctiesForMovementModes_;
}


void FloatingPlatform::setMinVelocitiesForMovementModes(std::vector<Velocity>& newMinVelocitiesForMovementModes) {
    minVeloctiesForMovementModes_ = newMinVelocitiesForMovementModes;
}


float FloatingPlatform::getStartToSlowDownDistance() const {
    return startToSlowDownDistance_;
}


void FloatingPlatform::setStartToSlowDownDistance(float newStartToSlowDownDistance) {
    startToSlowDownDistance_ = newStartToSlowDownDistance;
}


float FloatingPlatform::getSlowDownPerSecondRate() const {
    return slowDownPerSecondRate_;
}


void FloatingPlatform::setSlowDownPerSecondRate(float newSlowDownRate) {
    slowDownPerSecondRate_ = newSlowDownRate;
}


bool FloatingPlatform::isParticipatingInMomentum() const {
    return false;
}


bool FloatingPlatform::willBorderBeBreached(Point& svec) {
    bool borderBreachedX = false;
    bool borderBreachedY = false;

    float centerXAfterSvecTr = getCenter().x+svec.x;
    float centerYAfterSvecTr = getCenter().y+svec.y;

    if (svec.x == 0) {
        borderBreachedX = true;
    } else if (svec.x < 0 && centerXAfterSvecTr < bordersForMovementModes_[movementModeIndex_].x) {
        borderBreachedX = true;
    } else if (svec.x > 0 && centerXAfterSvecTr > bordersForMovementModes_[movementModeIndex_].x) {
        borderBreachedX = true;
    }

    if (svec.y == 0) {
        borderBreachedY = true; 
    } else if (svec.y < 0 && centerYAfterSvecTr < bordersForMovementModes_[movementModeIndex_].y) {
        borderBreachedY = true;
    } else if (svec.y > 0 && centerYAfterSvecTr > bordersForMovementModes_[movementModeIndex_].y) {
        borderBreachedY = true;
    }

    return borderBreachedX && borderBreachedY;
}


void FloatingPlatform::adjustVelocity() {
    if (getCenter().distanceFromOther(bordersForMovementModes_[movementModeIndex_]) < startToSlowDownDistance_) {
        float horizontalSign = getSign(velocitiesForMovementModes_[movementModeIndex_].horizontalVelocity);
        float verticalSign = getSign(velocitiesForMovementModes_[movementModeIndex_].verticalVelocity);

        if (std::abs(velocity_.horizontalVelocity-horizontalSign*slowDownPerSecondRate_*(1.0/FPS)) >= minVeloctiesForMovementModes_[movementModeIndex_].horizontalVelocity) {
            velocity_.horizontalVelocity -= horizontalSign*slowDownPerSecondRate_*(1.0/FPS);
        }
        if (std::abs(velocity_.verticalVelocity-verticalSign*slowDownPerSecondRate_*(1.0/FPS)) >=  minVeloctiesForMovementModes_[movementModeIndex_].verticalVelocity) {
            velocity_.verticalVelocity -= verticalSign*slowDownPerSecondRate_*(1.0/FPS);
        }
    }
}


void FloatingPlatform::adjustSVec(Point& svec) {
    svec.x = bordersForMovementModes_[movementModeIndex_].x - getCenter().x;
    svec.y = bordersForMovementModes_[movementModeIndex_].y - getCenter().y;
}


void FloatingPlatform::handleMoveAccordingToMode() {
    previouslyScheduled_ = scheduled_;

    bool incrementMovementModeIndex = false;
    
    adjustVelocity();
    float sx = velocity_.horizontalVelocity*(1.0/FPS);
    float sy = velocity_.verticalVelocity*(1.0/FPS);
    Point svec(sx, sy);
    if (willBorderBeBreached(svec)) {
        adjustSVec(svec);
        incrementMovementModeIndex = true;
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
                    mop = dynamic_cast<MobileObject*>(p);
                    if (mop->isDirectlyAbove(*this)) {
                        collisionDetected = false;
                        foundMobileDirectlyAbove.push_back(mop);
                    } else if (mop->isParticipatingInMomentum()) {
                        mop->registerBeingAffectedByOutsideMomentum(mass_, velocity_.horizontalVelocity, svec.x);
                    }    
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
        if (incrementMovementModeIndex) {
            movementModeIndex_ = (movementModeIndex_+1)%velocitiesForMovementModes_.size();
            velocity_ = velocitiesForMovementModes_[movementModeIndex_];
        }
    };

    setNewState(IDLE);

}


void FloatingPlatform::runScheduled() {
    handleMoveAccordingToMode();
}

