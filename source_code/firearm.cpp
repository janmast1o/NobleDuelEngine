#include "firearm.h"
#include "creature.h"
#include "utility_functions.h"
#include "projectile_factory.h"
#include <cmath>

Firearm::Firearm(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, 
            const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
            ProjectileFactory& projectileFactory, int usedAmmoTypeId, 
            std::optional<FirearmFireSpecs> fireSpecs, std::optional<FirearmFireSpecs> alternativeFireSpecs,
            ItemDependencyState initialDependencyState, Creature* initialOwner) :
            Object(renderer, center, modelCollection, sessionEngineClock),
            Item(renderer, center, modelCollection, sessionEngineClock, objectMap, mass, initialDependencyState, initialOwner),
            projectileFactory_(projectileFactory), usedAmmoTypeId_(usedAmmoTypeId),
            fireSpecs_(fireSpecs), alternativeFireSpecs_(alternativeFireSpecs) {}


double Firearm::calculateRotationAngle() {
    int ownerFacedSide = owner_->getFacedSideAsInt();
    if (ownerFacedSide*(owner_->getTargetedPoint().x - getCenter().x) <= 0) return 0;
    return -std::atan(findSlopeCoefficient(getCenter(), owner_->getTargetedPoint()))*(180/M_PI);    
}


void Firearm::handleFire() {
    FirearmFireSpecs* specs = &fireSpecs_.value();
    // std::cout << specs->numberOfBullets << std::endl;
    for (int i=0; i<specs->numberOfBullets; ++i) {
        ProjectileSpecs projectileSpecs;
        projectileSpecs.damage = specs->perBulletDamage;
        
        std::visit([&projectileSpecs, &i](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, float>) {
                projectileSpecs.travelDistance = arg;
            } else if constexpr (std::is_same_v<T, std::vector<float>>) {
                // std::cout << "A " << arg.size() << std::endl;  
                projectileSpecs.travelDistance = arg[i%arg.size()];
            }
         
        }, specs->bulletTravelDistance);

        std::visit([&projectileSpecs, &i](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, bool>) {
                projectileSpecs.pierce = arg;
            } else if constexpr (std::is_same_v<T, std::vector<bool>>) {
                // std::cout << "B " << arg.size() << std::endl;  
                projectileSpecs.pierce = arg[i%arg.size()];
            }
        }, specs->bulletPierce);

        std::visit([&projectileSpecs, &i](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, float>) {
                projectileSpecs.travelSpeed = arg;
            } else if constexpr (std::is_same_v<T, std::vector<float>>) {
                // std::cout << "C " << arg.size() << std::endl;  
                projectileSpecs.travelSpeed = arg[i%arg.size()];
            }
        }, specs->bulletSpeed);

        std::visit([&projectileSpecs, &i](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Matter>) {
                projectileSpecs.matter = arg;
            } else if constexpr (std::is_same_v<T, std::vector<Matter>>) {
                // std::cout << "D " << arg.size() << std::endl;  
                projectileSpecs.matter = arg[i%arg.size()];
            }
        }, specs->bulletMatter);
        
        if (specs->bulletSpreadFromCenter.size() == 0) {
            projectileFactory_.createNewInstanceOfALiveProjectile(usedAmmoTypeId_, projectileSpecs, 
                                                                  owner_, owner_->getTargetedPoint(), getCenter());                                                 
        } else {
            Point rotatedVector = rotateVector(owner_->getTargetedPoint()-getCenter(), 
                                               specs->bulletSpreadFromCenter[i%specs->bulletSpreadFromCenter.size()]);
            projectileFactory_.createNewInstanceOfALiveProjectile(usedAmmoTypeId_, projectileSpecs, 
                                                                  owner_, rotatedVector+getCenter(), getCenter());                                                  
        }

    }

    clearScheduled();
}


void Firearm::handleAlternativeFire() {
    FirearmFireSpecs* specs = &alternativeFireSpecs_.value();

    for (int i=0; i<specs->numberOfBullets; ++i) {
        ProjectileSpecs projectileSpecs;
        projectileSpecs.damage = specs->perBulletDamage;
        
        std::visit([&projectileSpecs, &i](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, float>) {
                projectileSpecs.travelDistance = arg;
            } else if constexpr (std::is_same_v<T, std::vector<float>>) {
                projectileSpecs.travelDistance = arg[i%arg.size()];
            }
         
        }, specs->bulletTravelDistance);

        std::visit([&projectileSpecs, &i](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, bool>) {
                projectileSpecs.pierce = arg;
            } else if constexpr (std::is_same_v<T, std::vector<bool>>) {
                projectileSpecs.pierce = arg[i%arg.size()];
            }
        }, specs->bulletPierce);

        std::visit([&projectileSpecs, &i](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, float>) {
                projectileSpecs.travelSpeed = arg;
            } else if constexpr (std::is_same_v<T, std::vector<float>>) {
                projectileSpecs.travelSpeed = arg[i%arg.size()];
            }
        }, specs->bulletSpeed);

        std::visit([&projectileSpecs, &i](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Matter>) {
                projectileSpecs.matter = arg;
            } else if constexpr (std::is_same_v<T, std::vector<Matter>>) {
                projectileSpecs.matter = arg[i%arg.size()];
            }
        }, specs->bulletMatter);
        
        if (specs->bulletSpreadFromCenter.size() == 0) {
            projectileFactory_.createNewInstanceOfALiveProjectile(usedAmmoTypeId_, projectileSpecs, 
                                                                  owner_, owner_->getTargetedPoint(), getCenter());
        } else {
            Point rotatedVector = rotateVector(owner_->getTargetedPoint()-getCenter(), 
                                               specs->bulletSpreadFromCenter[i%specs->bulletSpreadFromCenter.size()]);
            projectileFactory_.createNewInstanceOfALiveProjectile(usedAmmoTypeId_, projectileSpecs, 
                                                                  owner_, rotatedVector+getCenter(), getCenter()); 
        }

    }

    clearScheduled();
}


