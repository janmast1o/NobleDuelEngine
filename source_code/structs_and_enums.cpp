#include "structs_and_enums.h"
#include "constants.h"
#include "utility_functions.h"
#include "mobile_object.h"

float Point::comparisonEps = 1e-5;

Point::Point() : x(0), y(0) {}
Point::Point(float x, float y) : x(x), y(y) {}
Point::Point(const Point& otherPoint) : x(otherPoint.x), y(otherPoint.y) {}


Point Point::operator+(const Point& otherPoint) const {
    return Point(x + otherPoint.x, y + otherPoint.y);
}


Point Point::operator-(const Point& otherPoint) const {
    return Point(x - otherPoint.x, y - otherPoint.y);
}


Point Point::operator*(const float scalar) const {
    return Point(scalar * x, scalar * y);
}


Point operator*(const float scalar, const Point& point) {
    return Point(scalar * point.x, scalar * point.y);
}


Point operator*(const Point& point, const float scalar) {
    return Point(scalar * point.x, scalar * point.y);
}


Point Point::operator-() const {
    return Point(-x, -y);
}


Point& Point::operator+=(const Point& otherPoint) {
    x += otherPoint.x;
    y += otherPoint.y;
    return *this;
}


Point& Point::operator-=(const Point& otherPoint) {
    x -= otherPoint.x;
    y -= otherPoint.y;
    return *this;
}


Point& Point::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}


bool Point::operator==(const Point& otherPoint) const {
    return std::abs(x-otherPoint.x) < comparisonEps && std::abs(y-otherPoint.y) < comparisonEps;
}


bool Point::isLowerLeft(const Point& otherPoint) const {
    return x <= otherPoint.x && y <= otherPoint.y;
}


bool Point::isUpperRight(const Point& otherPoint) const {
    return x >= otherPoint.x && y >= otherPoint.y;
}


Point Point::getLowerLeft(const Point& otherPoint) const {
    return {std::min(x, otherPoint.x), std::min(y, otherPoint.y)};
}


Point Point::getUpperRight(const Point& otherPoint) const {
    return {std::max(x, otherPoint.x), std::max(y, otherPoint.y)};
}


float Point::distanceFromOther(const Point& otherPoint) const {
    return std::sqrt(std::pow(x-otherPoint.x, 2) + std::pow(y-otherPoint.y, 2));
}


float Point::asVectorLength() const {
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}


std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}


Rectangle::Rectangle() : lowerLeft({}), lowerRight({}), upperRight({}), upperLeft({}) {}


Rectangle::Rectangle(Point lowerLeft, Point upperRight) :
    lowerLeft(lowerLeft), upperRight(upperRight) {
        lowerRight = Point(upperRight.x, lowerLeft.y);
        upperLeft = Point(lowerLeft.x, upperRight.y);
}


Rectangle::Rectangle(float width, float height) {
    lowerLeft = Point(-width/2, -height/2);
    lowerRight = Point(width/2, -height/2);
    upperRight = Point(width/2, height/2);
    upperLeft = Point(-width/2, height/2);
}


Rectangle::Rectangle(const Rectangle& otherRectangle) :
    lowerLeft(otherRectangle.lowerLeft),
    lowerRight(otherRectangle.lowerRight),
    upperRight(otherRectangle.upperRight),
    upperLeft(otherRectangle.upperLeft) {}


Rectangle Rectangle::operator+(const Point& point) {
    return {lowerLeft+point, upperRight+point};
}


Rectangle Rectangle::operator+(const Point& point) const {
    return {lowerLeft+point, upperRight+point};
}


float Rectangle::getWidth() const {
    return (upperRight.x-lowerLeft.x);
}


float Rectangle::getHeight() const {
    return (upperRight.y-lowerLeft.y);
}


