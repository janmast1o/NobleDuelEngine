#ifndef FIREARM_H
#define FIREARM_H

#include "item.h"

class ProjectileFactory;

class Firearm : public Item {

private:

    ProjectileFactory& projectileFactory_;
    int usedAmmoTypeId_;
    std::optional<FirearmFireSpecs> fireSpecs_;
    std::optional<FirearmFireSpecs> alternativeFireSpecs_;

protected:

    double calculateRotationAngle();

    void handleFire();
    void handleAlternativeFire();

public:

    Firearm(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, 
            const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass,
            ProjectileFactory& projectileFactory, int usedAmmoTypeId, 
            std::optional<FirearmFireSpecs> fireSpecs, std::optional<FirearmFireSpecs> alternativeFireSpecs = std::nullopt,
            ItemDependencyState initialDependencyState = INDEPENDENT, Creature* initialOwner = nullptr);

    void use() override;
    void alternativeUse() override;  

    void redrawObject() override;
    void redrawObject(bool drawHitboxes, float pointSize) override; 

    void runScheduled() override;

};

#endif