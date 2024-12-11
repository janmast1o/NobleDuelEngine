#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"

class Player : public Creature {

private:

    PlayerSpecificKeyMapping keyMapping_;
    std::unordered_map<int, Uint8> previousKeyboardState_;
    std::unordered_map<int, int> bufferedInputs_;

    // PlayerActionReq previousPlayerReqs_;
    // PlayerActionReq bufferedInputs_;

    void rememberPreviousKeyboardState(const Uint8* keyboardState);
    // void rememberPreviousPlayerReqs(const PlayerActionReq playerActionReq);
    void bufferInputs(const Uint8* keyboardState);
    void decrementBufferTimers();
    void zeroBuffers();

    bool isOnPressRequested(const int action, const Uint8* currentKeybaordState);
    bool isOnReleaseRequested(const int action, const Uint8* currentKeyboardState);

public:

    Player(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
           int health, InteractableManager& interactableManager);

    PlayerSpecificKeyMapping& getKeyMappingRef();
    
    void addBufferable(const int newBufferable);
    void removeBufferable(const int bufferable);
    void clearBufferables();

    void readInputs(const Uint8* newKeyboardState);

    virtual ~Player() = default;

};

#endif