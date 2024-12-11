#ifndef ENGINE_CLOCK_H
#define ENGINE_CLOCK_H

struct EngineClock {

    unsigned int cycles = 0;
    unsigned int framesInCycle = 0;

    EngineClock();

    void incrementEngineClock();

};

#endif