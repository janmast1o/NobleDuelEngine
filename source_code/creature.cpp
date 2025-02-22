#include "creature.h"
#include "constants.h"
#include "interactable_manager.h"
#include "item.h"
#include "utility_functions.h"


int Creature::defaultInitialItemListSize_ = 3;


Creature::Creature(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
                   int health, InteractableManager& interactableManager) :
                   Object(renderer, center, modelCollection, sessionEngineClock),
                   MobileObject(renderer, center, modelCollection, sessionEngineClock, objectMap, mass),
                   interactableManager_(interactableManager),
                   itemListIndex_(0) {
        setHealth(health);
        setMaxHealth(health);
        interactionScheduled_ = NOTHING;
        previousInteractionScheduled_ = NOTHING;
        itemList_ = std::vector<Item*>(defaultInitialItemListSize_, nullptr);
        staminaDrainProtocol_ = {1, 20, 10};
    }


bool Creature::isStaminaRegeningPossible() const {
    Item* currentlyHeldItem = itemList_[itemListIndex_];
    if (currentlyHeldItem != nullptr && 
        (currentlyHeldItem->getStaminaDrainedOnUse() > 0 || currentlyHeldItem->getStaminaDrainedOnAlternativeUse() > 0) &&
        currentlyHeldItem->isAnythingScheduled()) return false;
    else if (scheduled_ == HANDLE_AIRBORNE || scheduled_ == HANDLE_FREEFALL || 
             scheduled_ == HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY || scheduled_ == HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY ||
             (scheduled_ == HANDLE_MOVE_HORIZONTALLY && 
             std::abs(std::abs(acceleration_.horizontalAcceleration)-creatureSpecificPhysicsChar_.slowWalkHorizontalAcc) <= ERROR_EPS)) return false;

    return true;             
}



