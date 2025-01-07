#include "projectile_manager.h"

ProjectileManager::ProjectileManager() {}


Projectile* ProjectileManager::makeNewProjectile(Projectile& projectileArchetype) {
    simulatedProjectiles_.emplace_back(projectileArchetype);
    simulatedProjectilesHitboxes_.emplace_back();
    Projectile* newSimulatedProjectile = &simulatedProjectiles_.back();
    // newSimulatedProjectile->replaceAllHitboxPtrsWithStaticCopies(simulatedProjectilesHitboxes_.back());
    return newSimulatedProjectile;
}


void ProjectileManager::simulateProjectiles() {
    // for (auto it = simulatedProjectiles_.begin(); it != simulatedProjectiles_.end(); ) {
    //     if (it->isAlive()) {
    //         it->runScheduled();
    //         ++it;
    //     } else {
    //         simulatedProjectiles_.erase(it);
    //         std::cout << "Erased" << std::endl;
    //     }
    // }

    auto it = simulatedProjectiles_.begin();
    auto ith = simulatedProjectilesHitboxes_.begin();
    while (it != simulatedProjectiles_.end() && ith != simulatedProjectilesHitboxes_.end()) {
        if (it->isAlive()) {
            it->runScheduled();
            ++it;
            ++ith;
        } else {
            it = simulatedProjectiles_.erase(it);
            ith = simulatedProjectilesHitboxes_.erase(ith);
        }
    }
}


void ProjectileManager::redrawProjectiles() {
    for (Projectile& p : simulatedProjectiles_) {
        p.redrawObject();
    }
}


void ProjectileManager::redrawProjectiles(bool drawHitboxes, float pointSize) {
    for (Projectile& p : simulatedProjectiles_) {
        p.redrawObject(drawHitboxes, pointSize);
    }
}


void ProjectileManager::redrawProjectiles(const Rectangle& currentlyObservedRectangle) {
    for (Projectile& p : simulatedProjectiles_) {
        p.redrawObject(currentlyObservedRectangle);
    }
}