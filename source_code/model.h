#ifndef MODEL_H
#define MODEL_H

#include <SDL2/SDL.h>
#include "structs_and_enums.h"
#include "hitbox.h"
#include "hitbox_container.h"

class ModelCycle;
class Hitbox;

class Model {

private:

    SDL_Texture* texture_;
    Hitbox* hitboxPtr_;
    Rectangle relativeRectangle_;

public:

    Model(SDL_Texture* texture, Hitbox* hitboxPtr);
    Model(SDL_Texture* texture, Hitbox* hitboxPtr, const Point textureULRelativeToCenter);
    Model(const Model& otherModel);

    SDL_Texture* getTexture() const;
    float getModelTextureWidth() const;
    float getModelTextureHeight() const;
    Point& getTextureRelativeUL();
    Hitbox* getHitboxPtr() const;

};

#endif