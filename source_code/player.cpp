#include "player.h"
#include "constants.h"

Player::Player(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
               int health, InteractableManager& interactableManager) : 
               Object(renderer, center, modelCollection, sessionEngineClock),
               Creature(renderer, center, modelCollection, sessionEngineClock, objectMap, mass, health, interactableManager),
               leftMouseButtonPreviouslyPressed_(false),
               rightMouseButtonPreviouslyPressed_(false),
               leftMouseButtonBuffer_(0),
               rightMouseButtonBuffer_(0) {
        previousKeyboardState_[keyMapping_.interactMapped] = 0;
        previousKeyboardState_[keyMapping_.switchToNextItemMapped] = 0;
        previousKeyboardState_[keyMapping_.dropItemMapped] = 0;
    }


void Player::rememberPreviousKeyboardState(const Uint8* keyboardState, bool leftMouseButtonPressed, bool rightMouseButtonPressed) {
    previousKeyboardState_[keyMapping_.interactMapped] = keyboardState[keyMapping_.interactMapped];
    previousKeyboardState_[keyMapping_.switchToNextItemMapped] = keyboardState[keyMapping_.switchToNextItemMapped];
    previousKeyboardState_[keyMapping_.dropItemMapped] = keyboardState[keyMapping_.dropItemMapped];
    leftMouseButtonPreviouslyPressed_ = leftMouseButtonPressed;
    rightMouseButtonPreviouslyPressed_ = rightMouseButtonPressed;
}    


// void Player::rememberPreviousPlayerReqs(const PlayerActionReq playerActionReq) {
//     previousPlayerReqs_.interact = playerActionReq.interact;
//     previousPlayerReqs_.dropItem = playerActionReq.dropItem;
//     previousPlayerReqs_.switchToNextItem = playerActionReq.switchToNextItem;
// }


void Player::bufferInputs(const Uint8* keyboardState, bool leftMouseButtonPressed, bool rightMouseButtonPressed) {
    for (auto& bp : bufferedInputs_) {
        if (keyboardState[bp.first]) {
            bp.second = BUF_FRAME_TIME+1;
        }
    }
    if (leftMouseButtonPressed) {
        leftMouseButtonBuffer_ = BUF_FRAME_TIME+1;
    }
    if (rightMouseButtonPressed) {
        rightMouseButtonBuffer_ = BUF_FRAME_TIME+1;
    }
}


void Player::decrementBufferTimers() {
    for (auto& bp : bufferedInputs_) {
        bp.second--;
        if (bp.second < 0) {
            bp.second = 0;
        }
    }
    leftMouseButtonBuffer_ -= 1;
    if (leftMouseButtonBuffer_ < 0) leftMouseButtonBuffer_ = 0;
    rightMouseButtonBuffer_ -= 1;
    if (rightMouseButtonBuffer_ < 0) rightMouseButtonBuffer_ = 0;
}


void Player::zeroBuffers() {
    for (auto& bp : bufferedInputs_) {
        bp.second = 0;
    }
    leftMouseButtonBuffer_ = 0;
    rightMouseButtonBuffer_ = 0;
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
            zeroBuffers();
            return true;
        } else if (bufferedInputs_.find(action) != bufferedInputs_.end() && bufferedInputs_[action] > 0) {
            zeroBuffers();
            return true;
        }
    }
    return false;
}


bool Player::isLMBOnReleaseRequested(bool leftMouseButtonPressed) {
    if (!leftMouseButtonPressed && (leftMouseButtonPreviouslyPressed_ || leftMouseButtonBuffer_ > 0)) {
        zeroBuffers();
        return true;
    }
    return false;
}


bool Player::isRMBOnReleaseRequested(bool rightMouseButtonPressed) {
        if (!rightMouseButtonPressed && (rightMouseButtonPreviouslyPressed_ || rightMouseButtonBuffer_ > 0)) {
        zeroBuffers();
        return true;
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


PlayerSpecificKeyMapping& Player::getKeyMappingRef() {
    return keyMapping_;
}


void Player::readInputs(const Uint8* newKeyboardState, bool leftMouseButtonPressed, bool rightMouseButtonPressed) {
    if (!isAnythingScheduled()) {
        if (!isAnythingInteractionLikeScheduled()) {
            if (isOnReleaseRequested(keyMapping_.interactMapped, newKeyboardState)) {
                setInteractionScheduled(HANDLE_INTERACT);
            } else if (isLMBOnReleaseRequested(leftMouseButtonPressed)) {
                setInteractionScheduled(HANDLE_USE);
            } else if (isRMBOnReleaseRequested(rightMouseButtonPressed)) {
                setInteractionScheduled(HANDLE_ALT_USE);
            } else if (isOnReleaseRequested(keyMapping_.dropItemMapped, newKeyboardState)) {
                setInteractionScheduled(HANDLE_DROP_ITEM);
            } else if (isOnReleaseRequested(keyMapping_.switchToNextItemMapped, newKeyboardState)) {
                setInteractionScheduled(HANDLE_SWITCH_TO_NEXT_ITEM);
            }
        }

        if (isOnPressRequested(keyMapping_.moveLeftMapped, newKeyboardState)) {
            if (!isOnPressRequested(keyMapping_.moveRightMapped, newKeyboardState)) {
                adjustAccAndVForRegular();
                if (isOnPressRequested(keyMapping_.sprintModifierMapped, newKeyboardState)) {
                    adjustAccAndVForSprint();
                } else if (isOnPressRequested(keyMapping_.slowWalkModifierMapped, newKeyboardState)) {
                    adjustAccAndVForSlowWalk();
                }
                newHorizontalAcceleration(LEFT);

                if (isOnPressRequested(keyMapping_.jumpMapped, newKeyboardState)) {
                    setScheduled(HANDLE_JUMP);
                } else {
                    setScheduled(HANDLE_MOVE_HORIZONTALLY);
                }
            } else {
                setScheduled(HANDLE_STOP);
            }
        } else if (isOnPressRequested(keyMapping_.moveRightMapped, newKeyboardState)) {
            if (!isOnPressRequested(keyMapping_.moveLeftMapped, newKeyboardState)) {
                adjustAccAndVForRegular();
                if (isOnPressRequested(keyMapping_.sprintModifierMapped, newKeyboardState)) {
                    adjustAccAndVForSprint();
                } else if (isOnPressRequested(keyMapping_.slowWalkModifierMapped, newKeyboardState)) {
                    adjustAccAndVForSlowWalk();
                }
                newHorizontalAcceleration(RIGHT);

                if (isOnPressRequested(keyMapping_.jumpMapped, newKeyboardState)) {
                    setScheduled(HANDLE_JUMP);
                } else {
                    setScheduled(HANDLE_MOVE_HORIZONTALLY);
                }
            } else {
                setScheduled(HANDLE_STOP);
            }
        } else if (isOnPressRequested(keyMapping_.jumpMapped, newKeyboardState)) {
            setScheduled(HANDLE_JUMP);
        } else if (getPreviouslyScheduled() != HANDLE_STOP) {
            setScheduled(HANDLE_STOP);
        }

        rememberPreviousKeyboardState(newKeyboardState, leftMouseButtonPressed, rightMouseButtonPressed);
        decrementBufferTimers();        
    
    } else {
        rememberPreviousKeyboardState(newKeyboardState, leftMouseButtonPressed, rightMouseButtonPressed);
        bufferInputs(newKeyboardState, leftMouseButtonPressed, rightMouseButtonPressed);
        decrementBufferTimers();
    }
    
}