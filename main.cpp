#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <thread>
#include <chrono>
#include "source_code/sprite.cpp"
#include "source_code/thrusting_weapon.cpp"

void mainLoop(SDL_Renderer* renderer);

int runGame() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed" << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Noble Duel Engine Showcase", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!window) {
        std::cerr << "SDL_CreateWindow failed" << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);;

    if (!renderer) {
        std::cerr << "SDL_CreateRendered failed" << std::endl;
        return 1;
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init failed" << std::endl;
    } 


    mainLoop(renderer);
    return 0;

}

void mainLoop(SDL_Renderer* renderer) {
    ObjectMap object_map;
    ItemManager item_manager;
    std::vector<Object*> all_objects;

    SDL_Texture* base_ground_texture = IMG_LoadTexture(renderer, "resources/base_ground_.png");
    int base_ground_width_int, base_ground_height_int;
    SDL_QueryTexture(base_ground_texture, NULL, NULL, &base_ground_width_int, &base_ground_height_int);
    float base_ground_width = (float) base_ground_width_int;
    float base_ground_height = (float) base_ground_height_int;
    Point base_ground_center(WINDOW_WIDTH/2, -(WINDOW_HEIGHT+25));
    std::vector<Point> base_ground_relative_hull = {Point(-base_ground_width/2, -base_ground_height/2), Point(base_ground_width/2, -base_ground_height/2), Point(base_ground_width/2, base_ground_height/2), Point(-base_ground_width/2, base_ground_height/2)};
    Hitbox base_ground_hitbox(base_ground_center, base_ground_relative_hull);
    Model base_ground_model(base_ground_center, base_ground_texture, base_ground_hitbox, base_ground_width, base_ground_height);
    std::list<Model> base_ground_model_cycle_list = {base_ground_model};
    ModelCycle base_ground_model_cycle(base_ground_model_cycle_list);
    std::unordered_map<State, ModelCycle&> base_ground_hashmap = {{IDLE, base_ground_model_cycle}};
    ModelCollection base_ground_model_collection(base_ground_hashmap);
    Object base_ground(renderer, base_ground_center, base_ground_model_collection, SOLID);
    object_map.add_to_map(base_ground);
    all_objects.push_back(&base_ground); 

    SDL_Texture* upper_central_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_long_ground_.png");
    int upper_central_floating_width_int, upper_central_floating_height_int;
    SDL_QueryTexture(upper_central_floating_texture, NULL, NULL, &upper_central_floating_width_int, &upper_central_floating_height_int);
    float upper_central_floating_width = (float) upper_central_floating_width_int;
    float upper_central_floating_height = (float) upper_central_floating_height_int;
    Point upper_central_floating_center(WINDOW_WIDTH/2, -345);
    std::vector<Point> upper_central_floating_relative_hull = {Point(-upper_central_floating_width/2, -upper_central_floating_height/2), Point(upper_central_floating_width/2, -upper_central_floating_height/2), Point(upper_central_floating_width/2, upper_central_floating_height/2), Point(-upper_central_floating_width/2, upper_central_floating_height/2)};
    Hitbox upper_central_floating_hitbox(upper_central_floating_center, upper_central_floating_relative_hull);
    Model upper_central_floating_model(upper_central_floating_center, upper_central_floating_texture, upper_central_floating_hitbox, upper_central_floating_width, upper_central_floating_height);
    std::list<Model> upper_central_floating_model_cycle_list = {upper_central_floating_model};
    ModelCycle upper_central_floating_model_cycle(upper_central_floating_model_cycle_list);
    std::unordered_map<State, ModelCycle&> upper_central_floating_hashmap = {{IDLE, upper_central_floating_model_cycle}};
    ModelCollection upper_central_floating_model_collection(upper_central_floating_hashmap);
    Object upper_central_floating(renderer, upper_central_floating_center, upper_central_floating_model_collection, SOLID);
    object_map.add_to_map(upper_central_floating);
    all_objects.push_back(&upper_central_floating);

    SDL_Texture* left_medium_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_short_ground_.png");
    int left_medium_floating_width_int, left_medium_floating_height_int;
    SDL_QueryTexture(left_medium_floating_texture, NULL, NULL, &left_medium_floating_width_int, &left_medium_floating_height_int);
    float left_medium_floating_width = (float) left_medium_floating_width_int;
    float left_medium_floating_height = (float) left_medium_floating_height_int;
    Point left_medium_floating_center(300, -460);
    std::vector<Point> left_medium_floating_relative_hull = {Point(-left_medium_floating_width/2, -left_medium_floating_height/2), Point(left_medium_floating_width/2, -left_medium_floating_height/2), Point(left_medium_floating_width/2, left_medium_floating_height/2), Point(-left_medium_floating_width/2, left_medium_floating_height/2)};
    Hitbox left_medium_floating_hitbox(left_medium_floating_center, left_medium_floating_relative_hull);
    Model left_medium_floating_model(left_medium_floating_center, left_medium_floating_texture, left_medium_floating_hitbox, left_medium_floating_width, left_medium_floating_height);
    std::list<Model> left_medium_floating_model_cycle_list = {left_medium_floating_model};
    ModelCycle left_medium_floating_model_cycle(left_medium_floating_model_cycle_list);
    std::unordered_map<State, ModelCycle&> left_medium_floating_hashmap = {{IDLE, left_medium_floating_model_cycle}};
    ModelCollection left_medium_floating_model_collection(left_medium_floating_hashmap);
    Object left_medium_floating(renderer, left_medium_floating_center, left_medium_floating_model_collection, SOLID);
    object_map.add_to_map(left_medium_floating);
    all_objects.push_back(&left_medium_floating);

    SDL_Texture* right_medium_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_short_ground_.png");
    int right_medium_floating_width_int, right_medium_floating_height_int;
    SDL_QueryTexture(right_medium_floating_texture, NULL, NULL, &right_medium_floating_width_int, &right_medium_floating_height_int);
    float right_medium_floating_width = (float) right_medium_floating_width_int;
    float right_medium_floating_height = (float) right_medium_floating_height_int;
    Point right_medium_floating_center(1100, -460);
    std::vector<Point> right_medium_floating_relative_hull = {Point(-right_medium_floating_width/2, -right_medium_floating_height/2), Point(right_medium_floating_width/2, -right_medium_floating_height/2), Point(right_medium_floating_width/2, right_medium_floating_height/2), Point(-right_medium_floating_width/2, right_medium_floating_height/2)};
    Hitbox right_medium_floating_hitbox(right_medium_floating_center, right_medium_floating_relative_hull);
    Model right_medium_floating_model(right_medium_floating_center, right_medium_floating_texture, right_medium_floating_hitbox, right_medium_floating_width, right_medium_floating_height);
    std::list<Model> right_medium_floating_model_cycle_list = {right_medium_floating_model};
    ModelCycle right_medium_floating_model_cycle(right_medium_floating_model_cycle_list);
    std::unordered_map<State, ModelCycle&> right_medium_floating_hashmap = {{IDLE, right_medium_floating_model_cycle}};
    ModelCollection right_medium_floating_model_collection(right_medium_floating_hashmap);
    Object right_medium_floating(renderer, right_medium_floating_center, right_medium_floating_model_collection, SOLID);
    object_map.add_to_map(right_medium_floating);
    all_objects.push_back(&right_medium_floating);

    SDL_Texture* left_small_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_very_short_ground_.png");
    int left_small_floating_width_int, left_small_floating_height_int;
    SDL_QueryTexture(left_small_floating_texture, NULL, NULL, &left_small_floating_width_int, &left_small_floating_height_int);
    float left_small_floating_width = (float) left_small_floating_width_int;
    float left_small_floating_height = (float) left_small_floating_height_int;
    Point left_small_floating_center(275, -310);
    std::vector<Point> left_small_floating_relative_hull = {Point(-left_small_floating_width/2, -left_small_floating_height/2), Point(left_small_floating_width/2, -left_small_floating_height/2), Point(left_small_floating_width/2, left_small_floating_height/2), Point(-left_small_floating_width/2, left_small_floating_height/2)};
    Hitbox left_small_floating_hitbox(left_small_floating_center, left_small_floating_relative_hull);
    Model left_small_floating_model(left_small_floating_center, left_small_floating_texture, left_small_floating_hitbox, left_small_floating_width, left_small_floating_height);
    std::list<Model> left_small_floating_model_cycle_list = {left_small_floating_model};
    ModelCycle left_small_floating_model_cycle(left_small_floating_model_cycle_list);
    std::unordered_map<State, ModelCycle&> left_small_floating_hashmap = {{IDLE, left_small_floating_model_cycle}};
    ModelCollection left_small_floating_model_collection(left_small_floating_hashmap);
    Object left_small_floating(renderer, left_small_floating_center, left_small_floating_model_collection, SOLID);
    object_map.add_to_map(left_small_floating);
    all_objects.push_back(&left_small_floating);

    SDL_Texture* right_small_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_very_short_ground_.png");
    int right_small_floating_width_int, right_small_floating_height_int;
    SDL_QueryTexture(right_small_floating_texture, NULL, NULL, &right_small_floating_width_int, &right_small_floating_height_int);
    float right_small_floating_width = (float) right_small_floating_width_int;
    float right_small_floating_height = (float) right_small_floating_height_int;
    Point right_small_floating_center(1125, -310);
    std::vector<Point> right_small_floating_relative_hull = {Point(-right_small_floating_width/2, -right_small_floating_height/2), Point(right_small_floating_width/2, -right_small_floating_height/2), Point(right_small_floating_width/2, right_small_floating_height/2), Point(-right_small_floating_width/2, right_small_floating_height/2)};
    Hitbox right_small_floating_hitbox(right_small_floating_center, right_small_floating_relative_hull);
    Model right_small_floating_model(right_small_floating_center, right_small_floating_texture, right_small_floating_hitbox, right_small_floating_width, right_small_floating_height);
    std::list<Model> right_small_floating_model_cycle_list = {right_small_floating_model};
    ModelCycle right_small_floating_model_cycle(right_small_floating_model_cycle_list);
    std::unordered_map<State, ModelCycle&> right_small_floating_hashmap = {{IDLE, right_small_floating_model_cycle}};
    ModelCollection right_small_floating_model_collection(right_small_floating_hashmap);
    Object right_small_floating(renderer, right_small_floating_center, right_small_floating_model_collection, SOLID);
    object_map.add_to_map(right_small_floating);
    all_objects.push_back(&right_small_floating);

    SDL_Texture* left_branch_texture = IMG_LoadTexture(renderer, "resources/left_tree_branch_.png");
    int left_branch_width_int, left_branch_height_int;
    SDL_QueryTexture(left_branch_texture, NULL, NULL, &left_branch_width_int, &left_branch_height_int);
    float left_branch_width = (float) left_branch_width_int;
    float left_branch_height = (float) left_branch_height_int;
    Point left_branch_center(0, -160);
    std::vector<Point> left_branch_relative_hull = {Point(0,-17), Point(0,-43), Point(143,-46), Point(192,-20), Point(199,-7), Point(77,0)};
    Hitbox left_branch_hitbox(left_branch_center, left_branch_relative_hull);
    Model left_branch_model(left_branch_center, left_branch_texture, left_branch_hitbox, left_branch_width, left_branch_height, Point(0,0));
    std::list<Model> left_branch_model_cycle_list = {left_branch_model};
    ModelCycle left_branch_model_cycle(left_branch_model_cycle_list);
    std::unordered_map<State, ModelCycle&> left_branch_hashmap = {{IDLE, left_branch_model_cycle}};
    ModelCollection left_branch_model_collection(left_branch_hashmap);
    Object left_branch(renderer, left_branch_center, left_branch_model_collection, SOLID);
    object_map.add_to_map(left_branch);
    all_objects.push_back(&left_branch);

    SDL_Texture* right_branch_texture = IMG_LoadTexture(renderer, "resources/right_tree_branch_.png");
    int right_branch_width_int, right_branch_height_int;
    SDL_QueryTexture(right_branch_texture, NULL, NULL, &right_branch_width_int, &right_branch_height_int);
    float right_branch_width = (float) right_branch_width_int;
    float right_branch_height = (float) right_branch_height_int;
    Point right_branch_center(1201, -160);
    std::vector<Point> right_branch_relative_hull = {Point(122,0), Point(0,-7), Point(7,-20), Point(56,-46), Point(199,-43), Point(199,-17)};
    Hitbox right_branch_hitbox(right_branch_center, right_branch_relative_hull);
    Model right_branch_model(right_branch_center, right_branch_texture, right_branch_hitbox, right_branch_width, right_branch_height, Point(0,0));
    std::list<Model> right_branch_model_cycle_list = {right_branch_model};
    ModelCycle right_branch_model_cycle(right_branch_model_cycle_list);
    std::unordered_map<State, ModelCycle&> right_branch_hashmap = {{IDLE, right_branch_model_cycle}};
    ModelCollection right_branch_model_collection(right_branch_hashmap);
    Object right_branch(renderer, right_branch_center, right_branch_model_collection, SOLID);
    object_map.add_to_map(right_branch);
    all_objects.push_back(&right_branch);

    SDL_Texture* left_triangular_stone_texture = IMG_LoadTexture(renderer, "resources/triangular_left_facing_stone_.png");
    int left_triangular_stone_width_int, left_triangular_stone_height_int;
    SDL_QueryTexture(left_triangular_stone_texture, NULL, NULL, &left_triangular_stone_width_int, &left_triangular_stone_height_int);
    float left_triangular_stone_width = (float) left_triangular_stone_width_int;
    float left_triangular_stone_height = (float) left_triangular_stone_height_int;
    Point left_triangular_stone_center(200, -750);
    std::vector<Point> left_triangular_stone_relative_hull = {Point(-left_triangular_stone_width/2, -left_triangular_stone_height/2), Point(left_triangular_stone_width/2, -left_triangular_stone_height/2), Point(-left_triangular_stone_width/2, left_triangular_stone_height/2)};
    Hitbox left_triangular_stone_hitbox(left_triangular_stone_center, left_triangular_stone_relative_hull);
    Model left_triangular_stone_model(left_triangular_stone_center, left_triangular_stone_texture, left_triangular_stone_hitbox, left_triangular_stone_width, left_triangular_stone_height);
    std::list<Model> left_triangular_stone_model_cycle_list = {left_triangular_stone_model};
    ModelCycle left_triangular_stone_model_cycle(left_triangular_stone_model_cycle_list);
    std::unordered_map<State, ModelCycle&> left_triangular_stone_hashmap = {{IDLE, left_triangular_stone_model_cycle}};
    ModelCollection left_triangular_stone_model_collection(left_triangular_stone_hashmap);
    Object left_triangular_stone(renderer, left_triangular_stone_center, left_triangular_stone_model_collection, SOLID);
    object_map.add_to_map(left_triangular_stone);
    all_objects.push_back(&left_triangular_stone);

    SDL_Texture* right_triangular_stone_texture = IMG_LoadTexture(renderer, "resources/triangular_right_facing_stone_.png");
    int right_triangular_stone_width_int, right_triangular_stone_height_int;
    SDL_QueryTexture(right_triangular_stone_texture, NULL, NULL, &right_triangular_stone_width_int, &right_triangular_stone_height_int);
    float right_triangular_stone_width = (float) right_triangular_stone_width_int;
    float right_triangular_stone_height = (float) right_triangular_stone_height_int;
    Point right_triangular_stone_center(1200, -750);
    std::vector<Point> right_triangular_stone_relative_hull = {Point(-right_triangular_stone_width/2, -right_triangular_stone_height/2), Point(right_triangular_stone_width/2, -right_triangular_stone_height/2), Point(right_triangular_stone_width/2, right_triangular_stone_height/2)};
    Hitbox right_triangular_stone_hitbox(right_triangular_stone_center, right_triangular_stone_relative_hull);
    Model right_triangular_stone_model(right_triangular_stone_center, right_triangular_stone_texture, right_triangular_stone_hitbox, right_triangular_stone_width, right_triangular_stone_height);
    std::list<Model> right_triangular_stone_model_cycle_list = {right_triangular_stone_model};
    ModelCycle right_triangular_stone_model_cycle(right_triangular_stone_model_cycle_list);
    std::unordered_map<State, ModelCycle&> right_triangular_stone_hashmap = {{IDLE, right_triangular_stone_model_cycle}};
    ModelCollection right_triangular_stone_model_collection(right_triangular_stone_hashmap);
    Object right_triangular_stone(renderer, right_triangular_stone_center, right_triangular_stone_model_collection, SOLID);
    object_map.add_to_map(right_triangular_stone);
    all_objects.push_back(&right_triangular_stone);

    SDL_Texture* left_stone_texture = IMG_LoadTexture(renderer, "resources/right_facing_stone_.png");
    int left_stone_width_int, left_stone_height_int;
    SDL_QueryTexture(left_stone_texture, NULL, NULL, &left_stone_width_int, &left_stone_height_int);
    float left_stone_width = (float) left_stone_width_int;
    float left_stone_height = (float) left_stone_height_int;
    Point left_stone_center((WINDOW_WIDTH/2)-250, -515);
    std::vector<Point> left_stone_relative_hull = {Point(250,0), Point(163,-25), Point(95,-64), Point(0,-200), Point(250,-200)};
    Hitbox left_stone_hitbox(left_stone_center, left_stone_relative_hull);
    Model left_stone_model(left_stone_center, left_stone_texture, left_stone_hitbox, left_stone_width, left_stone_height, Point(0,0));
    std::list<Model> left_stone_model_cycle_list = {left_stone_model};
    ModelCycle left_stone_model_cycle(left_stone_model_cycle_list);
    std::unordered_map<State, ModelCycle&> left_stone_hashmap = {{IDLE, left_stone_model_cycle}};
    ModelCollection left_stone_model_collection(left_stone_hashmap);
    Object left_stone(renderer, left_stone_center, left_stone_model_collection, SOLID);
    object_map.add_to_map(left_stone);
    all_objects.push_back(&left_stone);

    SDL_Texture* right_stone_texture = IMG_LoadTexture(renderer, "resources/left_facing_stone_.png");
    int right_stone_width_int, right_stone_height_int;
    SDL_QueryTexture(right_stone_texture, NULL, NULL, &right_stone_width_int, &right_stone_height_int);
    float right_stone_width = (float) right_stone_width_int;
    float right_stone_height = (float) right_stone_height_int;
    Point right_stone_center((WINDOW_WIDTH/2), -515);
    std::vector<Point> right_stone_relative_hull = {Point(0,-200), Point(250,-200), Point(155,-64), Point(87,-25), Point(0,0)};
    Hitbox right_stone_hitbox(right_stone_center, right_stone_relative_hull);
    Model right_stone_model(right_stone_center, right_stone_texture, right_stone_hitbox, right_stone_width, right_stone_height, Point(0,0));
    std::list<Model> right_stone_model_cycle_list = {right_stone_model};
    ModelCycle right_stone_model_cycle(right_stone_model_cycle_list);
    std::unordered_map<State, ModelCycle&> right_stone_hashmap = {{IDLE, right_stone_model_cycle}};
    ModelCollection right_stone_model_collection(right_stone_hashmap);
    Object right_stone(renderer, right_stone_center, right_stone_model_collection, SOLID);
    object_map.add_to_map(right_stone);
    all_objects.push_back(&right_stone);

    SDL_Texture* lower_central_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_slightly_longer_ground_.png");
    int lower_central_floating_width_int, lower_central_floating_height_int;
    SDL_QueryTexture(lower_central_floating_texture, NULL, NULL, &lower_central_floating_width_int, &lower_central_floating_height_int);
    float lower_central_floating_width = (float) lower_central_floating_width_int;
    float lower_central_floating_height = (float) lower_central_floating_height_int;
    Point lower_central_floating_center(WINDOW_WIDTH/2, -715);
    std::vector<Point> lower_central_floating_relative_hull = {Point(-lower_central_floating_width/2, -lower_central_floating_height/2), Point(lower_central_floating_width/2, -lower_central_floating_height/2), Point(lower_central_floating_width/2, lower_central_floating_height/2), Point(-lower_central_floating_width/2, lower_central_floating_height/2)};
    Hitbox lower_central_floating_hitbox(lower_central_floating_center, lower_central_floating_relative_hull);
    Model lower_central_floating_model(lower_central_floating_center, lower_central_floating_texture, lower_central_floating_hitbox, lower_central_floating_width, lower_central_floating_height);
    std::list<Model> lower_central_floating_model_cycle_list = {lower_central_floating_model};
    ModelCycle lower_central_floating_model_cycle(lower_central_floating_model_cycle_list);
    std::unordered_map<State, ModelCycle&> lower_central_floating_hashmap = {{IDLE, lower_central_floating_model_cycle}};
    ModelCollection lower_central_floating_model_collection(lower_central_floating_hashmap);
    Object lower_central_floating(renderer, lower_central_floating_center, lower_central_floating_model_collection, SOLID);
    object_map.add_to_map(lower_central_floating);
    all_objects.push_back(&lower_central_floating);

    Point rapier_starting_point(WINDOW_WIDTH/2, 0);
    SDL_Texture* golden_icon_texture = IMG_LoadTexture(renderer, "resources/golden_icon.png");
    SDL_Texture* left_facing_rapier_texture = IMG_LoadTexture(renderer, "resources/nobles_rapier_left.png");
    SDL_Texture* right_facing_rapier_texture = IMG_LoadTexture(renderer, "resources/nobles_rapier_right.png");
    SDL_Texture* charged_left_facing_rapier_texture = IMG_LoadTexture(renderer, "resources/charge_nobles_rapier_left.png");
    SDL_Texture* charged_right_facing_rapier_texture = IMG_LoadTexture(renderer, "resources/charge_nobles_rapier_right.png");

    int golden_icon_width_, golden_icon_height_;
    SDL_QueryTexture(golden_icon_texture, NULL, NULL, &golden_icon_width_, &golden_icon_height_);
    float golden_icon_width = (float) golden_icon_width_;
    float golden_icon_height = (float) golden_icon_height_;
    std::vector<Point> golden_icon_relative_hull = {Point(-golden_icon_width/2, -golden_icon_height/2), 
                                                        Point(golden_icon_width/2, -golden_icon_height/2), 
                                                        Point(golden_icon_width/2, golden_icon_height/2),
                                                        Point(-golden_icon_width/2, golden_icon_height/2)};
    Hitbox golden_icon_hitbox(rapier_starting_point, golden_icon_relative_hull);
    Model golden_icon_model(rapier_starting_point, golden_icon_texture, golden_icon_hitbox, golden_icon_width, golden_icon_height); 
    std::list<Model> golden_icon_model_list = {golden_icon_model};
    ModelCycle golden_icon_model_cycle(golden_icon_model_list);

    int left_facing_rapier_width_, left_facing_rapier_height_;
    SDL_QueryTexture(left_facing_rapier_texture, NULL, NULL, &left_facing_rapier_width_, &left_facing_rapier_height_);
    float left_facing_rapier_width = (float) left_facing_rapier_width_;
    float left_facing_rapier_height = (float) left_facing_rapier_height_;
    std::vector<Point> left_facing_rapier_relative_hull = {Point(-52,-2), Point(-49,-5), Point(-43,-5), Point(-43,1), Point(-49,1)};
    Hitbox left_facing_rapier_hitbox(rapier_starting_point, left_facing_rapier_relative_hull);
    Model left_facing_rapier_model(rapier_starting_point, left_facing_rapier_texture, left_facing_rapier_hitbox, left_facing_rapier_width, left_facing_rapier_height, Point(-63,17));
    std::list<Model> left_facing_rapier_model_list = {left_facing_rapier_model};
    ModelCycle left_facing_rapier_model_cycle(left_facing_rapier_model_list);

    int right_facing_rapier_width_, right_facing_rapier_height_;
    SDL_QueryTexture(right_facing_rapier_texture, NULL, NULL, &right_facing_rapier_width_, &right_facing_rapier_height_);
    float right_facing_rapier_width = (float) right_facing_rapier_width_;
    float right_facing_rapier_height = (float) right_facing_rapier_height_;
    std::vector<Point> right_facing_rapier_relative_hull = {Point(49,1), Point(43,1), Point(43,-5), Point(49,-5), Point(52,-2)};
    Hitbox right_facing_rapier_hitbox(rapier_starting_point, right_facing_rapier_relative_hull);
    Model right_facing_rapier_model(rapier_starting_point, right_facing_rapier_texture, right_facing_rapier_hitbox, right_facing_rapier_width, right_facing_rapier_height, Point(-17,17));
    std::list<Model> right_facing_rapier_model_list = {right_facing_rapier_model};
    ModelCycle right_facing_rapier_model_cycle(right_facing_rapier_model_list);

    int charged_left_facing_rapier_width_, charged_left_facing_rapier_height_;
    SDL_QueryTexture(charged_left_facing_rapier_texture, NULL, NULL, &charged_left_facing_rapier_width_, &charged_left_facing_rapier_height_);
    float charged_left_facing_rapier_width = (float) charged_left_facing_rapier_width_;
    float charged_left_facing_rapier_height = (float) charged_left_facing_rapier_height_;
    std::vector<Point> charged_left_facing_rapier_relative_hull = {Point(-52,-2), Point(-49,-5), Point(-43,-5), Point(-43,1), Point(-49,1)};
    Hitbox charged_left_facing_rapier_hitbox(rapier_starting_point, charged_left_facing_rapier_relative_hull);
    Model charged_left_facing_rapier_model(rapier_starting_point, charged_left_facing_rapier_texture, charged_left_facing_rapier_hitbox, charged_left_facing_rapier_width, charged_left_facing_rapier_height, Point(-63,17));
    std::list<Model> charged_left_facing_rapier_model_list = {charged_left_facing_rapier_model};
    ModelCycle charged_left_facing_rapier_model_cycle(charged_left_facing_rapier_model_list);

    int charged_right_facing_rapier_width_, charged_right_facing_rapier_height_;
    SDL_QueryTexture(charged_right_facing_rapier_texture, NULL, NULL, &charged_right_facing_rapier_width_, &charged_right_facing_rapier_height_);
    float charged_right_facing_rapier_width = (float) charged_right_facing_rapier_width_;
    float charged_right_facing_rapier_height = (float) charged_right_facing_rapier_height_;
    std::vector<Point> charged_right_facing_rapier_relative_hull = {Point(49,1), Point(43,1), Point(43,-5), Point(49,-5), Point(52,-2)};
    Hitbox charged_right_facing_rapier_hitbox(rapier_starting_point, charged_right_facing_rapier_relative_hull);
    Model charged_right_facing_rapier_model(rapier_starting_point, charged_right_facing_rapier_texture, charged_right_facing_rapier_hitbox, charged_right_facing_rapier_width, charged_right_facing_rapier_height, Point(-17,17));
    std::list<Model> charged_right_facing_rapier_model_list = {charged_right_facing_rapier_model};
    ModelCycle charged_right_facing_rapier_model_cycle(charged_right_facing_rapier_model_list);

    std::unordered_map<State, ModelCycle&> rapier_model_map = {{IDLE, golden_icon_model_cycle},
                                                                   {OWNED_LEFT, left_facing_rapier_model_cycle},
                                                                   {IN_USE_LEFT, left_facing_rapier_model_cycle},
                                                                   {IN_CHARGED_USE_LEFT, charged_left_facing_rapier_model_cycle},
                                                                   {OWNED_RIGHT, right_facing_rapier_model_cycle},
                                                                   {IN_USE_RIGHT, right_facing_rapier_model_cycle},
                                                                   {IN_CHARGED_USE_RIGHT, charged_right_facing_rapier_model_cycle}};
    ModelCollection rapier_model_collection(rapier_model_map);
    ThrustingWeapon rapier(renderer, rapier_starting_point, rapier_model_collection, LIGHT_PHANTOM, object_map, 1000, 1.5, 25, 35, 20, 10, 10);
    object_map.add_to_map(rapier);
    all_objects.push_back(&rapier);   
    item_manager.add_item(rapier);                                                        

    Point windowCenter = Point(WINDOW_WIDTH/2, 0); // sprite
    SDL_Texture* texture = IMG_LoadTexture(renderer, "resources/first_sprite_tbg_2.png");
    int sprite_width_, sprite_height_;
    SDL_QueryTexture(texture, NULL, NULL, &sprite_width_, &sprite_height_);
    float sprite_width = (float) sprite_width_;
    float sprite_height = (float) sprite_height_;
    // std::vector<Point> sprite_relative_hull = {Point(0,25), Point(-20,17), Point(-24,6), Point(-24,-5), Point(-18,-16), Point(0,-23), Point(21,-18), Point(24,1), Point(16,20)};
    std::vector<Point> sprite_relative_hull = {Point(-sprite_width/2,-sprite_height/2), Point(sprite_width/2,-sprite_height/2), Point(sprite_width/2,sprite_height/2), Point(-sprite_width/2,sprite_height/2)};
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    Hitbox hitbox(windowCenter , sprite_relative_hull);
    Model testModel(windowCenter, texture, hitbox, sprite_width, sprite_height);
    std::list<Model> modelList;
    modelList.push_back(testModel);
    ModelCycle testModelCycle(modelList); 
    std::unordered_map<State, ModelCycle&> cycleMap = {{IDLE, testModelCycle}};
    ModelCollection testModelCollection(cycleMap);
    Sprite testPlayer(renderer, windowCenter, testModelCollection, SOLID, object_map, {SDL_SCANCODE_SPACE}, 100, item_manager);
    object_map.add_to_map(testPlayer);
    all_objects.push_back(&testPlayer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
    
    std::thread framerate_cap_thread;
    int mouse_x, mouse_y;
    bool running = true;
    SDL_Event event;

    while (running) {
        framerate_cap_thread = std::thread([](){std::this_thread::sleep_for(std::chrono::milliseconds((int) 1000.0/FPS));});
        SDL_GetMouseState(&mouse_x, &mouse_y);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  
        testPlayer.update_targeted_point(Point((float) mouse_x, (float) -mouse_y));   
        item_manager.manage_scheduled_running();
        testPlayer.read_inputs();   
        testPlayer.run_scheduled();   
        for (Object* o : all_objects) {
            (*o).redraw_object();
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
        framerate_cap_thread.join();
        current_session_global_game_clock.increment();
        SDL_RenderPresent(renderer);
    }

    
    SDL_DestroyTexture(base_ground_texture);
    SDL_DestroyTexture(upper_central_floating_texture);
    SDL_DestroyTexture(lower_central_floating_texture);
    SDL_DestroyTexture(left_medium_floating_texture);
    SDL_DestroyTexture(right_medium_floating_texture);
    SDL_DestroyTexture(left_small_floating_texture);
    SDL_DestroyTexture(right_small_floating_texture);
    SDL_DestroyTexture(left_branch_texture);
    SDL_DestroyTexture(right_branch_texture);
    SDL_DestroyTexture(left_triangular_stone_texture);
    SDL_DestroyTexture(right_triangular_stone_texture);
    SDL_DestroyTexture(left_stone_texture);
    SDL_DestroyTexture(right_stone_texture);
    SDL_DestroyTexture(golden_icon_texture);
    SDL_DestroyTexture(left_facing_rapier_texture);
    SDL_DestroyTexture(right_facing_rapier_texture);
    SDL_DestroyTexture(charged_left_facing_rapier_texture);
    SDL_DestroyTexture(charged_right_facing_rapier_texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}


int main() {
    return runGame();
}
