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
    Point itemGripPointRelativeToCenter_;

public:

    Model(SDL_Texture* texture, Hitbox* hitboxPtr);
    Model(SDL_Texture* texture, Hitbox* hitboxPtr, const Point textureULRelativeToCenter);
    Model(const Model& otherModel);

    SDL_Texture* getTexture() const;
    float getModelTextureWidth() const;
    float getModelTextureHeight() const;
    Rectangle getRelativeRectangle() const;
    Point getItemGripPointRelativeToCenter() const;
    void setItemGripPointRelativeToCenter(Point newItemGripPointRelativeToCenter);
    Point& getTextureRelativeUL();
    Hitbox* getHitboxPtr() const;

    // Hitbox* replaceHitboxWithAStaticCopy(std::list<Hitbox>& saveContainer, Point* newHitboxNewOwnerCenterPtr);

};

#endif