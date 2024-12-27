#include <iostream>
#include <SDL2/SDL.h>
#include <optional>
#include <functional>
#include <list>
#include <memory>
#include <variant>
#include "engine_clock.h"

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

    float distanceFromOther(const Point& otherPoint) const;

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
    TRUEST_OF_PHANTOMS,
    PHANTOM,
    LIGHT_PHANTOM,
    SOLID,
    PHANTOM_SOLID
};

#endif

#ifndef STATE_H
#define STATE_H

enum State {
    IDLE = 1, 
    IDLE_LEFT = 3,
    IDLE_RIGHT = 2,
    MOVING_LEFT = 6,
    MOVING_RIGHT = 5,
    QUICKLY_M_LEFT = 9,
    QUICKLY_M_RIGHT = 8,
    SLOWLY_M_LEFT = 12,
    SLOWLY_M_RIGHT = 11,
    AIRBORNE_LEFT = 15,
    AIRBORNE_RIGHT = 14,
    FREEFALL_LEFT = 18,
    FREEFALL_RIGHT = 17,
    SLIDE_DOWN_LEFT = 21,
    SLIDE_DOWN_RIGHT = 20,
    PUSHED_LEFT = 24,
    PUSHED_RIGHT = 23,

    OWNED = 4,
    OWNED_LEFT = 27,
    OWNED_RIGHT = 26,
    IN_USE = 7,
    IN_USE_LEFT = 30,
    IN_USE_RIGHT = 29,
    IN_RECOVERY_LEFT = 33,
    IN_RECOVERY_RIGHT = 32,
    IN_WIND_UP_LEFT = 36,
    IN_WIND_UP_RIGHT = 35
};

std::ostream& operator<<(std::ostream& os, State state);

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
    HANDLE_BE_PUSHED_HORIZONTALLY_WITH_RETRY,
    HANDLE_BE_PUSHED_HORIZONTALLY_NO_RETRY,
    HANDLE_MOVE_HORIZONTALLY,
    HANDLE_SLIDE_DOWN_WITH_RETRY,
    HANDLE_SLIDE_DOWN_NO_RETRY,
    HANDLE_AIRBORNE,
    HANDLE_FREEFALL,
    HANDLE_STOP,
    HANDLE_JUMP,
    HANDLE_FOREVER_FREEFALL,
    HANDLE_SLIDE_OFF_TOP,
    HANDLE_ESCAPE_WITH_RETRY,
    HANDLE_ESCAPE_NO_RETRY,

    HANDLE_INTERACT,

    HANDLE_ATTACK,
    HANDLE_WIND_UP,
    HANDLE_RECOVER,

    HANDLE_USE,
    HANDLE_ALT_USE,
    HANDLE_DROP_ITEM,
    HANDLE_SWITCH_TO_NEXT_ITEM
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
    float maxSRVerticalV;
    float maxSRHorizontalV;
    float maxTrueVerticalV;
    float maxTrueHorizontalV;
    float horizontalAcc;
    float slideV;
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

#ifndef PLAYER_ACTION_REQ
#define PLAYER_ACTION_REQ

struct PlayerActionReq {
    bool moveLeft = false;
    bool moveRight = false;

    bool sprintModifier = false;
    bool slowWalkModifier = false;

    bool jump = false;

    bool interact = false;
    bool switchToNextItem = false;
    bool dropItem = false;
};

#endif

#ifndef PLAYER_SPECIFIC_KEY_MAPPING_H
#define PLAYER_SPECIFIC_KEY_MAPPING_H

struct PlayerSpecificKeyMapping {
    int moveLeftMapped;
    int moveRightMapped;

    int sprintModifierMapped;
    int slowWalkModifierMapped;

    int jumpMapped;

    int interactMapped;
    int switchToNextItemMapped;
    int dropItemMapped;

    PlayerSpecificKeyMapping();
    PlayerActionReq buildPlayerActionReq(const Uint8* keyboardState) const;
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

#ifndef MOMENTUM_DICTATED
#define MOMENTUM_DICTATED

struct MomentumDictated {
    float receivedHVelocity;
    float receivedExplicitHTranslation;
    float cumultativeReceivedMomentum;
    float maxReceivedPosExplicitHTranslation;
    float maxReceivedNegExplicitHTranslation;

    MomentumDictated();
    void clear();
    bool isEmpty() const;
};

#endif

#ifndef HANDLE_PARAMS
#define HANDLE_PARAMS

struct HandleParams {
    float paramSx = 0;
    bool retry = false;
};

#endif

#ifndef ITEM_DEPENDENCY_STATE
#define ITEM_DEPENDENCY_STATE

enum ItemDependencyState {
    DEPENDENT,
    INDEPENDENT
};

#endif

#ifndef CREATURE_GAME_STATS
#define CREATURE_GAME_STATS

using PassiveEffect = std::function<bool(void*)>;

struct CreatureGameStats {
    std::pair<unsigned int, unsigned int> lastPoiseSubtraction;
    std::pair<unsigned int, unsigned int> lastPoiseRegen;
    unsigned int waitSinceLastSubPoise; // in frames
    int currentPoiseHealth;
    int maxPoiseHealth;
    int poiseRegenAmount;
    int poiseRegenTick;

