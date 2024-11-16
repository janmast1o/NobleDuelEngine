#include "structs_and_enums.h"


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


ObjectSpecificPhysicsChar::ObjectSpecificPhysicsChar() :
    maxVerticalV(780),
    maxHorizontalV(225),
    horizontalAcc(215),
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
    moveLeft(SDL_SCANCODE_A),
    moveRight(SDL_SCANCODE_D),
    sprintModifier(SDL_SCANCODE_LSHIFT),
    slowWalkModifier(SDL_SCANCODE_V),
    jump(SDL_SCANCODE_SPACE),
    interact(SDL_SCANCODE_E),
    switchToNextItem(SDL_SCANCODE_C),
    dropItem(SDL_SCANCODE_R) {}