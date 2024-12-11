#ifndef FLOATING_PLATFORM_H
#define FLOATING_PLATFORM_H

#include "mobile_object.h"

class FloatingPlatform : public MobileObject {

protected:

    std::vector<Velocity> velocitiesForMovementModes_;
    std::vector<Point> bordersForMovementModes_;
    int movementModeIndex_;

    Velocity currentVelocity_;

    float startToSlowDownDistance_;
    float slowDownPerSecondRate_;

    std::vector<Velocity> minVeloctiesForMovementModes_;

    bool willBorderBeBreached(Point& svec);

    void adjustVelocity();
    void adjustSVec(Point& svec) override;
    
    virtual void handleMoveAccordingToMode();
    void runScheduled() override;                 

public:

    FloatingPlatform(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
                     const std::vector<Velocity>& velocitiesForMovementModes, const std::vector<Point>& bordersForMovementModes);

    std::vector<Velocity> getVelocitiesForMovementModes() const;
    void setVelocitiesForMovementModes(std::vector<Velocity>& newVelocitiesForMovementModes);

    std::vector<Point> getBordersForMovementModes() const;
    void setBordersForMovementModes(std::vector<Point>& newBordersForMovementModes);

    std::vector<Velocity> getMinVelocitiesForMovementModes() const;
    void setMinVelocitiesForMovementModes(std::vector<Velocity>& newMinVelocitiesForMovementModes);

    float getStartToSlowDownDistance() const;
    void setStartToSlowDownDistance(float newStartToSlowDownDistance);

    float getSlowDownPerSecondRate() const;
    void setSlowDownPerSecondRate(float newSlowDownRate);

    bool isParticipatingInMomentum() const override;

};

#endif