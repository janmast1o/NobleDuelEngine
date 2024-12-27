#include "engine_clock.h"
#include "constants.h"

EngineClock::EngineClock() {}


unsigned int EngineClock::getCurrentTimeInFrames() const {
    return cycles*FPS+framesInCurrentCycle;
}


void EngineClock::incrementEngineClock() {
    framesInCurrentCycle++;
    if (framesInCurrentCycle == FPS) {
        framesInCurrentCycle = 0;
        cycles++;
    }
}