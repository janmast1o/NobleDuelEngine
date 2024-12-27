#ifndef ENGINE_CLOCK_H
#define ENGINE_CLOCK_H

struct EngineClock {

    unsigned int cycles = 0;
    unsigned int framesInCurrentCycle = 0;

    EngineClock();

    unsigned int getCurrentTimeInFrames() const;
    void incrementEngineClock();

};

#endif