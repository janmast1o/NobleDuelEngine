#include "projectile.h"
#include "utility_functions.h"
#include "creature.h"

Projectile::Projectile(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, 
                       MobileHitbox leftTravellingHitbox, MobileHitbox rightTravellingHitbox,
                       const EngineClock& sessionEngineClock,
                       ObjectMap& objectMap, float mass, Point targetedPoint, ProjectileSpecs projectileSpecs, Creature* shooter) :
                       Object(renderer, center, modelCollection, sessionEngineClock),
                       MobileObject(renderer, center, modelCollection, sessionEngineClock, objectMap, mass),
                       projectileSpecs_(projectileSpecs), distanceCoveredSoFar_(0), shooter_(shooter),
                       leftTravellingHitbox_(leftTravellingHitbox), rightTravellingHitbox_(rightTravellingHitbox) {
    setHealth(1);
    setMatter(projectileSpecs_.matter);
    travelDirAlongXAxis_ = getSign(targetedPoint.x - getCenter().x);
    travelDirectionSlopeCoefficient_ = findSlopeCoefficient(center, targetedPoint);
    leftTravellingHitbox_.setOwnerCenterPtr(&getCenter());
    rightTravellingHitbox_.setOwnerCenterPtr(&getCenter());
}


Projectile::Projectile(const Projectile& otherProjectile) :
                       Object(otherProjectile), MobileObject(otherProjectile),
                       shooter_(otherProjectile.shooter_), 
                       travelDirAlongXAxis_(otherProjectile.travelDirAlongXAxis_),
                       travelDirectionSlopeCoefficient_(otherProjectile.travelDirectionSlopeCoefficient_),
                       projectileSpecs_(otherProjectile.projectileSpecs_),
                       distanceCoveredSoFar_(otherProjectile.distanceCoveredSoFar_),
                       hitRegistry_(otherProjectile.hitRegistry_),
                       leftTravellingHitbox_(otherProjectile.leftTravellingHitbox_),
                       rightTravellingHitbox_(otherProjectile.rightTravellingHitbox_) {

    leftTravellingHitbox_.setOwnerCenterPtr(&getCenter());
    rightTravellingHitbox_.setOwnerCenterPtr(&getCenter());
}


void Projectile::handleProjectileMovement() {
    previouslyScheduled_ = scheduled_;
    float sx = travelDirAlongXAxis_*std::abs(projectileSpecs_.travelSpeed / (FPS*std::sqrt(1+std::pow(travelDirectionSlopeCoefficient_,2))));
    float sy = sx*travelDirectionSlopeCoefficient_;
    Point svec(sx,sy);
    std::list<Object*> potentiallyColliding = objectMap_.getPotentiallyColliding(*this, svec);
    bool shouldDisappear = false;

    MobileHitbox* hitboxPtr;
    if (travelDirAlongXAxis_ > 0) hitboxPtr = &rightTravellingHitbox_;
    else hitboxPtr = &leftTravellingHitbox_;

    for (Object* p : potentiallyColliding) {
        if (p != this && p != shooter_) {
            if (hitboxPtr->collidesWithAfterVectorTranslation(p->getCurrentHitbox(), svec)) {
                if (hitRegistry_.find(p) == hitRegistry_.end()) {
                    hitRegistry_.emplace(p);
                    p->subtractFromHealth(projectileSpecs_.damage);
                }
                if (!projectileSpecs_.pierce) shouldDisappear = true;
            }
        }
    }

    distanceCoveredSoFar_ += svec.distanceFromOther({0,0});
    if (distanceCoveredSoFar_ >= projectileSpecs_.travelDistance) shouldDisappear = true;
    else translateObjectByVector(svec);

    // if (shouldDisappear) std::cout << "!!!" << std::endl;

    if (shouldDisappear) setHealth(0);
    if (travelDirAlongXAxis_ > 0) {
        setNewState(MOVING_RIGHT);
    } else {
        setNewState(MOVING_LEFT);
    }

}