bool Rectangle::collidesWith(const Rectangle& otherRectangle) const {
    if (lowerLeft.isLowerLeft(otherRectangle.lowerLeft) && upperRight.isUpperRight(otherRectangle.lowerLeft)) {
        return true;
    } else if (lowerLeft.isLowerLeft(otherRectangle.upperRight) && upperRight.isUpperRight(otherRectangle.upperRight)) {
        return true;
    } else if (lowerLeft.isLowerLeft(otherRectangle.lowerRight) && upperRight.isUpperRight(otherRectangle.lowerRight)) {
        return true;
    } else if (lowerLeft.isLowerLeft(otherRectangle.upperLeft) && upperRight.isUpperRight(otherRectangle.upperLeft)) {
        return true;
    } else if (otherRectangle.lowerLeft.isLowerLeft(lowerLeft) && otherRectangle.upperRight.isUpperRight(lowerLeft)) {
        return true;
    } else if (otherRectangle.lowerLeft.isLowerLeft(upperRight) && otherRectangle.upperRight.isUpperRight(upperRight)) {
        return true;
    } else if (otherRectangle.lowerLeft.isLowerLeft(lowerRight) && otherRectangle.upperRight.isUpperRight(lowerRight)) {
        return true;
    } else if (otherRectangle.lowerLeft.isLowerLeft(upperLeft) && otherRectangle.upperRight.isUpperRight(upperLeft)) {
        return true;
    } else {
        return false;
    }
}


Rectangle Rectangle::getSmallestRectContainingBoth(const Rectangle& otherRectangle) const {
    return {lowerLeft.getLowerLeft(otherRectangle.lowerLeft), upperRight.getUpperRight(otherRectangle.upperRight)};
}


std::ostream& operator<<(std::ostream& os, State state) {
    switch (state) {
        case IDLE: os << "IDLE"; break;
        case IDLE_LEFT: os << "IDLE_LEFT"; break;
        case IDLE_RIGHT: os << "IDLE_RIGHT"; break;
        case MOVING_LEFT: os << "MOVING_LEFT"; break;
        case MOVING_RIGHT: os << "MOVING_RIGHT"; break;
        case QUICKLY_M_LEFT: os << "QUICKLY_M_LEFT"; break;
        case QUICKLY_M_RIGHT: os << "QUICKLY_M_RIGHT"; break;
        case SLOWLY_M_LEFT: os << "SLOWLY_M_LEFT"; break;
        case SLOWLY_M_RIGHT: os << "SLOWLY_M_RIGHT"; break;
        case AIRBORNE_LEFT: os << "AIRBORNE_LEFT"; break;
        case AIRBORNE_RIGHT: os << "AIRBORNE_RIGHT"; break;
        case FREEFALL_LEFT: os << "FREEFALL_LEFT"; break;
        case FREEFALL_RIGHT: os << "FREEFALL_RIGHT"; break;
        case SLIDE_DOWN_LEFT: os << "SLIDE_DOWN_LEFT"; break;
        case SLIDE_DOWN_RIGHT: os << "SLIDE_DOWN_RIGHT"; break;
        case PUSHED_LEFT: os << "PUSHED_LEFT"; break;
        case PUSHED_RIGHT: os << "PUSHED_RIGHT"; break;

        case OWNED: os << "OWNED"; break;
        case OWNED_LEFT: os << "OWNED_LEFT"; break;
        case OWNED_RIGHT: os << "OWNED_RIGHT"; break;
        case IN_USE_LEFT: os << "IN_USE_LEFT"; break;
        case IN_USE_RIGHT: os << "IN_USE_RIGHT"; break;
        case IN_RECOVERY_LEFT: os << "IN_RECOVERY_LEFT"; break;
        case IN_RECOVERY_RIGHT: os << "IN_RECOVERY_RIGHT"; break;
        case IN_WIND_UP_LEFT: os << "IN_WIND_UP_LEFT"; break;
        case IN_WIND_UP_RIGHT: os << "IN_WIND_UP_RIGHT"; break;

        default: os << "UNKNOWN_STATE"; break;
    }
    return os;   
}


