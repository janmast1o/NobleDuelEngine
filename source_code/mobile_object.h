#ifndef MOBILE_OBJECT_H
#define MOBILE_OBJECT_H

#include "object.h"
#include "object_map.h"
#include "mobile_hitbox.h"

class MobileObject : public Object {

protected:

    float slopeInclineDirectlyUnderneath_;
    Velocity velocity_;
    Acceleration acceleration_;
    Velocity airborneGhostHorizontalVelocity_;
    ObjectSpecificPhysicsChar objectSpecificPhysicsChar_;

    ObjectMap& objectMap_;

    ScheduledInstruction scheduled_;
    ScheduledInstruction previouslyScheduled_;

    SingleStatePersistence singleStatePersistenceTimer_;

    std::list<MobileObject*> spawnedObjects_;

    void removeGroundReactionAcceleration();
    void addGroundReactionAcceleration();

    void zeroVelocity();
    void zeroAirborneGhostHorizontalVelocity();

    virtual void handleMoveHorizontally();
    void handleSlideDown();
    void handleAirborne();
    void handleFreefall();
    void handleStop();

    bool collidesWithAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    bool isDirectlyAboveAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    bool collidesWithTopAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    float isCollisionAfterVectorTranslationCausedByGentleSlope(Object& otherObject, const Point& translationVector) const;
    float findSlopeCoefficientDirectlyBelowAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;

    virtual void translateObjectByVector(const Point& translationVector);

    ScheduledInstruction getScheduled() const;
    ScheduledInstruction getPreviouslyScheduled() const;
    void setScheduled(ScheduledInstruction newScheduled);
    void clearScheduled();

public:

    MobileObject(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, ObjectMap& objectMap);
    
    float getMaxVerticalV() const;
    void setMaxVerticalV(float newMaxVerticalV);

    float getMaxHorizontalV() const;
    void setMaxHorizontalV(float newMaxHorizontalV);

    float getHorizontalAcc() const;
    void setHorizontalAcc(float newHorizontalAcc);

    int getMaxAirborneAccelerableFrames() const;
    void setMaxAirborneAccelerableFrames(int newMaxAirborneAccelerableFrames);

    void newHorizontalAcceleration(Direction direction);
    void newVelocity();

    int getFacedSideAsInt() const;

    bool isAnythingScheduled() const;
    virtual void runScheduled();

    virtual ~MobileObject() = default;

};

#endif
