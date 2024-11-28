#ifndef MOBILE_OBJECT_H
#define MOBILE_OBJECT_H

#include "object.h"
#include "object_map.h"
#include "mobile_hitbox.h"
#include <functional>

class MobileObject : public Object {

protected:

    float mass_;
    float slopeInclineDirectlyUnderneath_;
    MomentumDictated currentMomentumDictated_;

    std::function<bool(float, float)> shouldIgnoreOutsideMomentumFunction_;
    std::function<bool(float, float)> shouldOnlyBeMovedSlightlyByOutsideMomentmFunction_;

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

    void applyFriction();

    // void updateAwaitingMomDictScheduledVelocity(float receivedVelocity);
    // void updateAwaitingMomDictScheduledExplicitHTransaltion(float receivedExplicitHTranslation);

    void horizontalMovementMainBody(Point& svec, const std::list<Object*>& potentiallyColliding, 
                                    float& alpha, float& beta, float& gamma, bool& collisionDetected, bool& groundUnderneathFound, bool& changingSlopes);

    void adjustSVecForMaxVReqs(Point& svec) const;                                

    virtual void handleBePushedHorizontally(HandleParams handleParams = {0, true});
    virtual void handleMoveHorizontally();
    void handleSlideDown(HandleParams handleParams = {0, true});
    void handleAirborne(HandleParams handleParams = {0, false});
    void handleFreefall();
    void handleStop();

    bool collidesWithAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    bool isDirectlyAboveAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    bool collidesWithTopAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    float findMinVertDistanceFromTop(Object& otherObject) const;
    float findMinVertDistanceFromTopAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    float isCollisionAfterVectorTranslationCausedByGentleSlope(Object& otherObject, const Point& translationVector) const;
    float findSlopeCoefficientDirectlyBelowAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;

    virtual void translateObjectByVector(const Point& translationVector);

    ScheduledInstruction getScheduled() const;
    ScheduledInstruction getPreviouslyScheduled() const;
    void setScheduled(ScheduledInstruction newScheduled);
    void clearScheduled();

public:

    MobileObject(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, ObjectMap& objectMap, float mass);

    float getCurrentHVelocity() const;
    float getMass() const;
    
    float getMaxSRVerticalV() const;
    void setMaxSRVerticalV(float newMaxSRVerticalV);

    float getMaxSRHorizontalV() const;
    void setMaxSRHorizontalV(float newMaxSRHorizontalV);

    float getTrueMaxVerticalV() const;
    void setTrueMaxVerticalV(float newTrueMaxVerticalV);

    float getTrueMaxHorizontalV() const;
    void setTrueMaxHorizontalV(float newTrueMAxHorizontalV);

    float getHorizontalAcc() const;
    void setHorizontalAcc(float newHorizontalAcc);

    int getMaxAirborneAccelerableFrames() const;
    void setMaxAirborneAccelerableFrames(int newMaxAirborneAccelerableFrames);

    void newHorizontalAcceleration(Direction direction);
    void newVelocity();

    int getFacedSideAsInt() const;

    bool participatingInMomentum() const override;
    void registerBeingAffectedByOutsideMomentum(float otherObjectMass, float otherObjectHVelocity, float hTranslation) override; 

    bool isAnythingScheduled() const;
    void runScheduledForNonEmptyMomentum();
    virtual void runScheduled();

    virtual ~MobileObject() = default;

};

#endif
