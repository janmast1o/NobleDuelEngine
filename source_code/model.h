#ifndef MODEL_H
#define MODEL_H

#include <SDL2/SDL.h>
#include "structs_and_enums.cpp"
#include "hitbox.h"

class ModelCycle;
class Hitbox;

class Model {

private:

    ModelCycle* modelCycleContainer_;
    SDL_Texture* texture_;
    Hitbox* hitboxPtr_;
    Rectangle relativeRectangle_;

public:

    Model(SDL_Texture* texture, Hitbox* hitboxPtr);
    Model(SDL_Texture* texture, Hitbox* hitboxPtr, const Point textureULRelativeToCenter);
    Model(const Model& otherModel);
    
    void setModelCycleContainer(ModelCycle* newModelCycleContainer);

    Point* getCurrentOwnerCenterPtr();

    SDL_Texture* getTexture() const;
    float getModelWidth() const;
    float getModelHeight() const;
    Point& getTextureRelativeUL();
    Hitbox* getHitboxPtr();

};

#endif