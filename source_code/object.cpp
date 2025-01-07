#include "object.h"
#include "model.h"
#include "hitbox.h"
#include "utility_functions.h"
#include <cmath>
#include <vector>
#include <unordered_set>

const EngineClock Object::dummyEngineClock_ = EngineClock();

Object::Object() : sessionEngineClock_(dummyEngineClock_) {}

Object::Object(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock) :
    renderer_(renderer), center_(center), modelCollection_(modelCollection), sessionEngineClock_(sessionEngineClock) {
        health_ = INFINITY;
        maxHealth_ = INFINITY;
        matter_ = SOLID;
        state_ = IDLE;
        previousState_ = IDLE;
        damageReceiveState_ = INVULNERABLE;
        registerOwnerCenterPtrForHitboxes();
        modelCollection_.determineLargestRectangle();
    }


void Object::registerOwnerCenterPtrForHitboxes() {
    modelCollection_.registerOwnerCenterPtrForHitboxes(&center_);
}


bool Object::isParticipatingInCollisions() const {
    return true;
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


Rectangle Object::getLargestRectangle() const {
    return modelCollection_.getLargestRectangle();
}


Rectangle Object::getLargestRectangle() {
    return modelCollection_.getLargestRectangle();
}


Hitbox& Object::getCurrentCollisionMesh() const {
    return modelCollection_.getCurrentCollisionMesh(state_);
}


Hitbox& Object::getCurrentHitbox() const {
    return modelCollection_.getCurrentHitbox(state_);
}


Point Object::getCurrentItemGripPoint() const {
    return modelCollection_.getItemGripPointRelativeToCenter(state_) + center_;
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
    // std::cout << health_ << std::endl;
}


void Object::addToHealth(int addAmount) {
    health_ += addAmount;
    if (health_ > maxHealth_) {
        health_ = maxHealth_;
    }
}


int Object::getMaxHealth() const {
    return maxHealth_;
}


void Object::setMaxHealth(int newMaxHealth) {
    maxHealth_ = newMaxHealth;
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


void Object::redrawObject(const Rectangle& currentlyObservedRectangle) {
    Model* model = getNextModelPtr();
    if (model != nullptr && model->getTexture() != nullptr &&
        currentlyObservedRectangle.collidesWith(model->getRelativeRectangle()+center_)) {

        SDL_FRect destRect;
        destRect.w = model->getModelTextureWidth();
        destRect.h = model->getModelTextureHeight();
        destRect.x = model->getTextureRelativeUL().x + center_.x;
        destRect.y = model->getTextureRelativeUL().y + center_.y;
        destRect.x -= currentlyObservedRectangle.upperLeft.x;
        destRect.y -= currentlyObservedRectangle.upperLeft.y;
        destRect.y *= -1;

        SDL_RenderCopyF(renderer_, model->getTexture(), NULL, &destRect);
    } 
}


bool Object::collideableWith(const Object& otherObject) const {
    if (!otherObject.isParticipatingInCollisions()) {
        return false;
    } else if (matter_ == PHANTOM_SOLID || otherObject.matter_ == PHANTOM_SOLID) {
        return true;
    } else if (matter_ == TRUEST_OF_PHANTOMS || otherObject.matter_ == TRUEST_OF_PHANTOMS) {
        return false;
    } else if (matter_ == SOLID) {
        if (otherObject.matter_ == SOLID) {
            return true;
        } else {
            return false;
        }
    } else if (matter_ == PHANTOM) {
        if (otherObject.matter_ == PHANTOM) {
            return true;
        } else {
            return false;
        }
    } else if (matter_ == LIGHT_PHANTOM) {
        if (otherObject.matter_ == PHANTOM) {
            return false;
        } else {
            return true;
        }
    }

    return true;
}


bool Object::collideableWith(const Object& otherObject) {
    return static_cast<const Object*>(this)->collideableWith(otherObject);
}


bool Object::canHaveOtherOnTop() const {
    return true;
}


bool Object::isParticipatingInMomentum() const {
    return false;
}


void Object::registerBeingAffectedByOutsideMomentum(float otherObjectMass, float otherObjectHVelocity, float hTranslation) {;}


void Object::replaceAllHitboxPtrsWithStaticCopies(std::list<Hitbox>& saveContainer) {
    modelCollection_.replaceAllHitboxPtrsWithStaticCopies(saveContainer, &center_);
}