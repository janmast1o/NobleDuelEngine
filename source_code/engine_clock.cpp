#include "engine_clock.h"
#include "constants.h"

EngineClock::EngineClock() {}


unsigned int EngineClock::getCurrentTimeInFrames() const {
    return cycles*FPS+framesInCycle;
}


void EngineClock::incrementEngineClock() {
    framesInCycle++;
    if (framesInCycle == FPS) {
        framesInCycle = 0;
        cycles++;
    }
}