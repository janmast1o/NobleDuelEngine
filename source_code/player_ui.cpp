#include "player_ui.h"
#include "player.h"


PlayerUi::PlayerUi() : renderer_(nullptr), playerPtr_(nullptr) {}


PlayerUi::PlayerUi(SDL_Renderer* renderer, const Player* playerPtr, 
                   Point healthBoxUpperLeft, float healthBoxLength, float healthBoxHeight, float healthBarMargin,
                   Point staminaBoxUpperLeft, float staminaBoxLength, float staminaBoxHeight, float staminaBarMargin) :
                   renderer_(renderer), playerPtr_(playerPtr), healthBarMargin_(healthBarMargin), staminaBarMargin_(staminaBarMargin) {
    maxHealthBox_.w = healthBoxLength;
    maxHealthBox_.h = healthBoxHeight;
    maxHealthBox_.x = healthBoxUpperLeft.x;
    maxHealthBox_.y = healthBoxUpperLeft.y;
    maxStaminaBox_.w = staminaBoxLength;
    maxStaminaBox_.h = staminaBoxHeight;
    maxStaminaBox_.x = staminaBoxUpperLeft.x;
    maxStaminaBox_.y = staminaBoxUpperLeft.y;
}


Player* PlayerUi::getPlayerPtr() const {
    return const_cast<Player*>(playerPtr_);
}


void PlayerUi::setPlayerPtr(const Player* newPlayerPtr) {
    playerPtr_ = newPlayerPtr;
}


void PlayerUi::redrawBars() {
    if (playerPtr_ == nullptr) return;
    else if (!playerPtr_->isAlive()) {
        playerPtr_ = nullptr;
        return;
    }

    SDL_FRect redHealthBarBox = maxHealthBox_;
    redHealthBarBox.x += healthBarMargin_;
    redHealthBarBox.y += healthBarMargin_;
    redHealthBarBox.w = (maxHealthBox_.w-2*healthBarMargin_)*((float) playerPtr_->getHealth() / (float) playerPtr_->getMaxHealth());
    redHealthBarBox.h -= 2*healthBarMargin_;

    SDL_FRect greenStaminaBarBox_ = maxStaminaBox_;
    greenStaminaBarBox_.x += staminaBarMargin_;
    greenStaminaBarBox_.y += staminaBarMargin_;
    greenStaminaBarBox_.w = (maxStaminaBox_.w-2*staminaBarMargin_)*
                             ((float) playerPtr_->getCreatureGameStatsAsPtr()->currentStamina / (float) playerPtr_->getCreatureGameStatsAsPtr()->maxStamina);
    greenStaminaBarBox_.h -= 2*staminaBarMargin_;

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderFillRectF(renderer_, &maxHealthBox_);

    SDL_SetRenderDrawColor(renderer_, 225, 0, 0, 255);
    SDL_RenderFillRectF(renderer_, &redHealthBarBox);;

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderFillRectF(renderer_, &maxStaminaBox_);

    SDL_SetRenderDrawColor(renderer_, 0, 205, 0, 255);
    SDL_RenderFillRectF(renderer_, &greenStaminaBarBox_);
}


