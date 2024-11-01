#include "object.h"
#include "model.h"
#include "hitbox.h"
#include "utility_functions.cpp"
#include <cmath>
#include <vector>

Object::Object(SDL_Renderer* renderer, Point center, ModelCollection modelCollection) :
    renderer_(renderer), center_(center), modelCollection_(modelCollection) {
        modelCollection_.setOwner(this);
        health_ = INFINITY;
        state_ = IDLE;
        previousState_ = IDLE;
        damageReceiveState_ = INVULNERABLE;
    }


Model* Object::getNextModelPtr() {
    ;
}    


SDL_Renderer* Object::getRenderer() const {
    return renderer_;
}    


int Object::getHealth() const {
    return health_;
}


void Object::setHealth(int newHealth) {
    health_ = newHealth;
}


void Object::subtractFromHealth(int subtractAmount) {
    health_ -= subtractAmount;
    if (health_ < 0) {
        health_ = 0;
    }
}


Point& Object::getCenter() {
    return center_;
}


void Object::setCenter(const Point& newCenter) {
    center_ = newCenter;
}


Matter Object::getMatter() const {
    return matter_;
}


void Object::setMatter(Matter matter) {
    matter_ = matter;
}


State Object::getState() const {
    return state_;
}


State Object::getPreviousState() const {
    return previousState_;
}


void Object::setNewState(State newState) {
    previousState_ = state_;
    state_ = newState;
}


void Object::redrawObject() {
    SDL_FRect destRect;
    Model* model = getNextModelPtr();
    if (model != nullptr) {
        destRect.w = model->getModelWidth();
        destRect.h = model->getModelHeight();
        destRect.x = model->getTextureRelativeUL().x + center_.x;
        destRect.y = model->getTextureRelativeUL().y + center_.y;
        SDL_RenderCopyF(renderer_, model->getTexture(), NULL, &destRect);
    }
}


void Object::redrawObject(bool drawHitboxes, float pointSize) {
    SDL_FRect destRect;
    Model* model = getNextModelPtr();
    if (model != nullptr) {
        destRect.w = model->getModelWidth();
        destRect.h = model->getModelHeight();
        destRect.x = model->getTextureRelativeUL().x + center_.x;
        destRect.y = model->getTextureRelativeUL().y + center_.y;
        SDL_RenderCopyF(renderer_, model->getTexture(), NULL, &destRect);
        if (drawHitboxes) {
            std::vector<Point> hull = model->getHitbox().getCurrentHull();
            int n = hull.size();
            for (int i=0; i<n; i++) {
                drawPoint(renderer_, hull[i].x, -hull[i].y, pointSize);
            }
        }
    }
}