Creature* Projectile::getShooter() const {
    return shooter_;
}


void Projectile::setShooter(Creature* newShooter) {
    shooter_ = newShooter;
}


int Projectile::getTravelDirAlongXAxis() const {
    return travelDirAlongXAxis_;
}


void Projectile::setTargetedPoint(Point newTargetedPoint) {
    travelDirAlongXAxis_ = getSign(newTargetedPoint.x - getCenter().x);
    travelDirectionSlopeCoefficient_ = findSlopeCoefficient(getCenter(), newTargetedPoint);
}


ProjectileSpecs Projectile::getProjectileSpecs() const {
    return projectileSpecs_;
}


void Projectile::setProjectileSpecs(ProjectileSpecs& newProjectileSpecs) {
    projectileSpecs_ = newProjectileSpecs;
    setMatter(projectileSpecs_.matter);
}


void Projectile::redrawObject() {
    SDL_FRect destRect;
    Model* model = getNextModelPtr();
    if (model != nullptr && model->getTexture() != nullptr) {
        destRect.w = model->getModelTextureWidth();
        destRect.h = model->getModelTextureHeight();
        destRect.x = model->getTextureRelativeUL().x + getCenter().x;
        destRect.y = -(model->getTextureRelativeUL().y + getCenter().y);
        SDL_RenderCopyExF(getRenderer(), model->getTexture(), NULL, &destRect, 
                          -std::atan(travelDirectionSlopeCoefficient_)*(180/M_PI), NULL, SDL_FLIP_NONE);
    }
}


void Projectile::redrawObject(bool drawHitboxes, float pointSize) {
    SDL_FRect destRect;
    Model* model = getNextModelPtr();
    if (model != nullptr && model->getTexture() != nullptr) {
        destRect.w = model->getModelTextureWidth();
        destRect.h = model->getModelTextureHeight();
        destRect.x = model->getTextureRelativeUL().x + getCenter().x;
        destRect.y = -(model->getTextureRelativeUL().y + getCenter().y);
        SDL_RenderCopyExF(getRenderer(), model->getTexture(), NULL, &destRect, 
                          -std::atan(travelDirectionSlopeCoefficient_)*(180/M_PI), NULL, SDL_FLIP_NONE);
        if (drawHitboxes) {
            std::vector<Point> hull = model->getHitboxPtr()->getCurrentHull();
            int n = hull.size();
            for (int i=0; i<n; ++i) {
                drawPoint(getRenderer(), hull[i].x, -hull[i].y, pointSize);
            }
        }
    }
}


void Projectile::redrawObject(const Rectangle& currentlyObservedRectangle) {
    Model* model = getNextModelPtr();
    if (model != nullptr && model->getTexture() != nullptr &&
        currentlyObservedRectangle.collidesWith(model->getRelativeRectangle()+getCenter())) {
        SDL_FRect destRect;
        destRect.w = model->getModelTextureWidth();
        destRect.h = model->getModelTextureHeight();
        destRect.x = model->getTextureRelativeUL().x + getCenter().x;
        destRect.y = model->getTextureRelativeUL().y + getCenter().y;
        destRect.x -= currentlyObservedRectangle.upperLeft.x;
        destRect.y -= currentlyObservedRectangle.upperLeft.y;
        destRect.y *= -1;
        SDL_RenderCopyExF(getRenderer(), model->getTexture(), NULL, &destRect, 
                          -std::atan(travelDirectionSlopeCoefficient_)*(180/M_PI), NULL, SDL_FLIP_NONE);
    }
}


void Projectile::translateObjectByVector(const Point& translationVector) {
    setCenter(getCenter()+translationVector);
}


bool Projectile::isAnythingScheduled() const {
    return isAlive();
}


void Projectile::runScheduled() {
    if (isAnythingScheduled()) {
        handleProjectileMovement();
    }
}


