#include "object.h"
#include "model.h"
#include "hitbox.h"
#include "utility_functions.h"
#include <cmath>
#include <vector>
#include <unordered_set>

Object::Object(SDL_Renderer* renderer, Point center, ModelCollection modelCollection) :
    renderer_(renderer), center_(center), modelCollection_(modelCollection) {
        health_ = INFINITY;
        matter_ = SOLID;
        state_ = IDLE;
        previousState_ = IDLE;
        damageReceiveState_ = INVULNERABLE;
        registerOwnerCenterPtrForHitboxes();
    }


void Object::registerOwnerCenterPtrForHitboxes() {
    modelCollection_.registerOwnerCenterPtrForHitboxes(&center_);
}


bool Object::isMobile() const {
    return false;
}


Model* Object::getNextModelPtr() {
    if (previousState_ == state_) {
        return modelCollection_.getNewModelPtrForState(state_);
    } else {
        return modelCollection_.getFirstModelPtrForState(state_);
    }
}


Hitbox& Object::getCurrentCollisionMesh() const {
    return modelCollection_.getCurrentCollisionMesh(state_);
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


bool Object::isAlive() const {
    return health_ > 0;
}


Point& Object::getCenter() {
    return center_;
}


void Object::setCenter(const Point& newCenter) {
    center_.x = newCenter.x;
    center_.y = newCenter.y;
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


std::list<int> Object::getAllTiedHitboxes() const {
    return modelCollection_.getAllTiedHitboxes();
}


void Object::redrawObject() {
    SDL_FRect destRect;
    Model* model = getNextModelPtr();
    if (model != nullptr) {
        destRect.w = model->getModelTextureWidth();
        destRect.h = model->getModelTextureHeight();
        destRect.x = model->getTextureRelativeUL().x + center_.x;
        destRect.y = -(model->getTextureRelativeUL().y + center_.y);
        SDL_RenderCopyF(renderer_, model->getTexture(), NULL, &destRect);
    }
}


void Object::redrawObject(bool drawHitboxes, float pointSize) {
    SDL_FRect destRect;
    Model* model = getNextModelPtr();
    if (model != nullptr && model->getTexture() != nullptr) {
        destRect.w = model->getModelTextureWidth();
        destRect.h = model->getModelTextureHeight();
        destRect.x = model->getTextureRelativeUL().x + center_.x;
        destRect.y = -(model->getTextureRelativeUL().y + center_.y);
        SDL_RenderCopyF(renderer_, model->getTexture(), NULL, &destRect);
        if (drawHitboxes) {
            std::vector<Point> hull = model->getHitboxPtr()->getCurrentHull();
            int n = hull.size();
            for (int i=0; i<n; ++i) {
                drawPoint(renderer_, hull[i].x, -hull[i].y, pointSize);
            }
        }
    }
}


bool Object::collideableWith(const Object& otherObject) {
    if (matter_ == PHANTOM_SOLID || otherObject.matter_ == PHANTOM) {
        return true;
    }
    else if (matter_ == SOLID) {
        if (otherObject.matter_ == SOLID) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (matter_ == PHANTOM) {
        if (otherObject.matter_ == PHANTOM) {
            return true;
        }
        else {
            return false;
        }
    }
    else if (matter_ == LIGHT_PHANTOM) {
        if (otherObject.matter_ == PHANTOM) {
            return false;
        }
        else {
            return true;
        }
    }
    return true;
}


bool Object::canHaveOtherOnTop() const {
    return true;
}


bool Object::participatingInMomentum() const {
    return false;
}


void Object::registerBeingAffectedByOutsideMomentum(float otherObjectMass, float otherObjectHVelocity, float hTranslation) {;}