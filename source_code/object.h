#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include "structs_and_enums.h"
#include "model_collection.h"
#include "static_hitbox.h"
#include "engine_clock.h"
#include "grid_organizer.h"

class Object {

private:

    static const EngineClock dummyEngineClock_;

    SDL_Renderer* renderer_;

    int health_;
    int maxHealth_;
    Point center_;
    ModelCollection modelCollection_;
    Matter matter_;

    State state_;
    State previousState_;
    DamageReceiveState damageReceiveState_;

    // Point previousCurrentlyObservedRectangleUpperLeft_;
    // Point previousTextureUpperLeftDisplayPoint_;

    void registerOwnerCenterPtrForHitboxes();

protected:

    const EngineClock& sessionEngineClock_;
    
    Model* getNextModelPtr();

    // Hitbox& getCurrentCollisionMesh() const;

    friend class MobileObject;

public:

    Object();

    Object(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock);

    virtual bool isParticipatingInCollisions() const;
    virtual bool isMobile() const;

    SDL_Renderer* getRenderer() const;

    Rectangle getLargestRectangle() const;
    Rectangle getLargestRectangle();
    Hitbox& getCurrentCollisionMesh() const;
    Hitbox& getCurrentHitbox() const;
    Point getCurrentItemGripPoint() const;
 
    int getHealth() const;
    void setHealth(int newHealth);
    int getMaxHealth() const;
    void setMaxHealth(int newMaxHealth);
    void subtractFromHealth(int subtractAmount);
    void addToHealth(int addAmount);
    bool isAlive() const;

    Point& getCenter();
    virtual void setCenter(const Point& newCenter);

    Matter getMatter() const;
    void setMatter(Matter matter);

    State getState() const;
    State getPreviousState() const;
    virtual void setNewState(State newState);

    std::list<int> getAllTiedHitboxes() const;

    virtual void redrawObject();
    virtual void redrawObject(bool drawHitboxes, float pointSize);
    virtual void redrawObject(const Rectangle& currentlyObservedRectangle);

    virtual bool collideableWith(const Object& otherObject) const;
    virtual bool collideableWith(const Object& otherObject);

    virtual bool canHaveOtherOnTop() const;

    virtual bool isParticipatingInMomentum() const;
    virtual void registerBeingAffectedByOutsideMomentum(float otherObjectMass, float otherObjectHVelocity, float hTranslation); 

    void replaceAllHitboxPtrsWithStaticCopies(std::list<Hitbox>& saveContainer);

    virtual ~Object() = default;

};

#endif