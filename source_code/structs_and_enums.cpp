#include <iostream>

#ifndef POINT
#define POINT

struct Point {
    float x;
    float y;

    Point() : x(0), y(0) {}

    Point(float x, float y) : x(x), y(y) {}

    Point(const Point& otherPoint) : x(otherPoint.x), y(otherPoint.y) {}

    Point operator+(const Point& otherPoint) const {
        return Point(x+otherPoint.x, y+otherPoint.y);
    }

    Point operator-(const Point& otherPoint) const {
        return Point(x-otherPoint.x, y-otherPoint.y);
    }

    Point operator*(const float scalar) const {
        return Point(scalar*x, scalar*y);
    }

    friend Point operator*(const float scalar, const Point& point);

    Point operator-() const {
        return Point(-x,-y);
    }

    Point& operator+=(const Point& otherPoint) {
        x += otherPoint.x;
        y += otherPoint.y;
        return *this;
    }

    Point& operator-=(const Point& otherPoint) {
        x -= otherPoint.x;
        y -= otherPoint.y;
        return *this;
    }

    bool isLowerLeft(const Point& otherPoint) const {
        return x <= otherPoint.x && y <= otherPoint.y;
    }

    bool isUpperRight(const Point& otherPoint) const {
        return x >= otherPoint.x && y >= otherPoint.y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& point);

};

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

Point operator*(const float scalar, const Point& point) {
    return Point(scalar*point.x, scalar*point.y);
}

#endif

#ifndef RECTANGLE
#define RECTANGLE

struct Rectangle {
    Point lowerLeft;
    Point lowerRight;
    Point upperRight;
    Point upperLeft;

    Rectangle() : lowerLeft(Point()), lowerRight(Point()), upperRight(Point()), upperLeft(Point()) {}
    
    Rectangle(Point lowerLeft, Point upperRight) : lowerLeft(lowerLeft), upperRight(upperRight) {
        lowerRight = Point(upperRight.x, lowerLeft.y);
        upperLeft = Point(lowerLeft.x, upperRight.y);
    }

    Rectangle(float width, float height) {
        lowerLeft = Point(-width/2, -height/2);
        lowerRight = Point(width/2, -height/2);
        upperRight = Point(width/2, height/2);
        upperLeft = Point(-width/2, height/2);
    }

    Rectangle(const Rectangle& otherRectangle) : 
        lowerLeft(otherRectangle.lowerLeft),
        lowerRight(otherRectangle.lowerRight),
        upperRight(otherRectangle.upperRight),
        upperLeft(otherRectangle.upperLeft) {}

    bool collidesWith(const Rectangle& otherRectangle) const {
        if (lowerLeft.isLowerLeft(otherRectangle.lowerLeft) && upperRight.isUpperRight(otherRectangle.lowerLeft)) {
            return true;
        }
        else if (lowerLeft.isLowerLeft(otherRectangle.upperRight) && upperRight.isUpperRight(otherRectangle.upperRight)) {
            return true;
        }
        else if (lowerLeft.isLowerLeft(otherRectangle.lowerRight) && upperRight.isUpperRight(otherRectangle.lowerRight)) {
            return true;
        }
        else if (lowerLeft.isLowerLeft(otherRectangle.upperLeft) && upperRight.isUpperRight(otherRectangle.upperLeft)) {
            return true;
        }
        else if (otherRectangle.lowerLeft.isLowerLeft(lowerLeft) && otherRectangle.upperRight.isUpperRight(lowerLeft)) {
            return true;
        }
        else if (otherRectangle.lowerLeft.isLowerLeft(upperRight) && otherRectangle.upperRight.isUpperRight(upperRight)) {
            return true;
        }
        else if (otherRectangle.lowerLeft.isLowerLeft(lowerRight) && otherRectangle.upperRight.isUpperRight(lowerRight)) {
            return true;
        }
        else if (otherRectangle.lowerLeft.isLowerLeft(upperLeft) && otherRectangle.upperRight.isUpperRight(upperLeft)) {
            return true;
        }        
        else {
            return false;
        }
    }
};

#endif

#ifndef DIRECTION
#define DIRECTION

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#endif

#ifndef VELOCITY
#define VELOCITY

struct Velocity {
    float horizontalVelocity = 0;
    float verticalVelocity = 0;
};

#endif

#ifndef ACCELERATION
#define ACCELERATION

struct Acceleration {
    float horizontalAcceleration = 0;
    float veticalAcceleration = 0;
};

#endif

#ifndef MATTER
#define MATTER

enum Matter {
    PHANTOM,
    LIGHT_PHANTOM,
    SOLID,
    PHANTOM_SOLID
};

#endif

#ifndef STATE
#define STATE

enum State {
    IDLE, 
    IDLE_LEFT,
    IDLE_RIGHT,
    MOVING_LEFT,
    MOVING_RIGHT,
    QUICKLY_M_LEFT,
    QUICKLY_M_RIGHT,
    SLOWLY_M_LEFT,
    SLOWLY_M_RIGHT,
    AIRBORNE_LEFT,
    AIRBORNE_RIGHT,
    FREEFALL_LEFT,
    FREEFALL_RIGHT,
    SLIDE_DOWN_LEFT,
    SLIDE_DOWN_RIGHT,

    OWNED,
    OWNED_LEFT,
    OWNED_RIGHT,
    IN_USE_LEFT,
    IN_USE_RIGHT,
    IN_CHARGED_USE_LEFT,
    IN_CHARGED_USE_RIGHT
};

#endif

#ifndef DAMAGE_RECEIVE_STATE
#define DAMAGE_RECEIVE_STATE

enum DamageReceiveState {
    VULNERABLE,
    INVULNERABLE
};

#endif