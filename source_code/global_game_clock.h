#include "compilation_time_constants.h"

#ifndef GLOBAL_GAME_CLOCK
#define GLOBAL_GAME_CLOCK

struct GlobalGameClock {
    int cycles = 0;
    int frames_in_cycle = 0;

    void increment() {
        frames_in_cycle++;
        if (frames_in_cycle == FPS) {
            frames_in_cycle = 0;
            cycles++;
        }
    }

};

GlobalGameClock current_session_global_game_clock;

#endif