std::ostream& operator<<(std::ostream& os, ScheduledInstruction scheduledInstruction) {
    switch (scheduledInstruction) {
        case NOTHING: os << "NOTHING"; break;
        case HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY: os << "HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY"; break;
        case HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY: os << "HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY"; break;
        case HANDLE_MOVE_HORIZONTALLY: os << "HANDLE_MOVE_HORIZONTALLY"; break;
        case HANDLE_SLIDE_DOWN_WITH_RETRY: os << "HANDLE_SLIDE_DOWN_WITH_RETRY"; break;
        case HANDLE_SLIDE_DOWN_NO_RETRY: os << "HANDLE_SLIDE_DOWN_NO_RETRY"; break;
        case HANDLE_AIRBORNE: os << "HANDLE_AIRBORNE"; break;
        case HANDLE_FREEFALL: os << "HANDLE_FREEFALL"; break;
        case HANDLE_STOP: os << "HANDLE_STOP"; break;
        case HANDLE_JUMP: os << "HANDLE_JUMP"; break;
        case HANDLE_FOREVER_FREEFALL: os << "HANDLE_FOREVER_FREEFALL"; break;
        case HANDLE_SLIDE_OFF_TOP: os << "HANDLE_SLIDE_OFF_TOP"; break;
        case HANDLE_ESCAPE_WITH_RETRY: os << "HANDLE_ESCAPE_WITH_RETRY"; break;
        case HANDLE_ESCAPE_NO_RETRY: os << "HANDLE_ESCAPE_NO_RETRY"; break;
        case HANDLE_INTERACT: os << "HANDLE_INTERACT"; break;
        case HANDLE_ATTACK: os << "HANDLE_ATTACK"; break;
        case HANDLE_WIND_UP: os << "HANDLE_WIND_UP"; break;
        case HANDLE_RECOVER: os << "HANDLE_RECOVER"; break;
        case HANDLE_USE: os << "HANDLE_USE"; break;
        case HANDLE_ALT_USE: os << "HANDLE_ALT_USE"; break;
        case HANDLE_DROP_ITEM: os << "HANDLE_DROP_ITEM"; break;
        case HANDLE_SWITCH_TO_NEXT_ITEM: os << "HANDLE_SWITCH_TO_NEXT_ITEM"; break;
        default: os << "UNKNOWN_ACTION"; break;
    }
    return os;
}


ObjectSpecificPhysicsChar::ObjectSpecificPhysicsChar() :
    maxSRVerticalV(780),
    maxSRHorizontalV(225),
    maxTrueVerticalV(800),
    maxTrueHorizontalV(400),
    horizontalAcc(215),
    slideV(285),
    maxAirborneAccelerableFrames(150) {}


CreatureSpecificPhysicsChar::CreatureSpecificPhysicsChar() :
    jumpingV(385),
    regularHorizontalAcc(215),
    regularMaxHorizontalV(225),
    sprintHorizontalAcc(285),
    sprintMaxHorizontalV(300),
    slowWalkHorizontalAcc(40),
    slowWalkMaxHorizontalV(45) {}


PlayerSpecificKeyMapping::PlayerSpecificKeyMapping() :
    moveLeftMapped(SDL_SCANCODE_A),
    moveRightMapped(SDL_SCANCODE_D),
    sprintModifierMapped(SDL_SCANCODE_LSHIFT),
    slowWalkModifierMapped(SDL_SCANCODE_V),
    jumpMapped(SDL_SCANCODE_SPACE),
    interactMapped(SDL_SCANCODE_E),
    switchToNextItemMapped(SDL_SCANCODE_C),
    dropItemMapped(SDL_SCANCODE_R) {}


PlayerActionReq PlayerSpecificKeyMapping::buildPlayerActionReq(const Uint8* keyboardState) const {
    PlayerActionReq newPlayerActionReq;
    newPlayerActionReq.moveLeft = keyboardState[moveLeftMapped];
    newPlayerActionReq.moveRight = keyboardState[moveRightMapped];
    newPlayerActionReq.sprintModifier = keyboardState[sprintModifierMapped];
    newPlayerActionReq.slowWalkModifier = keyboardState[slowWalkModifierMapped];
    newPlayerActionReq.jump = keyboardState[jumpMapped];
    newPlayerActionReq.interact = keyboardState[interactMapped];
    newPlayerActionReq.switchToNextItem = keyboardState[switchToNextItemMapped];
    newPlayerActionReq.dropItem = keyboardState[dropItemMapped];

    return newPlayerActionReq;
}    


