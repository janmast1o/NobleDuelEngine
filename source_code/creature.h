#ifndef CREATURE_H
#define CREATURE_H

#include "mobile_object.h"

class InteractableManager;
class Item;

class Creature : public MobileObject {

private:

    static int defaultInitialItemListSize_;

    Point targetedPoint_;
    CreatureSpecificPhysicsChar creatureSpecificPhysicsChar_;

    std::vector<Item*> itemList_;
    size_t itemListIndex_;

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
    void handleDropItem();

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

    void translateObjectByVector(const Point& translationVector) override;

    // bool isLeftFacing() const;
    // bool isRightFacing() const;

    bool canHaveOtherOnTop() const override;

    void placeItemInItemList(Item& pickedUpItem);

    bool isAnythingInteractionLikeScheduled() const;
    bool isAnythingScheduledForItem() const;

    void runInteractionScheduled();
    void runScheduled() override;

    virtual ~Creature() = default;

};

#endif