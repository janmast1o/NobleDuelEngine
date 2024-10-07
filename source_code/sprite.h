#include "creature.cpp"

#ifndef SPRITE_H
#define SPRITE_H

namespace GameElement {
    class Sprite;
}

class Sprite : public Creature {

private:

    std::unordered_map<Uint8, int> buffered_inputs_;

protected:

    void buffer_inputs(const Uint8* new_buffer);
    void decrement_buffer_timers();
    bool is_requested(const Uint8 potentailly_requested, const Uint8* keyboard_state);

public:

    Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health);
    Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, std::vector<Uint8> bufferables);
    Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v);
    Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v, std::vector<Uint8> bufferables);
    void add_bufferable(const Uint8 new_bufferable);
    void remove_bufferable(const Uint8 bufferable);
    void read_inputs();

};

#endif