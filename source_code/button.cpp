#include "button.h"

Button::Button(SDL_Renderer* renderer, Point center, ModelCollection modelCollection, 
               const EngineClock& sessionEngineClock, std::function<void()>& buttonCommand) :
               Interactable(renderer, center, modelCollection, sessionEngineClock), buttonCommand_(buttonCommand) {}


void Button::performOnInteraction(void* interactionArgsVoidPtr) {
    std::cout << "!" << std::endl;
    buttonCommand_();
}    