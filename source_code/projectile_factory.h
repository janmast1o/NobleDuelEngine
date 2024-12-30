#ifndef PROJECTILE_FACTORY_H
#define PROJECTILE_FACTORY_H

#include <vector>
#include "structs_and_enums.h"

class Projectile;
class ProjectileManager;
class Creature;

class ProjectileFactory {

private:

    std::vector<Projectile> serializedProjectileArchetypes_;
    ProjectileManager& projectileManager_;

public:

    ProjectileFactory(ProjectileManager& projectileManager);
    // returns the index of the added archetype
    int registerNewProjectileArchetype(Projectile& newProjectileArchetype);
    // creates new instance of a projectile of a specfic archetype with specific specs and entrusts projectile manager with simulating it
    void createNewInstanceOfALiveProjectile(int archetypeId, ProjectileSpecs newProjectileSpecs, 
                                            Creature* shooter, Point targetedPoint, Point startingPoint);

};

#endif