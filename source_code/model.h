#ifndef MODEL_H
#define MODEL_H

#include <SDL2/SDL.h>
#include "structs_and_enums.cpp"

class Object;
class Hitbox;

class Model {

private:

    // Object* owner_;
    SDL_Texture* texture_;
    Hitbox hitbox_;
    Rectangle relativeRectangle_;

public:

    Model(SDL_Texture* texture, Hitbox hitbox);
    Model(SDL_Texture* texture, Hitbox hitbox, const Point textureULRelativeToCenter);
    Model(const Model& otherModel);
    
    // void setOwner(Object* owner);

    // Point& getCenter();

    SDL_Texture* getTexture() const;
    float getModelWidth() const;
    float getModelHeight() const;
    Point& getTextureRelativeUL();
    Hitbox& getHitbox();

};

#endif