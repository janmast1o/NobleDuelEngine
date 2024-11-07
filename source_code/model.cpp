#include "model.h"
#include "hitbox.h"
#include "model_cycle.h"

Model::Model(SDL_Texture* texture, Hitbox* hitboxPtr) : texture_(texture), hitboxPtr_(hitboxPtr) {
    int iwidth, iheight;
    SDL_QueryTexture(texture, NULL, NULL, &iwidth, &iheight);
    relativeRectangle_ = Rectangle((float) iwidth, float(iheight));
    hitboxPtr_->setModelOwner(this);
    modelCycleContainer_ = nullptr;
}


Model::Model(SDL_Texture* texture, Hitbox* hitboxPtr, const Point textureULRelativeToCenter) : texture_(texture), hitboxPtr_(hitboxPtr) {
    int iwidth, iheight;
    SDL_QueryTexture(texture, NULL, NULL, &iwidth, &iheight);
    float fwidth = (float) iwidth;
    float fheight = (float) fheight;
    relativeRectangle_ = Rectangle(Point(0,-fheight) + textureULRelativeToCenter, Point(fwidth,0) + textureULRelativeToCenter);
    hitboxPtr_->setModelOwner(this);
    modelCycleContainer_ = nullptr;
}


Model::Model(const Model& otherModel) : 
    texture_(otherModel.texture_),
    hitboxPtr_(otherModel.hitboxPtr_),
    relativeRectangle_(otherModel.relativeRectangle_) {}


void Model::setModelCycleContainer(ModelCycle* newModelCycleContainer) {
    modelCycleContainer_ = newModelCycleContainer;
}


Point* Model::getCurrentOwnerCenterPtr() {
    if (modelCycleContainer_ != nullptr) {
        return modelCycleContainer_->getCurrentOwnerCenterPtr();
    } else {
        return nullptr;
    }
}


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


Hitbox* Model::getHitboxPtr() {
    return hitboxPtr_;
}