#include <iostream>
#include <SDL2/SDL.h>

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
    float verticalAcceleration = 0;
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

#ifndef SCHEDULED_INSTRUCTION
#define SCHEDULED_INSTRUCTION

enum ScheduledInstruction {
    NOTHING,
    HANDLE_MOVE_HORIZONTALLY,
    HANDLE_SLIDE_DOWN_WITH_RETRY,
    HANDLE_AIRBORNE,
    HANDLE_FREEFALL,
    HANDLE_STOP,
    HANDLE_JUMP
};

#endif

#ifndef SINGLE_STATE_PERSISTENCE
#define SINGLE_STATE_PERSISTENCE

struct SingleStatePersistence {
    int movingHorizontallyTimer = 0;
    int slideDownTimer = 0;
    int airborneTimer = 0;
    int freefallTimer = 0;
};

#endif

#ifndef OBJECT_SPECIFIC_PHYSICS_CHAR
#define OBJECT_SPECIFIC_PHYSICS_CHAR

struct ObjectSpecificPhysicsChar {
    float maxSRVerticalV;
    float maxSRHorizontalV;
    float horizontalAcc;
    int maxAirborneAccelerableFrames;

    ObjectSpecificPhysicsChar() :
        maxSRVerticalV(780),
        maxSRHorizontalV(225),
        horizontalAcc(215),
        maxAirborneAccelerableFrames(150) {}

};

#endif

#ifndef CREATURE_SPECIFIC_PHYSICS_CHAR
#define CREATURE_SPECIFIC_PHYSICS_CHAR

struct CreatureSpecificPhysicsChar {
    float jumpingV;

    float regularHorizontalAcc;
    float regularMaxHorizontalV;
    
    float sprintHorizontalAcc;
    float sprintMaxHorizontalV;

    float slowWalkHorizontalAcc;
    float slowWalkMaxHorizontalV;

    CreatureSpecificPhysicsChar() :
        jumpingV(385),
        regularHorizontalAcc(215),
        regularMaxHorizontalV(225),
        sprintHorizontalAcc(285),
        sprintMaxHorizontalV(300),
        slowWalkHorizontalAcc(40),
        slowWalkMaxHorizontalV(45) {}

};

#endif

#ifndef PLAYER_SPECIFIC_KEY_MAPPING
#define PLAYER_SPECIFIC_KEY_MAPPING

struct PlayerSpecificKeyMapping {
    int moveLeft;
    int moveRight;

    int sprintModifier;
    int slowWalkModifier;

    int jump;

    int interact;
    int switchToNextItem;
    int dropItem;

    PlayerSpecificKeyMapping() :
        moveLeft(SDL_SCANCODE_A),
        moveRight(SDL_SCANCODE_D),
        sprintModifier(SDL_SCANCODE_LSHIFT),
        slowWalkModifier(SDL_SCANCODE_V),
        jump(SDL_SCANCODE_SPACE),
        interact(SDL_SCANCODE_E),
        switchToNextItem(SDL_SCANCODE_C),
        dropItem(SDL_SCANCODE_R) {}

};

#endif

