#include "structs_and_enums.h"
#include "constants.h"

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


bool Point::isLowerLeft(const Point& otherPoint) const {
    return x <= otherPoint.x && y <= otherPoint.y;
}


bool Point::isUpperRight(const Point& otherPoint) const {
    return x >= otherPoint.x && y >= otherPoint.y;
}


float Point::distanceFromOther(const Point& otherPoint) const {
    return std::sqrt(std::pow(x-otherPoint.x, 2) + std::pow(y-otherPoint.y, 2));
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
    cumultativeReceivedMomentum(0),
    maxReceivedPosExplicitHTranslation(0),
    maxReceivedNegExplicitHTranslation(0) {}


void MomentumDictated::clear() {
    receivedHVelocity = 0;
    receivedExplicitHTranslation = 0;
    cumultativeReceivedMomentum = 0;
    maxReceivedPosExplicitHTranslation = 0;
    maxReceivedNegExplicitHTranslation = 0;
}    


bool MomentumDictated::isEmpty() const {
    return std::abs(receivedHVelocity) < ERROR_EPS &&
           std::abs(receivedExplicitHTranslation) < ERROR_EPS && 
           std::abs(cumultativeReceivedMomentum) < ERROR_EPS;
}