void Creature::setNewState(State newState) {
    Object::setNewState(newState);
    if (itemList_[itemListIndex_] != nullptr) {
        if (isLeftFacing(newState)) {
            itemList_[itemListIndex_]->setNewState(OWNED_LEFT);
        } else {
            itemList_[itemListIndex_]->setNewState(OWNED_RIGHT);
        }
    }
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
                this->MobileObject::slopeInclineDirectlyUnderneath_ = alpha;
                // objectCurrentlyUnderneath_ = alphaTempObjectCurrentlyUnderneath;
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
        // objectCurrentlyUnderneath_ = gammaTempObjectCurrentlyUnderneath;
        if (moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec+Point(0,-beta))) {
            translateObjectByVector(svec+Point(0,-beta));
        }

        if (!gammaTempObjectCurrentlyUnderneath->canHaveOtherOnTop()) {
            prepareNextSlideOffTopScheduled(*gammaTempObjectCurrentlyUnderneath);
        } else if (std::abs(beta)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
        } else {
            setScheduled(HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY);
        }

    } else {
        svec *= dis/svec.asVectorLength();
        moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec);
        translateObjectByVector(svec);

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

    if (std::abs(std::abs(acceleration_.horizontalAcceleration)-creatureSpecificPhysicsChar_.slowWalkHorizontalAcc) <= ERROR_EPS &&
        staminaDrainProtocol_.drainStaminaForSlowWalk(creatureGameStats_, sessionEngineClock_)) {
            
        clearScheduled();
        return;
    }
    else if (std::abs(std::abs(acceleration_.horizontalAcceleration)-creatureSpecificPhysicsChar_.regularHorizontalAcc) <= ERROR_EPS &&
             staminaDrainProtocol_.drainStaminaForRegularWalk(creatureGameStats_, sessionEngineClock_)) {
        
        if (!staminaDrainProtocol_.drainStaminaForSlowWalk(creatureGameStats_, sessionEngineClock_)) {
            adjustAccAndVForSlowWalk();
        } else {
            clearScheduled();
            return;
        }
    }
    else if (std::abs(std::abs(acceleration_.horizontalAcceleration)-creatureSpecificPhysicsChar_.sprintHorizontalAcc) <= ERROR_EPS &&
             staminaDrainProtocol_.drainStaminaForSprint(creatureGameStats_, sessionEngineClock_)) {
        
        if (!staminaDrainProtocol_.drainStaminaForRegularWalk(creatureGameStats_, sessionEngineClock_)) {
            adjustAccAndVForRegular();
        } else if (!staminaDrainProtocol_.drainStaminaForSlowWalk(creatureGameStats_, sessionEngineClock_)) {
            adjustAccAndVForSlowWalk();
        } else {
            clearScheduled();
            return;
        }
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
                               true, &foundMobileDirectlyAbove,
                               alphaTempObjectCurrentlyUnderneath,
                               gammaTempObjectCurrentlyUnderneath);    

    bool x = false;                           
    if (alpha > -INFINITY) { 
        std::cout << sessionEngineClock_.getCurrentTimeInFrames() << " " << alpha << " " << collisionDetected << changingSlopes << groundUnderneathFound << "\n";
        x = true;
    }

    if (!collisionDetected && !changingSlopes) {
        // std::cout << "A " << sessionEngineClock_.getCurrentTimeInFrames() << "\n";
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
            // std::cout << "B " << sessionEngineClock_.getCurrentTimeInFrames() << "\n";
            if (std::abs(alpha)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT <= ERROR_EPS) {
                this->MobileObject::slopeInclineDirectlyUnderneath_ = alpha;
                // std::cout << slopeInclineDirectlyUnderneath_ << sessionEngineClock_.getCurrentTimeInFrames() << "\n";
            }
            if (x) std::cout << slopeInclineDirectlyUnderneath_ << " " << sessionEngineClock_.getCurrentTimeInFrames() << std::endl;

            // std::cout << "A " << alphaTempObjectCurrentlyUnderneath << " " << typeid(alphaTempObjectCurrentlyUnderneath).name() << std::endl;
            if (alphaTempObjectCurrentlyUnderneath != nullptr && !(alphaTempObjectCurrentlyUnderneath->canHaveOtherOnTop())) {
                // std::cout << "C" << std::endl;
                prepareNextSlideOffTopScheduled(*alphaTempObjectCurrentlyUnderneath);
            } else {
                clearScheduled();
            }
            // std::cout << "B" << std::endl;
        }

    } else if (!collisionDetected && changingSlopes) {
        std::cout << sessionEngineClock_.getCurrentTimeInFrames() << " " << gamma << std::endl;
        slopeInclineDirectlyUnderneath_ = gamma;
        // objectCurrentlyUnderneath_ = gammaTempObjectCurrentlyUnderneath;
        if (moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec+Point(0,-beta))) {
            translateObjectByVector(svec+Point(0,-beta));
        }

        if (!(gammaTempObjectCurrentlyUnderneath->canHaveOtherOnTop())) {
            prepareNextSlideOffTopScheduled(*gammaTempObjectCurrentlyUnderneath);
        } else if (std::abs(gamma)-MAXIMUM_GENTLE_SLOPE_COEFFICIENT > -ERROR_EPS) {
            removeGroundReactionAcceleration();
            setScheduled(HANDLE_SLIDE_DOWN_WITH_RETRY);
        } else {
            clearScheduled();
        }

    } else {
        svec *= dis/svec.asVectorLength();
        if (moveMobileDirectlyAbove(foundMobileDirectlyAbove, svec)) {
            translateObjectByVector(svec);
        }
        // std::cout << "!\n";

        clearScheduled();
    }

    if (svec.x < 0) {
        if (std::abs(-acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.slowWalkHorizontalAcc) <= ERROR_EPS) {
            setNewState(SLOWLY_M_LEFT);
        }
        else if (std::abs(-acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.regularHorizontalAcc) <= ERROR_EPS) {
            setNewState(MOVING_LEFT);
        }
        else if (std::abs(-acceleration_.horizontalAcceleration-creatureSpecificPhysicsChar_.sprintHorizontalAcc) <= ERROR_EPS) {
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
    if (staminaDrainProtocol_.drainStaminaForJump(creatureGameStats_, sessionEngineClock_)) {
        clearScheduled();
        return;
    }
    
    velocity_.verticalVelocity = creatureSpecificPhysicsChar_.jumpingV;
    removeGroundReactionAcceleration();
    airborneGhostHorizontalVelocity_.horizontalVelocity = velocity_.horizontalVelocity;
    setScheduled(HANDLE_AIRBORNE);
}


void Creature::handleInteract() {
    previousInteractionScheduled_ = interactionScheduled_;
    std::list<Interactable*> availableInteractables = interactableManager_.getAllAvailableInteractables(targetedPoint_, *this);
    if (availableInteractables.size() != 0) {
        availableInteractables.front()->performOnInteraction((void*) this);
    }
    clearInteractionScheduled();
}


void Creature::handleUseItem() {
    previousInteractionScheduled_ = interactionScheduled_;
    if (itemList_[itemListIndex_] != nullptr) {
        itemList_[itemListIndex_]->use();
    } 
    clearInteractionScheduled();
}


void Creature::handleAltUseItem() {
    previousInteractionScheduled_ = interactionScheduled_;
    if (itemList_[itemListIndex_] != nullptr) {
        itemList_[itemListIndex_]->alternativeUse();
    } 
    clearInteractionScheduled();
}


void Creature::handleDropItem() {
    previousInteractionScheduled_ = interactionScheduled_;
    if (itemList_[itemListIndex_] != nullptr) {
        itemList_[itemListIndex_]->ridOfOwner();
        itemList_[itemListIndex_] = nullptr;
    }
    clearInteractionScheduled();
}


void Creature::handleSwitchToNextItem() {
    previousInteractionScheduled_ = interactionScheduled_;
    itemListIndex_ = (itemListIndex_+1)%itemList_.size();
    if (itemList_[itemListIndex_] != nullptr) {
        itemList_[itemListIndex_]->setCenter(getCurrentItemGripPoint());
    }
    clearInteractionScheduled();
}


void Creature::translateObjectByVector(const Point& translationVector) {
    setCenter(getCenter()+translationVector);
    if (itemList_[itemListIndex_] != nullptr) {
        itemList_[itemListIndex_]->translateObjectByVector(translationVector);
    }
}


ScheduledInstruction Creature::getInteractionScheduled() const {
    return interactionScheduled_; 
}


ScheduledInstruction Creature::getPreviousInteractionScheduled() const {
    return previousInteractionScheduled_;
}


void Creature::setInteractionScheduled(ScheduledInstruction newInteractionScheduled) {
    interactionScheduled_ = newInteractionScheduled;
}
    
    
void Creature::clearInteractionScheduled() {
    interactionScheduled_ = NOTHING;
}


void Creature::updateTargetedPoint(const Point& newTargetedPoint) {
    targetedPoint_.x = newTargetedPoint.x;
    targetedPoint_.y = newTargetedPoint.y;
}


Point Creature::getTargetedPoint() const {
    return targetedPoint_;
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


CreatureGameStats* Creature::getCreatureGameStatsAsPtr() {
    return &creatureGameStats_;
}


CreatureGameStats* Creature::getCreatureGameStatsAsPtr() const {
    return const_cast<CreatureGameStats*>(&creatureGameStats_);
}


StaminaDrainProtocol* Creature::getStaminaDrainProtocolAsPtr() {
    return &staminaDrainProtocol_;
}


StaminaDrainProtocol* Creature::getStaminaDrainProtocolAsPtr() const {
    return const_cast<StaminaDrainProtocol*>(&staminaDrainProtocol_);
}


int Creature::getFacedSideAsInt() const {
    if (isLeftFacing(getState())) {
        return -1;
    } else {
        return 1;
    }
}


bool Creature::canHaveOtherOnTop() const {
    return false;
}


void Creature::placeItemInItemList(Item& pickedUpItem) {
    if (itemList_[itemListIndex_] != nullptr) {
        itemList_[itemListIndex_]->ridOfOwner();
    }
    itemList_[itemListIndex_] = &pickedUpItem;
}


bool Creature::isAnythingInteractionLikeScheduled() const {
    if (itemList_[itemListIndex_] != nullptr && itemList_[itemListIndex_]->isAnythingScheduled()) return true; 
    return interactionScheduled_ != NOTHING;
}


bool Creature::isAnythingScheduledForItem() const {
    return itemList_[itemListIndex_] != nullptr && itemList_[itemListIndex_]->isAnythingScheduled();
}


void Creature::redrawObject() {
    Object::redrawObject();
    if (itemList_[itemListIndex_] != nullptr) {
        // itemList_[itemListIndex_]->Object::redrawObject();
        itemList_[itemListIndex_]->temporarilySetIndependentForRedrawing();
        itemList_[itemListIndex_]->redrawObject();
    }
}
    
    
void Creature::redrawObject(bool drawHitboxes, float pointSize) {
    Object::redrawObject(drawHitboxes, pointSize);
    if (itemList_[itemListIndex_] != nullptr) {
        // itemList_[itemListIndex_]->Object::redrawObject(drawHitboxes, pointSize);
        itemList_[itemListIndex_]->temporarilySetIndependentForRedrawing();
        itemList_[itemListIndex_]->redrawObject();
    }
}


void Creature::redrawObject(const Rectangle& currentlyObservedRectangle, bool smoothOut) {
    MobileObject::redrawObject(currentlyObservedRectangle, smoothOut);
    if (itemList_[itemListIndex_] != nullptr) {
        itemList_[itemListIndex_]->temporarilySetIndependentForRedrawing();
        itemList_[itemListIndex_]->redrawObject(currentlyObservedRectangle, smoothOut);
    }
}


void Creature::runInteractionScheduled() {
    if (isAnythingInteractionLikeScheduled()) {
        switch (interactionScheduled_) {
            case HANDLE_INTERACT:
                handleInteract();
                break;
            case HANDLE_USE:
                handleUseItem();
                break;
            case HANDLE_ALT_USE:
                handleAltUseItem();
                break;
            case HANDLE_DROP_ITEM:
                handleDropItem();
                break;
            case HANDLE_SWITCH_TO_NEXT_ITEM:
                handleSwitchToNextItem();
                break;               
            default:
                clearInteractionScheduled();
                break;    
        }
    }   
}


void Creature::runScheduled() {
    if (isStaminaRegeningPossible()) {
        creatureGameStats_.runStaminaRegening(sessionEngineClock_);
    } else {
        creatureGameStats_.subtractFromStamina(0, sessionEngineClock_);
    }
    creatureGameStats_.runPoiseRegening(sessionEngineClock_);
    creatureGameStats_.runPassiveEffects(sessionEngineClock_);

    runInteractionScheduled();
    if (momentumTransferProtcol_.runScheduledCorrespondingToFoundInterval()) return;
    
    if (isAnythingScheduled()) {
        currentMomentumDictated_.clear();
        std::cout << scheduled_ << std::endl;
        switch (scheduled_) {
            case HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY:
                handleBePushedHorizontally();
                break;
            case HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY:
                handleBePushedHorizontally({0, false});
                break;     
            case HANDLE_MOVE_HORIZONTALLY:
                handleMoveHorizontally();
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
            case HANDLE_JUMP:
                handleJump();
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