void Firearm::use() {
    if (!fireSpecs_.has_value()) return; 
    else if (isAvailableForNextUse() && !owner_->getCreatureGameStatsAsPtr()->subtractFromStamina(staminaDrainedOnUse_, sessionEngineClock_) &&
             owner_->getFacedSideAsInt()*(owner_->getTargetedPoint().x - owner_->getCenter().x) > 0) {
        // std::cout << "A" << std::endl;
        updateLastUse();
        setScheduled(HANDLE_USE);
    }
}


void Firearm::alternativeUse() {
    if (!alternativeFireSpecs_.has_value()) return;
    else if (isAvailableForNextUse() && !owner_->getCreatureGameStatsAsPtr()->subtractFromStamina(staminaDrainedOnAlternativeUse_, sessionEngineClock_) &&
             owner_->getFacedSideAsInt()*(owner_->getTargetedPoint().x - owner_->getCenter().x) > 0) {
        updateLastUse();
        setScheduled(HANDLE_ALT_USE);
    }
}


void Firearm::redrawObject() {
    if (dependencyState_ == INDEPENDENT) {
        Object::redrawObject();
        return;
    } else if (dependencyState_ == TEMP_INDEPENDENT) {
        SDL_FRect destRect;
        Model* model = getNextModelPtr();
        if (model != nullptr && model->getTexture() != nullptr) {
            destRect.w = model->getModelTextureWidth();
            destRect.h = model->getModelTextureHeight();
            destRect.x = model->getTextureRelativeUL().x + getCenter().x;
            destRect.y = -(model->getTextureRelativeUL().y + getCenter().y);
            // SDL_FPoint rotationPoint = {-model->getTextureRelativeUL().x, model->getTextureRelativeUL().y-destRect.h};
            // SDL_RenderCopyExF(getRenderer(), model->getTexture(), NULL, &destRect, calculateRotationAngle(), &rotationPoint, SDL_FLIP_NONE);
            SDL_RenderCopyExF(getRenderer(), model->getTexture(), NULL, &destRect, calculateRotationAngle(), NULL, SDL_FLIP_NONE);
        }
        dependencyState_ = DEPENDENT;
    }
}


void Firearm::redrawObject(bool drawHitboxes, float pointSize) {
    if (dependencyState_ == INDEPENDENT) {
        Object::redrawObject(drawHitboxes, pointSize);
        return;
    } else if (dependencyState_ == TEMP_INDEPENDENT) {
        SDL_FRect destRect;
        Model* model = getNextModelPtr();
        if (model != nullptr && model->getTexture() != nullptr) {
            destRect.w = model->getModelTextureWidth();
            destRect.h = model->getModelTextureHeight();
            destRect.x = model->getTextureRelativeUL().x + getCenter().x;
            destRect.y = -(model->getTextureRelativeUL().y + getCenter().y);
            // SDL_FPoint rotationPoint = {-model->getTextureRelativeUL().x, model->getTextureRelativeUL().y-destRect.h};
            // SDL_RenderCopyExF(getRenderer(), model->getTexture(), NULL, &destRect, calculateRotationAngle(), &rotationPoint, SDL_FLIP_NONE);
            SDL_RenderCopyExF(getRenderer(), model->getTexture(), NULL, &destRect, calculateRotationAngle(), NULL, SDL_FLIP_NONE);
            if (drawHitboxes) {
                std::vector<Point> hull = model->getHitboxPtr()->getCurrentHull();
                int n = hull.size();
                for (int i=0; i<n; ++i) {
                    drawPoint(getRenderer(), hull[i].x, -hull[i].y, pointSize);
                }
            }
        }
        dependencyState_ = DEPENDENT;
    }
}


void Firearm::runScheduled() {
    if (dependencyState_ == INDEPENDENT) {
        MobileObject::runScheduled();
    } else if (isAnythingScheduled()) {
        switch (scheduled_) {
            case HANDLE_USE:
                handleFire();
                break;
            case HANDLE_ALT_USE:
                handleAlternativeFire();
                break;    
            default:
                clearScheduled();
                break;    
        }
    }
}