#ifndef ELEVATOR_H
#define EVELATOR_H

#include "floating_platform.h"
#include <queue>

class Elevator : public FloatingPlatform {

private:

    bool stopped_;
    std::queue<int> elevatorReqs_;
    int maxQueueSize_;
    int perFloorRemain_;

protected:

    void handleMoveAccordingToMode() override;
    void runScheduled() override;

public:

    Elevator(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, ObjectMap& objectMap, float mass, 
             const std::vector<Velocity>& velocitiesForMovementModes, const std::vector<Point>& bordersForMovementModes);

    int getMaxQueueSize() const;
    void setMaxQueueSize(int newMaxQueueSize);

    int getPerFloorRemain() const;
    void setPerFloorRemain(int newPerFloorRemain);

    bool requestNewElevatorMovement(int requestedElevatorModeIndex);

};

#endif