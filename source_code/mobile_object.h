#ifndef MOBILE_OBJECT_H
#define MOBILE_OBJECT_H

#include "object.h"

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

    virtual void handleMoveHorizontally();
    void handleSlideDown();
    void handleAirborne();
    void handleFreefall();
    void handleStop();

    void removeGroundReactionAcceleration();
    void addGroundReactionAcceleration();

    ScheduledInstruction getSCheduled() const;
    ScheduledInstruction getPreviouslyScheduled() const;
    void setScheduled(ScheduledInstruction newScheduled);
    void clearScheduled();

    void zeroVelocity();
    void zeroAirborneGhostHorizontalVelocity();

    bool collidesWithAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    bool isDirectlyAboveAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    bool collidesWithTop(Object& otherObject, const Point& translationVector) const;
    float isCollisionAfterVectorTranslationCausedByGentleSlope(Object& otherObject, const Point& translationVector) const;
    float findSlopeCoefficientDirectlyBelow(Object& otherObject, const Point& translationVector) const;
    virtual void translateObjectByVector(const Point& translationVector);

public:

    MobileObject(SDL_Renderer* renderer, Point center, ModelCollection modelCollection);
    
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

};

#endif
