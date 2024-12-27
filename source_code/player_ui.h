#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include <SDL2/SDL.h>
#include "structs_and_enums.h"

class Player;

class PlayerUi {

private:

    SDL_Renderer* renderer_;
    const Player* playerPtr_;
    SDL_FRect maxHealthBox_;
    SDL_FRect maxStaminaBox_;
    float healthBarMargin_;
    float staminaBarMargin_;

public:

    PlayerUi();
    PlayerUi(SDL_Renderer* renderer, const Player* playerPtr, 
             Point healthBoxUpperLeft, float healthBoxLength, float healthBoxHeight, float healthBarMargin,
             Point staminaBoxUpperLeft, float staminaBoxLength, float staminaBoxHeight, float staminaBarMargin);
    Player* getPlayerPtr() const;
    void setPlayerPtr(const Player* newPlayerPtr);
    void redrawBars();
    virtual ~PlayerUi() = default;

};


#endif