#ifndef ITEM_H
#define ITEM_H

#include "mobile_object.h"
#include "interactable.h"

class Creature;

class Item : public MobileObject, public Interactable {

protected:

    std::pair<unsigned int, unsigned int> lastUse_;

    int staminaDrainedOnUse_;
    int staminaDrainedOnAlternativeUse_;
    unsigned int cooldown_;
    
    ItemDependencyState dependencyState_;
    Creature* owner_;

public:

    Item(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
         ItemDependencyState initialDependencyState = INDEPENDENT, Creature* initialOwner = nullptr);
    
    void temporarilySetIndependentForRedrawing();
    
    bool isParticipatingInCollisions() const override;
    void setNewState(State newState) override;
    
    void updateLastInteraction() override;
    bool getCurrentlyAvailable() const override;

    void updateLastUse();
    bool isAvailableForNextUse() const;

    int getStaminaDrainedOnUse() const;
    void setStaminaDrainedOnUse(int newStaminaDrainedOnUse);

    int getStaminaDrainedOnAlternativeUse() const;
    void setStaminaDrainedOnAlternativeUse(int newStaminaDrainedOnAlternativeUse);
    
    unsigned int getCooldown() const;
    void setCooldown(unsigned int newCooldown);
    
    Creature* getOwner() const;
    void performOnInteraction(void* interactionArgsVoidPtr) override;
    void ridOfOwner();     
    virtual void use() = 0;
    virtual void alternativeUse() = 0;

    bool collideableWith(const Object& otherObject) const;
    bool collideableWith(const Object& otherObject);

    void redrawObject() override;
    void redrawObject(bool drawHitboxes, float pointSize) override;
    void redrawObject(const Rectangle& currentlyObservedRectangle, bool smoothOut = true) override;

    void runScheduled() override = 0;

};

#endif