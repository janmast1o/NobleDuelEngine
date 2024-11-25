#include <iostream>
#include <SDL2/SDL.h>
#include <optional>

#ifndef POINT_H
#define POINT_H

struct Point {
    float x;
    float y;

    Point();
    Point(float x, float y);
    Point(const Point& otherPoint);

    Point operator+(const Point& otherPoint) const;
    Point operator-(const Point& otherPoint) const;
    Point operator*(const float scalar) const;

    friend Point operator*(const float scalar, const Point& point);

    Point operator-() const;

    Point& operator+=(const Point& otherPoint);
    Point& operator-=(const Point& otherPoint);

    bool isLowerLeft(const Point& otherPoint) const;
    bool isUpperRight(const Point& otherPoint) const;

    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

#endif 

#ifndef RECTANGLE_H
#define RECTANGLE_H

struct Rectangle {
    Point lowerLeft;
    Point lowerRight;
    Point upperRight;
    Point upperLeft;

    Rectangle();
    Rectangle(Point lowerLeft, Point upperRight);
    Rectangle(float width, float height);
    Rectangle(const Rectangle& otherRectangle);

    bool collidesWith(const Rectangle& otherRectangle) const;
};

#endif 

#ifndef DIRECTION_H
#define DIRECTION_H

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#endif

#ifndef MATTER_H
#define MATTER_H

enum Matter {
    PHANTOM,
    LIGHT_PHANTOM,
    SOLID,
    PHANTOM_SOLID
};

#endif

#ifndef STATE_H
#define STATE_H

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
    PUSHED_LEFT,
    PUSHED_RIGHT,

    OWNED,
    OWNED_LEFT,
    OWNED_RIGHT,
    IN_USE_LEFT,
    IN_USE_RIGHT,
    IN_CHARGED_USE_LEFT,
    IN_CHARGED_USE_RIGHT
};

#endif

#ifndef DAMAGE_RECEIVE_STATE_H
#define DAMAGE_RECEIVE_STATE_H

enum DamageReceiveState {
    VULNERABLE,
    INVULNERABLE
};

#endif

#ifndef SCHEDULED_INSTRUCTION_H
#define SCHEDULED_INSTRUCTION_H

enum ScheduledInstruction {
    NOTHING,
    HANDLE_BE_PUSHED_HORIZONTALLY,
    HANDLE_MOVE_HORIZONTALLY,
    HANDLE_SLIDE_DOWN,
    HANDLE_AIRBORNE,
    HANDLE_FREEFALL,
    HANDLE_STOP,
    HANDLE_JUMP
};

#endif

#ifndef VELOCITY_H
#define VELOCITY_H

struct Velocity {
    float horizontalVelocity = 0;
    float verticalVelocity = 0;
};

#endif

#ifndef ACCELERATION_H
#define ACCELERATION_H

struct Acceleration {
    float horizontalAcceleration = 0;
    float verticalAcceleration = 0; 
};

#endif

#ifndef SINGLE_STATE_PERSISTENCE_TIMER_H
#define SINGLE_STATE_PERSISTENCE_TIMER_H

struct SingleStatePersistence {
    int movingHorizontallyTimer = 0;
    int slideDownTimer = 0;
    int airborneTimer = 0;
    int freefallTimer = 0;
};

#endif

#ifndef OBJECT_SPECIFIC_PHYSICS_CHAR_H
#define OBJECT_SPECIFIC_PHYSICS_CHAR_H

struct ObjectSpecificPhysicsChar {
    float maxVerticalV;
    float maxHorizontalV;
    float horizontalAcc;
    int maxAirborneAccelerableFrames;

    ObjectSpecificPhysicsChar();
};

#endif

#ifndef CREATURE_SPECIFIC_PHYSICS_CHAR_H
#define CREATURE_SPECIFIC_PHYSICS_CHAR_H

struct CreatureSpecificPhysicsChar {
    float jumpingV;

    float regularHorizontalAcc;
    float regularMaxHorizontalV;

    float sprintHorizontalAcc;
    float sprintMaxHorizontalV;

    float slowWalkHorizontalAcc;
    float slowWalkMaxHorizontalV;

    CreatureSpecificPhysicsChar();
};

#endif

#ifndef PLAYER_SPECIFIC_KEY_MAPPING_H
#define PLAYER_SPECIFIC_KEY_MAPPING_H

struct PlayerSpecificKeyMapping {
    int moveLeft;
    int moveRight;

    int sprintModifier;
    int slowWalkModifier;

    int jump;

    int interact;
    int switchToNextItem;
    int dropItem;

    PlayerSpecificKeyMapping();
};

#endif

#ifndef T_ENTRY_H
#define T_ENTRY_H

struct TEntry {
    std::optional<Point> sPoint;
    std::optional<Point> rPoint;
    std::optional<Point> previousSPoint;
    std::optional<Point> nextSPoint;
    std::optional<Point> previousRPoint;
    std::optional<Point> nextRPoint;
};

#endif

// #ifndef MOM_DICT_SCHEDULED_SPECS
// #define MOM_DICT_SCHEDULED_SPECS

// struct MomDictatedScheduledSpec {

// };

// #endif

