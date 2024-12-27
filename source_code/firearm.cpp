#include "firearm.h"
#include "creature.h"

Firearm::Firearm(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, 
            const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
            ItemDependencyState initialDependencyState, Creature* initialOwner,
            ProjectileFactory& projectileFactory, int usedAmmoTypeId, 
            std::optional<FirearmFireSpecs> fireSpecs, std::optional<FirearmFireSpecs> alternativeFireSpecs) :
            Object(renderer, center, modelCollection, sessionEngineClock),
            Item(renderer, center, modelCollection, sessionEngineClock, objectMap, mass, initialDependencyState, initialOwner),
            projectileFactory_(projectileFactory), usedAmmoTypeId_(usedAmmoTypeId),
            fireSpecs_(fireSpecs), alternativeFireSpecs_(alternativeFireSpecs) {}


double Firearm::calculateRotationAngle() {
    
}


void Firearm::handleFire() {}


void Firearm::handleAlternativeFire() {}


void Firearm::use() {
    if (!fireSpecs_.has_value()) return; 
    else if (isAvailableForNextUse() && !owner_->getCreatureGameStatsAsPtr()->subtractFromStamina(staminaDrainedOnUse_, sessionEngineClock_)) {
        updateLastUse();
        setScheduled(HANDLE_USE);
    }
}


void Firearm::alternativeUse() {
    if (!alternativeFireSpecs_.has_value()) return;
    else if (isAvailableForNextUse() && !owner_->getCreatureGameStatsAsPtr()->subtractFromStamina(staminaDrainedOnAlternativeUse_, sessionEngineClock_)) {
        updateLastUse();
        setScheduled(HANDLE_ALT_USE);
    }
}


void Firearm::redrawObject() {
    SDL_FRect destRect;
    Model* model = getNextModelPtr();
    if (model != nullptr && model->getTexture() != nullptr) {
        destRect.w = model->getModelTextureWidth();
        destRect.h = model->getModelTextureHeight();
        destRect.x = model->getTextureRelativeUL().x + getCenter().x;
        destRect.y = -(model->getTextureRelativeUL().y + getCenter().y);
        SDL_RenderCopyExF(getRenderer(), model->getTexture(), NULL, &destRect, calculateRotationAngle(), NULL, SDL_FLIP_NONE);
    }
}