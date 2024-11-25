#include "player.h"
#include "constants.h"

Player::Player(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, ObjectMap& objectMap, float mass, int health) : 
    Creature(renderer, center, modelCollection, objectMap, mass, health) {
        previousKeyboardState_[keyMapping_.interact] = 0;
        previousKeyboardState_[keyMapping_.switchToNextItem] = 0;
        previousKeyboardState_[keyMapping_.dropItem] = 0;
    }


void Player::rememberPreviousKeyboardState(const Uint8* keyboardState) {
    previousKeyboardState_[keyMapping_.interact] = keyboardState[keyMapping_.interact];
    previousKeyboardState_[keyMapping_.switchToNextItem] = keyboardState[keyMapping_.switchToNextItem];
    previousKeyboardState_[keyMapping_.dropItem] = keyboardState[keyMapping_.dropItem];
}    


void Player::bufferInputs(const Uint8* keyboardState) {
    for (auto& bp : bufferedInputs_) {
        if (keyboardState[bp.first]) {
            bp.second = BUF_FRAME_TIME+1;
        }
    }
}


void Player::decrementBufferTimers() {
    for (auto& bp : bufferedInputs_) {
        bp.second--;
        if (bp.second < 0) {
            bp.second = 0;
        }
    }
}


void Player::zeroBuffers() {
    for (auto& bp : bufferedInputs_) {
        bp.second = 0;
    }
}


bool Player::isOnPressRequested(const int action, const Uint8* currentKeyboardState) {
    if (currentKeyboardState[action]) {
        if (bufferedInputs_.find(action) != bufferedInputs_.end()) {
            zeroBuffers();
        }
        return true;
    } else {
        if (bufferedInputs_.find(action) != bufferedInputs_.end() && bufferedInputs_[action] > 0) {
            zeroBuffers();
            return true;
        } else {
            return false;
        }
    }
}


bool Player::isOnReleaseRequested(const int action, const Uint8* currentKeyboardState) {
    if (!currentKeyboardState[action]) {
        if (previousKeyboardState_.find(action) != previousKeyboardState_.end() && previousKeyboardState_[action]) {
            return true;
        } else if (bufferedInputs_.find(action) != bufferedInputs_.end() && bufferedInputs_[action] > 0) {
            zeroBuffers();
            return true;
        }
    }
    return false;
}


void Player::addBufferable(const int newBufferable) {
    bufferedInputs_[newBufferable] = 0;
}


void Player::removeBufferable(const int bufferable) {
    bufferedInputs_.erase(bufferable);
}


void Player::clearBufferables() {
    bufferedInputs_.clear();
}


void Player::readInputs(const Uint8* newKeyboardState) {
    if (!isAnythingScheduled()) {
        if (isOnPressRequested(keyMapping_.moveLeft, newKeyboardState)) {
            if (!isOnPressRequested(keyMapping_.moveRight, newKeyboardState)) {
                adjustAccAndVForRegular();
                if (isOnPressRequested(keyMapping_.sprintModifier, newKeyboardState)) {
                    adjustAccAndVForSprint();
                } else if (isOnPressRequested(keyMapping_.slowWalkModifier, newKeyboardState)) {
                    adjustAccAndVForSlowWalk();
                }
                newHorizontalAcceleration(LEFT);

                if (isOnPressRequested(keyMapping_.jump, newKeyboardState)) {
                    setScheduled(HANDLE_JUMP);
                } else {
                    setScheduled(HANDLE_MOVE_HORIZONTALLY);
                }
            } else {
                setScheduled(HANDLE_STOP);
            }
        } else if (isOnPressRequested(keyMapping_.moveRight, newKeyboardState)) {
            if (!isOnPressRequested(keyMapping_.moveLeft, newKeyboardState)) {
                adjustAccAndVForRegular();
                if (isOnPressRequested(keyMapping_.sprintModifier, newKeyboardState)) {
                    adjustAccAndVForSprint();
                } else if (isOnPressRequested(keyMapping_.slowWalkModifier, newKeyboardState)) {
                    adjustAccAndVForSlowWalk();
                }
                newHorizontalAcceleration(RIGHT);

                if (isOnPressRequested(keyMapping_.jump, newKeyboardState)) {
                    setScheduled(HANDLE_JUMP);
                } else {
                    setScheduled(HANDLE_MOVE_HORIZONTALLY);
                }
            } else {
                setScheduled(HANDLE_STOP);
            }
        } else if (isOnPressRequested(keyMapping_.jump, newKeyboardState)) {
            setScheduled(HANDLE_JUMP);
        } else if (getPreviouslyScheduled() != HANDLE_STOP) {
            setScheduled(HANDLE_STOP);
        }

        rememberPreviousKeyboardState(newKeyboardState);
        decrementBufferTimers();        
    
    } else {
        rememberPreviousKeyboardState(newKeyboardState);
        bufferInputs(newKeyboardState);
        decrementBufferTimers();
    }
    
}