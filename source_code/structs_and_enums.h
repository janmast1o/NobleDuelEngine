#include <SDL2/SDL.h>
// #include "hitbox.cpp"

#ifndef POINT
#define POINT

struct Point {
    float x;
    float y;

    Point() : x(0.0), y(0.0) {}

    Point(float x, float y) : x(x), y(y) {}

    Point(const Point& other_point) : x(other_point.x), y(other_point.y) {}

    Point operator-(const Point& other_point) const {
        return Point(x-other_point.x, y-other_point.y);
    }

    Point operator+(const Point& other_point) const {
        return Point(x+other_point.x, y+other_point.y);
    }

    Point operator-() const {
        return Point(-x,-y);
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& point);

    bool is_lower_left(const Point& other_point) const {
        return (x <= other_point.x && y <= other_point.y);
    }

    bool is_upper_right(const Point& other_point) const {
        return (x >= other_point.x && y >= other_point.y);
    }

};

std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << "(" << point.x << ", " << point.y << ")";
        return os;
    }

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

#ifndef STATE
#define STATE

enum State {
    DEAD,
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
    SLIDE_DOWN_RIGHT, // for cretures

    OWNED,
    OWNED_LEFT,
    OWNED_RIGHT,
    IN_USE_LEFT,
    IN_USE_RIGHT,
    IN_CHARGED_USE_LEFT,
    IN_CHARGED_USE_RIGHT // for items
};

#endif

#ifndef DAMAGE_REC_STATE
#define DAMAGE_REC_STATE

enum DamageRecState {
    VULNERABLE,
    INVULNERABLE
};

#endif

#ifndef MATTER
#define MATTER

enum Matter {
    SOLID,
    PHANTOM,
    LIGHT_PHANTOM
};

#endif

#ifndef VELOCITY
#define VELOCITY

struct Velocity {
    float v_up = 0.0;
    float v_down = 0.0;
    float v_left = 0.0;
    float v_right = 0.0;
};

#endif

#ifndef ACCELERATION
#define ACCELERATION

struct Acceleration {
    float a_up = 0.0;
    float a_down = 0.0; // change it to gravitional pull later
    float a_left = 0.0;
    float a_right = 0.0;
};

#endif

#ifndef RECTANGLE
#define RECTANGLE

struct Rectangle {
    Point lower_left;
    Point upper_right;
    Point lower_right;
    Point upper_left;

    Rectangle() : lower_left(Point()), upper_right(Point()), lower_right(Point()), upper_left(Point()) {}
    Rectangle(Point lower_left, Point upper_right) : lower_left(lower_left), upper_right(upper_right), lower_right(Point(upper_right.x,lower_left.y)), upper_left(Point(lower_left.x,upper_right.y)) {}
    Rectangle(float width, float height) : lower_left(Point(-width/2, -height/2)), upper_right(Point(width/2, height/2)), lower_right(Point(width/2, -height/2)), upper_left(Point(-width/2, height/2)) {}

    bool collides_with(const Rectangle& other_rectangle) {
        if (lower_left.is_lower_left(other_rectangle.lower_left) && upper_right.is_upper_right(other_rectangle.lower_left)) {
            return true;
        }
        else if (lower_left.is_lower_left(other_rectangle.upper_right) && upper_right.is_upper_right(other_rectangle.upper_right)) {
            return true;
        }
        else if (lower_left.is_lower_left(other_rectangle.lower_right) && upper_right.is_upper_right(other_rectangle.lower_right)) {
            return true;
        }
        else if (lower_left.is_lower_left(other_rectangle.upper_left) && upper_right.is_upper_right(other_rectangle.upper_left)) {
            return true;
        }
        else if (other_rectangle.lower_left.is_lower_left(lower_left) && other_rectangle.upper_right.is_upper_right(lower_left)) {
            return true;
        }
        else if (other_rectangle.lower_left.is_lower_left(upper_right) && other_rectangle.upper_right.is_upper_right(upper_right)) {
            return true;
        }
        else if (other_rectangle.lower_left.is_lower_left(lower_right) && other_rectangle.upper_right.is_upper_right(lower_right)) {
            return true;
        }
        else if (other_rectangle.lower_left.is_lower_left(upper_left) && other_rectangle.upper_right.is_upper_right(upper_left)) {
            return true;
        }        
        else {
            return false;
        }
    }

};

#endif

#ifndef SCHEDULED_INSTRUCTION
#define SCHEDULED_INSTRUCTION

enum ScheduledInstruction {
    NOTHING,
    HANDLE_MOVING_HORIZONTALLY,
    HANDLE_STOP,
    HANDLE_AIRBORNE,
    HANDLE_FREEFALL,
    HANDLE_SLIDE_DOWN,
    HANDLE_JUMP,
    HANDLE_PICK_UP_ITEM,
    HANDLE_DROP_ITEM,
    HANDLE_SWITCH_TO_NEXT_ITEM,
    HANDLE_ATTACK,
    HANDLE_CHARGE_ATTACK,
    HANDLE_REVERT_TO_DEFAULT
};

#endif

#ifndef SINGLE_STATE_PERSISTENCE
#define SINGLE_STATE_PERSISTENCE

struct SingleStatePersistance {
    int moving_horizontally_timer = 0; // add slide down timer
    int airborne_timer = 0;
    int freefall_timer = 0;
};

#endif

#ifndef OBJECT_SPECIFIC_PHYSICS_CHAR
#define OBJECT_SPECIFIC_PHYSICS_CHAR

struct ObjectSpecificPhysicsChar {
    float max_upward_v;
    float max_downward_v;
    float max_horizontal_v;
    float horizontal_acc;
    int max_airborne_accelerable_period;

    ObjectSpecificPhysicsChar() :
        max_upward_v(235.0), max_downward_v(700.0), max_horizontal_v(225.0), horizontal_acc(215.0), max_airborne_accelerable_period(90) {}

};

#endif

#ifndef DEPENDENCY_STATE
#define DEPENDENCY_STATE

enum DependencyState {
    DEPENDENT,
    INDEPENDENT
};

#endif