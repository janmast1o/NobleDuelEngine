#ifndef BUTTON_H
#define BUTTON_H

#include "interactable.h"
#include <functional>

class Button : public Interactable {

private:

    std::function<void()> buttonCommand_;

public:

    Button(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, const EngineClock& sessionEngineClock, std::function<void()>& buttonCommand);
    void performOnInteraction(void* interactionArgsVoidPtr) override;

};

#endif