    std::pair<unsigned int, unsigned int> lastStaminaSubtraction;
    std::pair<unsigned int, unsigned int> lastStaminaRegen;
    unsigned int waitSinceLastSubStamina; // in frames
    int currentStamina;
    int maxStamina;
    int staminaRegenAmount;
    int staminaRegenTick;

    std::list<PassiveEffect> passiveEffects;

    CreatureGameStats(int maxPoiseHealth = 100, int maxStamina = 100);
    CreatureGameStats(const CreatureGameStats& otherCreatureGameStats);

    // returns true if poise broken
    bool subtractFromPoiseHealth(int subtractAmount, const EngineClock& sessionClock);
    void addToPoiseHealth(int addAmount);

    // returns true if depletes stamina
    bool subtractFromStamina(int subtractAmount, const EngineClock& sessionClock);
    void addToStamina(int addAmount);

    void runPoiseRegening(const EngineClock& sessionClock);
    void runStaminaRegening(const EngineClock& sessionClock);
    
    void addPassiveEffect(PassiveEffect& newPassiveEffect);
    void runPassiveEffects(const EngineClock& sessionEngine);

};

#endif

#ifndef STAMINA_DRAIN_PROTOCOL
#define STAMINA_DRAIN_PROTOCOL

struct StaminaDrainProtocol {
    int slowWalkStaminaDrainAmount;
    int slowWalkStaminaDrainTick;
    std::pair<unsigned int, unsigned int> lastSlowWalkStaminaSubtraction;

    int regularWalkStaminaDrainAmount;
    int regularWalkStaminaDrainTick;
    std::pair<unsigned int, unsigned int> lastRegularWalkStaminaSubtraction;

    int sprintStaminaDrainAmount;
    int sprintStaminaDrainTick;
    std::pair<unsigned int, unsigned int> lastSprintStaminaSubtracion;

    int jumpStaminaDrainAmount;
    // int staminaDodgeAmount;

    StaminaDrainProtocol(int sprintStaminaDrainAmount = 5, int sprintStaminaDrainTick = 15,
                         int jumpStaminaDrainAmount = 15);
    StaminaDrainProtocol(const StaminaDrainProtocol& otherStaminaDrainProtocol);                     

    // returns true if action would deplete stamina
    bool drainStaminaForSlowWalk(CreatureGameStats& creatureGameStats, const EngineClock& sessionClock);     
    bool drainStaminaForRegularWalk(CreatureGameStats& creatureGameStats, const EngineClock& sessionClock);
    bool drainStaminaForSprint(CreatureGameStats& cretureGameStats, const EngineClock& sessionClock);
    bool drainStaminaForJump(CreatureGameStats& creatureGameStats, const EngineClock& sessionClock);                

};

#endif

#ifndef CREATURE_GAME_STATS_RET_WRAPPER
#define CREATURE_GAME_STATS_RET_WRAPPER

struct CreatureGameStatsRetWrapper {
    const std::pair<unsigned int, unsigned int>& lastPoiseSubtraction;
    const std::pair<unsigned int, unsigned int>& lastPoiseRegen;
    unsigned int& waitSinceLastSubPoise; // in frames
    const int& currentPoiseHealth;
    int& maxPoiseHealth;
    int& poiseRegenAmount;
    int& poiseRegenTick;

    const std::pair<unsigned int, unsigned int>& lastStaminaSubtraction;
    const std::pair<unsigned int, unsigned int>& lastStaminaRegen;
    unsigned int& waitSinceLastSubStamina; // in frames
    const int& currentStamina;
    int& maxStamina;
    int& staminaRegenAmount;
    int& staminaRegenTick;

    const std::list<PassiveEffect>& passiveEffects;

    CreatureGameStatsRetWrapper(const CreatureGameStats& creatureGameStats);
};


#endif

#ifndef FIREARM_FIRE_SPECS
#define FIREARM_FIRE_SPECS

struct FirearmFireSpecs {
    int numberOfBullets;
    int perBulletDamage;
    std::vector<int> bulletSpreadFromCenter;
    std::variant<std::vector<int>, int> bulletTravelDistance;
    std::variant<std::vector<bool>, bool> bulletPierce;

    FirearmFireSpecs(int numberOfBullets, int perBulletDamage, std::vector<int> bulletSpreadFromCenter = {0}, 
                     int bulletTravelDistance = 550, bool bulletPierce = false);

};

#endif
