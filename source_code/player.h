#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"

class Player : public Creature {

private:

    PlayerSpecificKeyMapping keyMapping_;
    std::unordered_map<int, Uint8> previousKeyboardState_;
    std::unordered_map<int, int> bufferedInputs_;

    bool leftMouseButtonPreviouslyPressed_;
    bool rightMouseButtonPreviouslyPressed_;
    int leftMouseButtonBuffer_;
    int rightMouseButtonBuffer_;

    void rememberPreviousKeyboardState(const Uint8* keyboardState, bool leftMouseButtonPressed, bool rightMouseButtonPressed);
    void bufferInputs(const Uint8* keyboardState, bool leftMouseButtonPressed, bool rightMouseButtonPressed);
    void decrementBufferTimers();
    void zeroBuffers();

    bool isOnPressRequested(const int action, const Uint8* currentKeybaordState);
    bool isOnReleaseRequested(const int action, const Uint8* currentKeyboardState);

    bool isLMBOnReleaseRequested(bool leftMouseButtonPressed);
    bool isRMBOnReleaseRequested(bool rightMouseButtonPressed);

public:

    Player(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
           int health, InteractableManager& interactableManager);

    PlayerSpecificKeyMapping& getKeyMappingRef();
    
    void addBufferable(const int newBufferable);
    void removeBufferable(const int bufferable);
    void clearBufferables();

    void readInputs(const Uint8* newKeyboardState, bool leftMouseButtonPressed, bool rightMouseButtonPressed);

    virtual ~Player() = default;

};

#endif