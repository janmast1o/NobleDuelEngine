#include "engine_clock.h"
#include "constants.h"

EngineClock::EngineClock() {}


void EngineClock::incrementEngineClock() {
    framesInCycle++;
    if (framesInCycle == FPS) {
        framesInCycle = 0;
        cycles++;
    }
}