MomentumDictated::MomentumDictated() :
    receivedHVelocity(0),
    receivedExplicitHTranslation(0),
    cumulativeReceivedMomentum(0),
    maxReceivedPosExplicitHTranslation(0),
    maxReceivedNegExplicitHTranslation(0) {}


void MomentumDictated::clear() {
    receivedHVelocity = 0;
    receivedExplicitHTranslation = 0;
    cumulativeReceivedMomentum = 0;
    maxReceivedPosExplicitHTranslation = 0;
    maxReceivedNegExplicitHTranslation = 0;
}    


bool MomentumDictated::isEmpty() const {
    return std::abs(receivedHVelocity) < ERROR_EPS &&
           std::abs(receivedExplicitHTranslation) < ERROR_EPS && 
           std::abs(cumulativeReceivedMomentum) < ERROR_EPS;
}


CreatureGameStats::CreatureGameStats(int maxPoiseHealth, int maxStamina) :
    lastPoiseSubtraction({0,0}), lastPoiseRegen({0,0}), waitSinceLastSubPoise(120),
    maxPoiseHealth(maxPoiseHealth), currentPoiseHealth(maxPoiseHealth),
    poiseRegenAmount(1), poiseRegenTick(4),
    lastStaminaSubtraction({0,0}), lastStaminaRegen({0,0}), waitSinceLastSubStamina(30),
    maxStamina(maxStamina), currentStamina(maxStamina),
    staminaRegenAmount(2), staminaRegenTick(2) {}


CreatureGameStats::CreatureGameStats(const CreatureGameStats& otherCreatureGameStats) = default;


bool CreatureGameStats::subtractFromPoiseHealth(int subtractAmount, const EngineClock& sessionClock) {
    if (currentPoiseHealth < subtractAmount) {
        return true;
    }
    currentPoiseHealth -= subtractAmount;
    lastPoiseSubtraction = {sessionClock.cycles, sessionClock.framesInCurrentCycle};
    return false;
}


void CreatureGameStats::addToPoiseHealth(int addAmount) {
    currentPoiseHealth += addAmount;
    if (currentPoiseHealth > maxPoiseHealth) {
        currentPoiseHealth = maxPoiseHealth;
    }
}

    
bool CreatureGameStats::subtractFromStamina(int subtractAmount, const EngineClock& sessionClock) {
    if (currentStamina < subtractAmount) {
        return true;
    }
    currentStamina -= subtractAmount;
    lastStaminaSubtraction = {sessionClock.cycles, sessionClock.framesInCurrentCycle};
    return false;
}


void CreatureGameStats::addToStamina(int addAmount) {
    currentStamina += addAmount;
    if (currentStamina > maxStamina) {
        currentStamina = maxStamina;
    }
}
    

void CreatureGameStats::addPassiveEffect(PassiveEffect& newPassiveEffect) {
    passiveEffects.emplace_back(newPassiveEffect);
}


void CreatureGameStats::runPoiseRegening(const EngineClock& sessionClock) {
    if (currentPoiseHealth < maxPoiseHealth) {
        if (sessionClock.getCurrentTimeInFrames() > convertPairToFrames(lastPoiseSubtraction, FPS)+waitSinceLastSubPoise) {
            if (sessionClock.getCurrentTimeInFrames() > convertPairToFrames(lastPoiseRegen, FPS)+poiseRegenTick) {
                lastPoiseRegen = {sessionClock.cycles, sessionClock.framesInCurrentCycle};
                addToPoiseHealth(poiseRegenAmount);
            }
        } else {
            lastPoiseRegen = {0,0};
        }
    }

}


void CreatureGameStats::runStaminaRegening(const EngineClock& sessionClock) {
    if (currentStamina < maxStamina) {
        if (sessionClock.getCurrentTimeInFrames() > convertPairToFrames(lastStaminaSubtraction, FPS)+waitSinceLastSubStamina) {
            if (sessionClock.getCurrentTimeInFrames() > convertPairToFrames(lastStaminaRegen, FPS)+staminaRegenTick) {
                lastStaminaRegen = {sessionClock.cycles, sessionClock.framesInCurrentCycle};
                addToStamina(staminaRegenAmount);
            }
        } else {
            lastStaminaRegen = {0,0};
        }
    }
}


