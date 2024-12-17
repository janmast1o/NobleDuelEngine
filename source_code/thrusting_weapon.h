#ifndef THRUSTING_WEAPON_H
#define THRUSTING_WEAPON_H

#include "item.h"
#include <unordered_set>

class ThrustingWeapon : public Item {

private:

    int damage_;
    int poiseDamage_;

    bool hasWindUp_;

    unsigned short windUpFrames_;
    unsigned short attackFrames_;
    unsigned short recoveryFrames_;

    float windUpExtendRange_;
    float attackExtendRange_;

    unsigned short currentFrameCounter_;
    std::unordered_set<Object*> hitRegistry_;

    int sideFacedDuringAttackStartUpAsInt_;

protected:

    void handleAttack();
    void handleRecoverToNeutral();
    void handleWindUp();

public:

    ThrustingWeapon(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, 
                    const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass,
                    int damage, int poiseDamage, unsigned short attackFrames, 
                    unsigned short recoveryFrames, float attackExtendRange,
                    ItemDependencyState initialDependencyState = INDEPENDENT, Creature* initialOwner = nullptr);
    
    int getDamage() const;
    void setDamage(int newDamage);

    int getPoiseDamage() const;
    void setPoiseDamage(int newPoiseDamage);

    bool getHasWindUp() const;
    void setHasWindUp(bool newHasWindUp);

    unsigned short getWindUpFrames() const;
    void setWindUpFrames(unsigned short newWindUpFrames);

    unsigned short getAttackFrames() const;
    void setAttackFrames(unsigned short newAttackFrames);

    unsigned short getRecoveryFrames() const;
    void setRecoveryFrames(unsigned short newRecoveryFrames);

    float getWindUpExtendRange() const;
    void setWindUpExtendRange(float newWindUpExtendRange);

    float getAttackExtendRange() const;
    void setAttackExtendRange(float newAttackExtendRange);
    
    void use() override;
    void alternativeUse() override;
    void runScheduled() override;

};

#endif