#ifndef MOBILE_OBJECT_H
#define MOBILE_OBJECT_H

#include "object.h"
#include "object_map.h"
#include "mobile_hitbox.h"
#include <functional>

class MobileObject : virtual public Object {

protected:

    static const float defaultMTPIgnoreCoeff;
    static const float defaultMTPBeNudgedCoeff;
    static const float defaultMTPBeVecTranslatedCoeff;

    static const float defaultCanCarryWeightOnTopCoeff;
    static const float defaultShouldIgnoreOtherSlidingOffTopCoeff;

    static const float defaultParamSxVecMomentumTransferLoss;

    Point previousFrameCenter_;
    
    float mass_;
    float slopeInclineDirectlyUnderneath_;
    MomentumDictated currentMomentumDictated_;

    float auxDistanceCoveredSoFar_;
    float auxDistanceToCover_;

    Object* objectCurrentlyUnderneath_; // potential null pointing pointer

    MomentumTransferProtocol momentumTransferProtcol_;
    float onlyParamSxMomentumTransferLoss_;

    std::function<bool(float, float)> canCarryWeightOnTopFunction_;
    std::function<bool(float, float)> shouldIgnoreOtherSlidingOffTopFunction_;

    Velocity velocity_;
    Acceleration acceleration_;
    Velocity airborneGhostHorizontalVelocity_;
    ObjectSpecificPhysicsChar objectSpecificPhysicsChar_;
    bool participatingInMomentum_;

    ObjectMap& objectMap_;

    ScheduledInstruction scheduled_;
    ScheduledInstruction previouslyScheduled_;

    SingleStatePersistence singleStatePersistenceTimer_;

    std::list<MobileObject*> spawnedObjects_;

    Point smoothOutForDisplay(Point currentlyObservedRectangleUpperLeft, Point calcDisplayUpperLeft) override;
    
    void removeGroundReactionAcceleration();
    void addGroundReactionAcceleration();

    void zeroVelocity();
    void zeroAirborneGhostHorizontalVelocity();

    void applyFriction();
    bool fixReceivedVelocityIfNeccessary(float& receivedHVelocity, float& receivedHTranslation) const;

    void prepareNextSlideOffTopScheduled();
    void prepareNextEscapeScheduled(float escapeDirection, MobileObject& escapingFrom);

    void horizontalMovementMainBody(Point& svec, const std::list<Object*>& potentiallyColliding, 
                                    float& alpha, float& beta, float& gamma, float& delta, bool& collisionDetected, bool& groundUnderneathFound, bool& changingSlopes,
                                    bool moveHorizontallyCurrentlyHandled, std::list<MobileObject*>* foundMobileDirectlyAbove,
                                    Object*& alphaTempObjectCurrentlyUnderneath, Object*& gammaTempObjectCurrentlyUnderneath);

    void freefallMainBody(Point& svec, const std::list<Object*>& potentiallyUnderneath,
                          float& alpha, float& delta, 
                          bool& groundUnderneathFound,
                          Object*& alphaTempObjectCurrentlyUnderneath);                      
    
    virtual void adjustSVec(Point& svec);   
    bool moveMobileDirectlyAbove(std::list<MobileObject*>& mobileDirectlyAbove, const Point& translationVector);                             

    void handleCheckForGroundDirectlyUnderneath();
    virtual void handleBePushedHorizontally(HandleParams handleParams = {0, true});
    void handleEscapeFromUnderneathObjectOnTop(HandleParams handleParams = {0, true});
    void handleSlideDown(HandleParams handleParams = {0, true});
    void handleSlideOffTop();
    void handleAirborne(HandleParams handleParams = {0, false});
    void handleFreefall();
    void handleStop();

    ScheduledInstruction getScheduled() const;
    ScheduledInstruction getPreviouslyScheduled() const;
    void setScheduled(ScheduledInstruction newInteractionScheduled);
    void clearScheduled();

    friend MomentumTransferProtocol;

public:

    MobileObject(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass);

    bool isMobile() const override;

    void setCenter(const Point& newCenter) override;
    void updatePreviousFrameCenter();

    float getCurrentHVelocity() const;
    float getMass() const;
    void setMass(float newMass, bool updateMomentumTransferProtocol = true); 

    float getMaxSRVerticalV() const;
    void setMaxSRVerticalV(float newMaxSRVerticalV);

    float getMaxSRHorizontalV() const;
    void setMaxSRHorizontalV(float newMaxSRHorizontalV);

    float getTrueMaxVerticalV() const;
    void setTrueMaxVerticalV(float newTrueMaxVerticalV);

    float getTrueMaxHorizontalV() const;
    void setTrueMaxHorizontalV(float newTrueMAxHorizontalV, bool updateMomentumTransferProtocol = true);

    float getHorizontalAcc() const;
    void setHorizontalAcc(float newHorizontalAcc);

    int getMaxAirborneAccelerableFrames() const;
    void setMaxAirborneAccelerableFrames(int newMaxAirborneAccelerableFrames);

    float getShouldMTPIgnoreUpperThreshold() const;
    float getShouldMTPBeNudgedThreshold() const;
    float getShouldMTPBeVecTranslatedThreshold() const;

    void setShouldMTPIgnoreUpperThreshold(float newIgnoreUpperThreshold);
    void setShouldMTPBeNudgedUpperThreshold(float newBeNudgedUpperThreshold);
    void setShouldMTPBeVecTranslatedUpperThreshold(float newVecTranslatedUpperThreshold);

    float getParamSxMomentumTransferLoss() const;
    void setOnlyParamSxMomentumTransferLoss(float newOnlyVecMomentumTransferLoss);

    void setCanCarryWeightOnTopFunction(std::function<bool(float, float)>& newCanCarryWeightOnTopFunction);
    void setShouldIgnoreOtherSlidingOffTopFunction(std::function<bool(float, float)>& newShouldIgnoreOtherSlidingOffTopFunction);

    void newHorizontalAcceleration(Direction direction);
    void newVelocity();

    virtual void translateObjectByVector(const Point& translationVector);

    bool collidesWithAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    bool isDirectlyAbove(Object& otherObject) const;
    bool isDirectlyAboveAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    bool collidesWithTopAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    float findMinDistanceAlongTheLine(Object& otherObject, const Point& translationVector) const;
    float findMinVertDistanceFromTop(Object& otherObject) const;
    float findMinVertDistanceFromTopAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    float isCollisionAfterVectorTranslationCausedByGentleSlope(Object& otherObject, const Point& translationVector) const;
    float findSlopeCoefficientDirectlyBelowAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;
    float findSlopeCoefficientDirectlyBelow(Object& otherObject) const;
    float findEscapeDisAlongXAxis(Object& otherObject, float escapeDirection) const;

    bool collidesWithHitboxAfterVectorTranslation(Object& otherObject, const Point& translationVector) const;

    bool isParticipatingInMomentum() const override;
    void registerBeingAffectedByOutsideMomentum(float otherObjectMass, float otherObjectHVelocity, float hTranslation) override; 

    void redrawObject(const Rectangle& currentlyObservedRectangle, bool smoothOut = true) override;

    virtual bool isAnythingScheduled() const;
    virtual void runScheduled();

    virtual ~MobileObject() = default;

};

#endif
