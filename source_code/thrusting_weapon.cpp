#include "thrusting_weapon.h"
#include "creature.h"


ThrustingWeapon::ThrustingWeapon(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, 
                    const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass,
                    int damage, int poiseDamage, unsigned short attackFrames, 
                    unsigned short recoveryFrames, float attackExtendRange,
                    ItemDependencyState initialDependencyState, Creature* initialOwner) :
                    Object(renderer, center, modelCollection, sessionEngineClock),
                    Item(renderer, center, modelCollection, sessionEngineClock, objectMap, mass, initialDependencyState, initialOwner),
                    damage_(damage), poiseDamage_(poiseDamage), 
                    attackFrames_(attackFrames), recoveryFrames_(recoveryFrames),
                    attackExtendRange_(attackExtendRange),
                    hasWindUp_(false), windUpFrames_(0), windUpExtendRange_(0),
                    currentFrameCounter_(0),
                    sideFacedDuringAttackStartUpAsInt_(0) {}


int ThrustingWeapon::getDamage() const {
    return damage_;
}                  


void ThrustingWeapon::setDamage(int newDamage) {
    damage_ = newDamage;
}


int ThrustingWeapon::getPoiseDamage() const {
    return poiseDamage_;
}


void ThrustingWeapon::setPoiseDamage(int newPoiseDamage) {
    poiseDamage_ = newPoiseDamage;
} 


bool ThrustingWeapon::getHasWindUp() const {
    return hasWindUp_;
}


void ThrustingWeapon::setHasWindUp(bool hasWindUp) {
    hasWindUp_ = hasWindUp;
}


unsigned short ThrustingWeapon::getWindUpFrames() const {
    return windUpFrames_;
}


void ThrustingWeapon::setWindUpFrames(unsigned short newWindUpFrames) {
    windUpFrames_ = newWindUpFrames;
}


unsigned short ThrustingWeapon::getAttackFrames() const {
    return attackFrames_;
}


void ThrustingWeapon::setAttackFrames(unsigned short newAttackFrames_) {
    attackFrames_ = newAttackFrames_;
}


unsigned short ThrustingWeapon::getRecoveryFrames() const {
    return recoveryFrames_;
}


void ThrustingWeapon::setRecoveryFrames(unsigned short newRecoveryFrames) {
    recoveryFrames_ = newRecoveryFrames;
}


float ThrustingWeapon::getWindUpExtendRange() const {
    return windUpExtendRange_;
}


void ThrustingWeapon::setWindUpExtendRange(float newWindUpExtendRange) {
    windUpExtendRange_ = newWindUpExtendRange;
}


float ThrustingWeapon::getAttackExtendRange() const {
    return windUpExtendRange_;
}


void ThrustingWeapon::setAttackExtendRange(float newAttackExtendRange) {
    attackExtendRange_ = newAttackExtendRange;
}


void ThrustingWeapon::handleAttack() {
    previouslyScheduled_ = scheduled_;
    if (sideFacedDuringAttackStartUpAsInt_ != getOwner()->getFacedSideAsInt()) {
        currentFrameCounter_ = 0;
        setScheduled(HANDLE_RECOVER);
        hitRegistry_.clear();
        return;
    }

    float sx = sideFacedDuringAttackStartUpAsInt_*(attackExtendRange_/ (float) attackFrames_);
    Point svec(sx, 0);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this);
    setMatter(LIGHT_PHANTOM);
    bool bouncedOffDetected = false;

    for (Object* p : potentiallyColliding) {
        if (p != this && p != getOwner() && collideableWith(*p) && hitRegistry_.find(p) != hitRegistry_.end()) {
            if (collidesWithHitboxAfterVectorTranslation(*p, svec)) {
                hitRegistry_.emplace(p);
                p->subtractFromHealth(damage_);
                // p->subtractFromPoiseHealth(poiseDamage_); // <- TODO: implement
                setMatter(PHANTOM);
                if (collideableWith(*p)) {
                    bouncedOffDetected = true;
                }
                setMatter(LIGHT_PHANTOM);
            }
        }
    }

    translateObjectByVector(svec);
    currentFrameCounter_++;
    if (currentFrameCounter_ == attackFrames_ || bouncedOffDetected) {
        currentFrameCounter_ = 0;
        hitRegistry_.clear();
        setScheduled(HANDLE_RECOVER);
    } else {
        setScheduled(HANDLE_ATTACK);
    }

    if (sideFacedDuringAttackStartUpAsInt_ < 0) {
        setNewState(IN_USE_LEFT);
    } else {
        setNewState(IN_USE_RIGHT);
    }
    
}


void ThrustingWeapon::handleRecoverToNeutral() {
    previouslyScheduled_ = scheduled_;
    float sx = -sideFacedDuringAttackStartUpAsInt_*(recoveryFrames_/(float) (attackExtendRange_ - windUpExtendRange_));
    Point svec(sx, 0);
    translateObjectByVector(svec);
    currentFrameCounter_++;
    if (currentFrameCounter_ == recoveryFrames_) {
        currentFrameCounter_ = 0;
        clearScheduled();
    } else {
        setScheduled(HANDLE_RECOVER);
    }

    if (sideFacedDuringAttackStartUpAsInt_ < 0) {
        setNewState(IN_RECOVERY_LEFT);
    } else {
        setNewState(IN_RECOVERY_RIGHT);
    }
}


void ThrustingWeapon::handleWindUp() {
    previouslyScheduled_ = scheduled_;
    if (sideFacedDuringAttackStartUpAsInt_ != getOwner()->getFacedSideAsInt()) {
        currentFrameCounter_ = 0;
        clearScheduled();
        return;
    }

    float sx = -sideFacedDuringAttackStartUpAsInt_*(windUpExtendRange_/(float) windUpFrames_);
    Point svec(sx, 0);
    translateObjectByVector(svec);
    currentFrameCounter_++;
    if (currentFrameCounter_ == windUpFrames_) {
        currentFrameCounter_ = 0;
        setScheduled(HANDLE_ATTACK);
    } else {
        setScheduled(HANDLE_WIND_UP);
    }

    if (sideFacedDuringAttackStartUpAsInt_ < 0) {
        setNewState(IN_WIND_UP_LEFT);
    } else {
        setNewState(IN_WIND_UP_RIGHT);
    }
}


void ThrustingWeapon::use() {
    if (isAvailableForNextUse()) {
        sideFacedDuringAttackStartUpAsInt_ = owner_->getFacedSideAsInt();
        updateLastUse();
        hitRegistry_.clear();
        setScheduled(HANDLE_ATTACK);
    }
}


void ThrustingWeapon::alternativeUse() {;}


void ThrustingWeapon::runScheduled() {
    if (dependencyState_ == INDEPENDENT) {
        MobileObject::runScheduled();
    } else if (isAnythingScheduled()) {
        switch (scheduled_) {
            case HANDLE_ATTACK:
                handleAttack();
                break;
            case HANDLE_RECOVER:
                handleRecoverToNeutral();
                break;    
            case HANDLE_WIND_UP:
                handleWindUp();
                break;
            default:
                clearScheduled();
                break;        
        }
    }
}