#include "projectile_factory.h"
#include "projectile.h"
#include "projectile_manager.h"
#include "creature.h"

ProjectileFactory::ProjectileFactory(ProjectileManager& projectileManager) : projectileManager_(projectileManager) {}


int ProjectileFactory::registerNewProjectileArchetype(Projectile& newProjectileArchetype) {
    serializedProjectileArchetypes_.emplace_back(newProjectileArchetype);
    return serializedProjectileArchetypes_.size()-1;
}


void ProjectileFactory::createNewInstanceOfALiveProjectile(int archetypeId, ProjectileSpecs newProjectileSpecs, 
                                                           Creature* shooter, Point targetedPoint, Point startingPoint) {
    if (archetypeId >= serializedProjectileArchetypes_.size()) return;
    Projectile* newlyCreatedProjectile = projectileManager_.makeNewProjectile(serializedProjectileArchetypes_[archetypeId]);
    // std::cout << startingPoint << std::endl;
    newlyCreatedProjectile->setCenter(startingPoint);
    newlyCreatedProjectile->setProjectileSpecs(newProjectileSpecs);
    newlyCreatedProjectile->setShooter(shooter);
    newlyCreatedProjectile->setTargetedPoint(targetedPoint);
    // std::cout << *newlyCreatedProjectile->getCurrentCollisionMesh().getOwnerCenterPtr() << std::endl;
}