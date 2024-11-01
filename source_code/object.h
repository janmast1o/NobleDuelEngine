#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include "structs_and_enums.cpp"
#include "model_collection.h"

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

protected:

    Model* getNextModelPtr();

public:

    Object(SDL_Renderer* renderer, Point center, ModelCollection modelCollection);

    SDL_Renderer* getRenderer() const;
    
    int getHealth() const;
    void setHealth(int newHealth);
    void subtractFromHealth(int subtractAmount);

    Point& getCenter();
    void setCenter(const Point& newCenter);

    Matter getMatter() const;
    void setMatter(Matter matter);

    State getState() const;
    State getPreviousState() const;
    void setNewState(State newState);

    void redrawObject();
    void redrawObject(bool drawHitboxes, float pointSize);

};

#endif