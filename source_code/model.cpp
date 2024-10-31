#include "model.h"
#include "hitbox.h"

Model::Model(SDL_Texture* texture, Hitbox hitbox) : texture_(texture), hitbox_(hitbox) {
    int iwidth, iheight;
    SDL_QueryTexture(texture, NULL, NULL, &iwidth, &iheight);
    relativeRectangle_ = Rectangle((float) iwidth, float(iheight));
    hitbox_.setModelOwner(this);
    // owner_ = nullptr;
}


Model::Model(SDL_Texture* texture, Hitbox hitbox, const Point textureULRelativeToCenter) : texture_(texture), hitbox_(hitbox) {
    int iwidth, iheight;
    SDL_QueryTexture(texture, NULL, NULL, &iwidth, &iheight);
    float fwidth = (float) iwidth;
    float fheight = (float) fheight;
    relativeRectangle_ = Rectangle(Point(0,-fheight) + textureULRelativeToCenter, Point(fwidth,0) + textureULRelativeToCenter);
    hitbox_.setModelOwner(this);
    // owner_ = nullptr;
}


Model::Model(const Model& otherModel) : 
    texture_(otherModel.texture_),
    hitbox_(otherModel.hitbox_),
    relativeRectangle_(otherModel.relativeRectangle_) {}


// owner setting

// center getting


SDL_Texture* Model::getTexture() const {
    return texture_;
}


float Model::getModelWidth() const {
    return relativeRectangle_.upperRight.x - relativeRectangle_.lowerLeft.x;
}


float Model::getModelHeight() const {
    return relativeRectangle_.upperRight.y - relativeRectangle_.lowerLeft.y;
}


Point& Model::getTextureRelativeUL() {
    return relativeRectangle_.upperLeft;
}


Hitbox& Model::getHitbox() {
    return hitbox_;
}