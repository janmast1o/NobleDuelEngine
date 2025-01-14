#ifndef PROJECTILE_MANAGER_H
#define PROJECTILE_MANAGER_H

#include "projectile.h"

class ProjectileManager {

private:

    std::list<Projectile> simulatedProjectiles_;
    // std::list<std::list<Hitbox>> simulatedProjectilesHitboxes_;

public:

    ProjectileManager();
    Projectile* makeNewProjectile(Projectile& projectileArchetype);
    void simulateProjectiles();
    void redrawProjectiles();
    void redrawProjectiles(bool drawHitboxes, float pointSize);
    void redrawProjectiles(const Rectangle& currentlyObservedRectangle);

};

#endif