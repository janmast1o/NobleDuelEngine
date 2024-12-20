#include "model.h"
#include "hitbox.h"
#include "model_cycle.h"

Model::Model(SDL_Texture* texture, Hitbox* hitboxPtr) : texture_(texture), hitboxPtr_(hitboxPtr) {
    int iwidth, iheight;
    SDL_QueryTexture(texture, NULL, NULL, &iwidth, &iheight);
    relativeRectangle_ = Rectangle((float) iwidth, (float) iheight);
    itemGripPointRelativeToCenter_ = {0, 0};
}


Model::Model(SDL_Texture* texture, Hitbox* hitboxPtr, const Point textureULRelativeToCenter) : texture_(texture), hitboxPtr_(hitboxPtr) {
    int iwidth, iheight;
    SDL_QueryTexture(texture, NULL, NULL, &iwidth, &iheight);
    float fwidth = (float) iwidth;
    float fheight = (float) iheight;
    relativeRectangle_ = Rectangle(Point(0,-fheight) + textureULRelativeToCenter, Point(fwidth,0) + textureULRelativeToCenter);
    itemGripPointRelativeToCenter_ = {0, 0};
}


Model::Model(const Model& otherModel) : 
    texture_(otherModel.texture_),
    hitboxPtr_(otherModel.hitboxPtr_),
    relativeRectangle_(otherModel.relativeRectangle_),
    itemGripPointRelativeToCenter_(otherModel.itemGripPointRelativeToCenter_) {}


SDL_Texture* Model::getTexture() const {
    return texture_;
}


float Model::getModelTextureWidth() const {
    return relativeRectangle_.upperRight.x - relativeRectangle_.lowerLeft.x;
}


float Model::getModelTextureHeight() const {
    return relativeRectangle_.upperRight.y - relativeRectangle_.lowerLeft.y;
}


Point Model::getItemGripPointRelativeToCenter() const {
    return itemGripPointRelativeToCenter_;
}


void Model::setItemGripPointRelativeToCenter(Point newItemGripPointRelativeToCenter) {
    itemGripPointRelativeToCenter_ = newItemGripPointRelativeToCenter;
}


Point& Model::getTextureRelativeUL() {
    return relativeRectangle_.upperLeft;
}


Hitbox* Model::getHitboxPtr() const {
    return hitboxPtr_;
}