void CreatureGameStats::runPassiveEffects(const EngineClock& sessionClock) {
    auto runPassiveIt = passiveEffects.begin();
    // auto runPassiveAuxIt = auxForPassiveEffects.begin();
    while (runPassiveIt != passiveEffects.end()) {
        bool shouldTerminateEffect = (*runPassiveIt)(nullptr);
        if (shouldTerminateEffect) {
            runPassiveIt = passiveEffects.erase(runPassiveIt);
            // runPassiveAuxIt = auxForPassiveEffects.erase(runPassiveAuxIt);
        } else {
            ++runPassiveIt;
            // ++runPassiveAuxIt;
        }
    }
} 


StaminaDrainProtocol::StaminaDrainProtocol(int sprintStaminaDrainAmount, int sprintStaminaDrainTick,
                                           int jumpStaminaDrainAmount) :
                                           slowWalkStaminaDrainAmount(0), slowWalkStaminaDrainTick(0),
                                           regularWalkStaminaDrainAmount(0), regularWalkStaminaDrainTick(0),
                                           sprintStaminaDrainAmount(sprintStaminaDrainAmount),
                                           sprintStaminaDrainTick(sprintStaminaDrainTick),
                                           jumpStaminaDrainAmount(jumpStaminaDrainAmount) {}


StaminaDrainProtocol::StaminaDrainProtocol(const StaminaDrainProtocol& otherStaminaDrainProtocol) = default;                                           


bool StaminaDrainProtocol::drainStaminaForSlowWalk(CreatureGameStats& creatureGameStats, const EngineClock& sessionClock) {
    if (slowWalkStaminaDrainAmount == 0) return false;
    else if (sessionClock.getCurrentTimeInFrames() > convertPairToFrames(lastSlowWalkStaminaSubtraction, FPS)+slowWalkStaminaDrainTick &&
             creatureGameStats.subtractFromStamina(slowWalkStaminaDrainAmount, sessionClock)) {
                lastSlowWalkStaminaSubtraction = {sessionClock.cycles, sessionClock.framesInCurrentCycle};
                return true;
    }
    return false;
}


bool StaminaDrainProtocol::drainStaminaForRegularWalk(CreatureGameStats& creatureGameStats, const EngineClock& sessionClock) {
    if (regularWalkStaminaDrainAmount == 0) return false;
    else if (sessionClock.getCurrentTimeInFrames() > convertPairToFrames(lastRegularWalkStaminaSubtraction, FPS)+regularWalkStaminaDrainTick &&
             creatureGameStats.subtractFromStamina(regularWalkStaminaDrainAmount, sessionClock)) {
                lastRegularWalkStaminaSubtraction = {sessionClock.cycles, sessionClock.framesInCurrentCycle};
                return true;
             }
    return false;
}


bool StaminaDrainProtocol::drainStaminaForSprint(CreatureGameStats& creatureGameStats, const EngineClock& sessionClock) {
    if (sprintStaminaDrainAmount == 0) return false;
    else if (sessionClock.getCurrentTimeInFrames() > convertPairToFrames(lastSprintStaminaSubtracion, FPS)+sprintStaminaDrainTick &&
             creatureGameStats.subtractFromStamina(sprintStaminaDrainAmount, sessionClock)) {
                lastSprintStaminaSubtracion = {sessionClock.cycles, sessionClock.framesInCurrentCycle};
                return true;
             }
    return false;         
}


bool StaminaDrainProtocol::drainStaminaForJump(CreatureGameStats& creatureGameStats, const EngineClock& sessionClock) {
    if (jumpStaminaDrainAmount == 0) return false;
    else return creatureGameStats.subtractFromStamina(jumpStaminaDrainAmount, sessionClock); 
}


