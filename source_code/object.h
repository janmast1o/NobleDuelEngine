#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include "structs_and_enums.h"
#include "model_collection.h"
#include "static_hitbox.h"

class Object {

private:

    SDL_Renderer* renderer_;

    int health_;
    Point center_;
    ModelCollection modelCollection_;
    Matter matter_;

    State state_;
    State previousState_;
    DamageReceiveState damageReceiveState_;

    void registerOwnerCenterPtrForHitboxes();

protected:

    Model* getNextModelPtr();

    Hitbox& getCurrentCollisionMesh() const;

    friend class MobileObject;

public:

    Object(SDL_Renderer* renderer, Point center, ModelCollection modelCollection);

    SDL_Renderer* getRenderer() const;
    
    int getHealth() const;
    void setHealth(int newHealth);
    void subtractFromHealth(int subtractAmount);
    bool isAlive() const;

    Point& getCenter();
    void setCenter(const Point& newCenter);

    Matter getMatter() const;
    void setMatter(Matter matter);

    State getState() const;
    State getPreviousState() const;
    void setNewState(State newState);

    std::list<int> getAllTiedHitboxes() const;

    void redrawObject();
    void redrawObject(bool drawHitboxes, float pointSize);

    bool collideableWith(const Object& otherObject);

    virtual ~Object() = default;

};

#endif