#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"

class Consumable : public Item {

private:

    std::function<void(void*)> onConsumptionFunction_;

    float raiseRange_;

    unsigned short approachEffectFrames_;
    unsigned short recoveryFrames_;

    unsigned short currentFrameCounter_;

    void handleConsume();
    void handleRecover();

public:

    Consumable(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
               std::function<void(void*)> onConsumptionFunction, float raiseRange,
               unsigned short approachEffectFrames, unsigned short recoveryFrames,
               ItemDependencyState initialDependencyState = INDEPENDENT, Creature* initialOwner = nullptr);

    void use() override;           
    void alternativeUse() override;

    void runScheduled() override;

};

#endif