CreatureGameStatsRetWrapper::CreatureGameStatsRetWrapper(const CreatureGameStats& creatureGameStats) :
    lastPoiseSubtraction(creatureGameStats.lastPoiseSubtraction),
    lastPoiseRegen(creatureGameStats.lastPoiseRegen),
    waitSinceLastSubPoise(const_cast<unsigned int&>(creatureGameStats.waitSinceLastSubPoise)),
    currentPoiseHealth(const_cast<const int&>(creatureGameStats.currentPoiseHealth)),
    maxPoiseHealth(const_cast<int&>(creatureGameStats.maxPoiseHealth)),
    poiseRegenAmount(const_cast<int&>(creatureGameStats.poiseRegenAmount)),
    poiseRegenTick(const_cast<int&>(creatureGameStats.poiseRegenTick)),
    lastStaminaSubtraction(creatureGameStats.lastStaminaSubtraction),
    lastStaminaRegen(creatureGameStats.lastStaminaRegen),
    waitSinceLastSubStamina(const_cast<unsigned int&>(creatureGameStats.waitSinceLastSubStamina)),
    currentStamina(const_cast<const int&>(creatureGameStats.currentStamina)),
    maxStamina(const_cast<int&>(creatureGameStats.maxStamina)),
    staminaRegenAmount(const_cast<int&>(creatureGameStats.staminaRegenAmount)),
    staminaRegenTick(const_cast<int&>(creatureGameStats.staminaRegenTick)),
    passiveEffects(const_cast<const std::list<PassiveEffect>&>(creatureGameStats.passiveEffects)) {}


FirearmFireSpecs::FirearmFireSpecs(int numberOfBullets, int perBulletDamage, std::vector<float> bulletSpreadFromCenter, 
                                   float bulletTravelDistance, bool bulletPierce, float bulletSpeed, Matter bulletMatter) :
                                   numberOfBullets(numberOfBullets), perBulletDamage(perBulletDamage),
                                   bulletSpreadFromCenter(bulletSpreadFromCenter), bulletTravelDistance(bulletTravelDistance),
                                   bulletPierce(bulletPierce), bulletSpeed(bulletSpeed), bulletMatter(bulletMatter) {}


ProjectileSpecs::ProjectileSpecs() : damage(0), travelDistance(0), pierce(false), travelSpeed(0) {}


ProjectileSpecs::ProjectileSpecs(int damage, float travelDistance, bool pierce, float travelSpeed, Matter matter) :
                                 damage(damage), travelDistance(travelDistance), pierce(pierce), travelSpeed(travelSpeed), matter(matter) {}  


GridOrganizerCreationArgs::GridOrganizerCreationArgs(Rectangle& celledRectangle, int numOfRows, int numOfCols) :
                                                     celledRectangle(celledRectangle), numOfRows(numOfRows), numOfCols(numOfCols) {}



MomentumTransferProtocol::MomentumTransferProtocol(MobileObject& correspondingMobileObject) :
    correspondingMobileObject(correspondingMobileObject),
    shouldIgnoreUpperThreshold(INFINITY), shouldBeNudgedUpperThreshold(INFINITY), shouldBeVecTranslatedUpperThreshold(INFINITY) {}


MomentumTransferProtocol::MomentumTransferProtocol(MobileObject& correspondingMobileObject, 
    float shouldIgnoreUpperTheshold, float shouldBeNudgedUpperThreshold, float shouldBeVecTranslatedUpperThreshold) :
    correspondingMobileObject(correspondingMobileObject),
    shouldIgnoreUpperThreshold(shouldIgnoreUpperTheshold), 
    shouldBeNudgedUpperThreshold(shouldBeNudgedUpperThreshold),
    shouldBeVecTranslatedUpperThreshold(shouldBeVecTranslatedUpperThreshold) {}


float MomentumTransferProtocol::getIgnoreUpperThreshold() const {
    return shouldIgnoreUpperThreshold;
}


void MomentumTransferProtocol::setIgnoreUpperThreshold(float newIgnoreUpperThreshold) {
    shouldIgnoreUpperThreshold = newIgnoreUpperThreshold;
}


float MomentumTransferProtocol::getBeNudgedUpperThreshold() const {
    return shouldBeNudgedUpperThreshold;
}


