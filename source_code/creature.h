#ifndef CREATURE_H
#define CREATURE_H

#include "mobile_object.h"

class InteractableManager;

class Creature : public MobileObject {

private:

    Point targetedPoint_;
    CreatureSpecificPhysicsChar creatureSpecificPhysicsChar_;
    // ItemManager& itemManager_;

protected:

    ScheduledInstruction interactionScheduled_;
    ScheduledInstruction previousInteractionScheduled_;

    InteractableManager& interactableManager_;
    
    // std::vector<Item*> itemLoadout_; // implement when implementing items
    // int itemLoadoutIndex_;
    // int chargeUpFramesAccumulated_;

    void adjustAccAndVForRegular();
    void adjustAccAndVForSprint();
    void adjustAccAndVForSlowWalk();
    
    void handleBePushedHorizontally(HandleParams handleParams = {0, true}) override;
    void handleMoveHorizontally() override;
    void handleJump();

    void handleInteract();
    // void handlePickUpItem();
    // void handleDropItem();
    // void handleSwitchToNextItem();

    ScheduledInstruction getInteractionScheduled() const;
    ScheduledInstruction getPreviousInteractionScheduled() const;
    void setInteractionScheduled(ScheduledInstruction newScheduled);
    void clearInteractionScheduled();

public:

    Creature(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
             int health, InteractableManager& interactableManager);

    void updateTargetedPoint(const Point& newTargtedPoint);

    float getJumpingV() const;

    float getRegularHorizontalAcc() const;
    float getRegularMaxHorizontalV() const;

    float getSprintHorizontalAcc() const;
    float getSprintMaxHorizontalV() const;

    float getSlowWalkHorizontalAcc() const;
    float getSlowWalkMaxHorizontalV() const;

    void setJumpingV(float newJumpingV);

    void setRegularHorizontalAcc(float newRegularHorizontalAcc);
    void setRegularMaxHorizontalV(float newRegularMaxHorizontalV);
    
    void setSprintHorizontalAcc(float newSprintHorizontalAcc);
    void setSprintMaxHorizontalV(float newSprintMaxHorizontalV);

    void setSlowWalkHorizontalAcc(float newSlowWalkHorizontalAcc);
    void setSlowWalkMaxHorizontalV(float newSlowWalkMaxHorizontalV);

    bool canHaveOtherOnTop() const override;

    bool isAnythingInteractionLikeScheduled() const;

    void runInteractionScheduled();
    void runScheduled() override;

    virtual ~Creature() = default;

};

#endif