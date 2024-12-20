#ifndef ITEM_H
#define ITEM_H

#include "mobile_object.h"
#include "interactable.h"

class Creature;

class Item : public MobileObject, public Interactable {

protected:

    std::pair<unsigned int, unsigned int> lastUse_;

    unsigned int cooldown_;
    
    ItemDependencyState dependencyState_;
    Creature* owner_;

public:

    Item(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
         ItemDependencyState initialDependencyState = INDEPENDENT, Creature* initialOwner = nullptr);
    
    bool isParticipatingInCollisions() const override;
    void setNewState(State newState) override;
    
    void updateLastInteraction() override;
    bool getCurrentlyAvailable() const override;

    void updateLastUse();
    bool isAvailableForNextUse() const;

    unsigned int getCooldown() const;
    void setCooldown(unsigned int newCooldown);
    
    Creature* getOwner() const;
    void performOnInteraction(void* interactionArgsVoidPtr) override;
    void ridOfOwner();     
    virtual void use() = 0;
    virtual void alternativeUse() = 0;

    bool collideableWith(const Object& otherObject) const;
    bool collideableWith(const Object& otherObject);

    virtual void redrawObject() override;
    virtual void redrawObject(bool drawHitboxes, float pointSize) override;

    void runScheduled() override = 0;

};

#endif