void MomentumTransferProtocol::setBeNudgedUpperThreshold(float newBeNudgedUpperThreshold) {
    shouldBeNudgedUpperThreshold = newBeNudgedUpperThreshold;
}


float MomentumTransferProtocol::getBeVecTranslatedUpperThreshold() const {
    return shouldBeVecTranslatedUpperThreshold;
}


void MomentumTransferProtocol::setBeVecTranslatedUpperThreshold(float newBeVecTranslatedThreshold) {
    shouldBeVecTranslatedUpperThreshold = newBeVecTranslatedThreshold;
}


bool MomentumTransferProtocol::shouldIgnoreOutsideMomentum() const {
    return shouldIgnoreUpperThreshold >= std::abs(correspondingMobileObject.currentMomentumDictated_.cumulativeReceivedMomentum);
}


bool MomentumTransferProtocol::shouldOnlyBeNudged() const {
    return shouldBeNudgedUpperThreshold >= std::abs(correspondingMobileObject.currentMomentumDictated_.cumulativeReceivedMomentum) &&
           shouldIgnoreUpperThreshold < std::abs(correspondingMobileObject.currentMomentumDictated_.cumulativeReceivedMomentum);
}


bool MomentumTransferProtocol::shouldOnlyBeVecTranslated() const {
    return shouldBeVecTranslatedUpperThreshold >= std::abs(correspondingMobileObject.currentMomentumDictated_.cumulativeReceivedMomentum) &&
           shouldBeNudgedUpperThreshold < std::abs(correspondingMobileObject.currentMomentumDictated_.cumulativeReceivedMomentum);
}


bool MomentumTransferProtocol::shouldHaveOutsideMomentumTransferred() const {
    return shouldBeVecTranslatedUpperThreshold < std::abs(correspondingMobileObject.currentMomentumDictated_.cumulativeReceivedMomentum);
}


float MomentumTransferProtocol::calculateSxCoeffForNudgingOnly() const {
    return (std::abs(correspondingMobileObject.currentMomentumDictated_.cumulativeReceivedMomentum) - shouldIgnoreUpperThreshold) / (shouldBeNudgedUpperThreshold - shouldIgnoreUpperThreshold);
}


bool MomentumTransferProtocol::runScheduledCorrespondingToFoundInterval() {
    if (correspondingMobileObject.currentMomentumDictated_.isEmpty() || shouldIgnoreOutsideMomentum()) {
        correspondingMobileObject.currentMomentumDictated_.clear();
        return false;
    }

    float paramSx = correspondingMobileObject.currentMomentumDictated_.receivedExplicitHTranslation;
    if (paramSx < 0 || paramSx < correspondingMobileObject.currentMomentumDictated_.maxReceivedNegExplicitHTranslation) {
        paramSx = correspondingMobileObject.currentMomentumDictated_.maxReceivedNegExplicitHTranslation;
    } else if (paramSx > 0 && paramSx > correspondingMobileObject.currentMomentumDictated_.maxReceivedPosExplicitHTranslation) {
        paramSx = correspondingMobileObject.currentMomentumDictated_.maxReceivedPosExplicitHTranslation;
    }

    if (shouldHaveOutsideMomentumTransferred()) {
        correspondingMobileObject.velocity_.horizontalVelocity = correspondingMobileObject.currentMomentumDictated_.receivedHVelocity;
    } else {
        if (shouldOnlyBeNudged()) paramSx *= calculateSxCoeffForNudgingOnly();

        if (getSign(paramSx*correspondingMobileObject.velocity_.horizontalVelocity) < 0) {
            correspondingMobileObject.velocity_.horizontalVelocity = 0;
        }
    }

    correspondingMobileObject.currentMomentumDictated_.clear();
    switch (correspondingMobileObject.scheduled_) {
        case HANDLE_FREEFALL:
            correspondingMobileObject.handleAirborne({paramSx});
            break;
        case HANDLE_AIRBORNE:
            correspondingMobileObject.handleAirborne({paramSx});
            break;
        default:
            correspondingMobileObject.handleBePushedHorizontally({paramSx, true});
            break;     
    }

    return true;    
}
