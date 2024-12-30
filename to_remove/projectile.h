#ifndef PROJECTILE_H
#define PROJECTILE_H

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

protected:

    void handleProjectileMovement();

public:

    Projectile(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock,
               ObjectMap& objectMap, float mass, Point targetedPoint, ProjectileSpecs projectileSpecs = {}, Creature* shooter = nullptr);

    Creature* getShooter() const;
    void setShooter(Creature* newShooter); 

    int getTravelDirAlongXAxis() const;
    void setTargetedPoint(Point newTargetedPoint);

    ProjectileSpecs getProjectileSpecs() const;
    void setProjectileSpecs(ProjectileSpecs& newProjectileSpecs);

    void redrawObject() override;
    void redrawObject(bool drawHitboxes, float pointSize) override;

    bool isAnythingScheduled() const override; 
    void runScheduled() override;

};

#endif