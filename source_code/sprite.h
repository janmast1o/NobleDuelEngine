#include "creature.cpp"

#ifndef SPRITE_H
#define SPRITE_H

namespace GameElement {
    class Sprite;
}

class Sprite : public Creature {

private:

    bool previously_item_usage_requested_;
    int item_usage_buffer_;
    std::unordered_map<Uint8, int> previous_keyboard_state_;
    std::unordered_map<Uint8, int> buffered_inputs_;

protected:

    void buffer_inputs(const Uint8* new_buffer, const bool new_item_usage_buffer);
    void decrement_buffer_timers();
    bool is_requested(const Uint8 potentailly_requested, const Uint8* keyboard_state);
    bool is_requested_on_release(const Uint8 potentially_requested, const Uint8* keyboard_state);
    bool is_item_usage_requested_on_release(const bool item_usage_requested);

public:

    Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, ItemManager& item_manager);
    Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, ItemManager& item_manager, std::vector<Uint8> bufferables);
    Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v, ItemManager& item_manager);
    Sprite(SDL_Renderer* renderer, Point& center, ModelCollection& model_collection, Matter matter, ObjectMap& object_map, int health, float jumping_v, ItemManager& item_manager, std::vector<Uint8> bufferables);
    void add_bufferable(const Uint8 new_bufferable);
    void remove_bufferable(const Uint8 bufferable);
    void read_inputs();

};

#endif