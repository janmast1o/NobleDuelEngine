#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "mobile_object.h"
#include <unordered_set>

class Creature;

class Projectile : public MobileObject {

private:

    Creature* shooter_;
    int travelDirAlongXAxis_;
    float travelDirectionSlopeCoefficient_;
    ProjectileSpecs projectileSpecs_;
    float distanceCoveredSoFar_;
    std::unordered_set<Object*> hitRegistry_;

    MobileHitbox leftTravellingHitbox_;
    MobileHitbox rightTravellingHitbox_;

protected:

    void handleProjectileMovement();

public:

    Projectile(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, 
               MobileHitbox leftTravellingHitbox, MobileHitbox rightTravellingHitbox, 
               const EngineClock& sessionEngineClock,
               ObjectMap& objectMap, float mass, Point targetedPoint, ProjectileSpecs projectileSpecs = {}, Creature* shooter = nullptr);

    Projectile(const Projectile& otherProjectile);           

    void setCenter(const Point& newCenter) override;

    Creature* getShooter() const;
    void setShooter(Creature* newShooter); 

    int getTravelDirAlongXAxis() const;
    void setTargetedPoint(Point newTargetedPoint);

    ProjectileSpecs getProjectileSpecs() const;
    void setProjectileSpecs(ProjectileSpecs& newProjectileSpecs);

    void redrawObject() override;
    void redrawObject(bool drawHitboxes, float pointSize) override;
    void redrawObject(const Rectangle& currentlyObservedRectangle, bool smoothOut = true) override;

    void translateObjectByVector(const Point& translationVector) override;

    bool isAnythingScheduled() const override; 
    void runScheduled() override;

};

#endif