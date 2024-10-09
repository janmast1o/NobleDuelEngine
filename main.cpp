#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <thread>
#include <chrono>
#include "source_code/sprite.cpp"

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


    // SDL_Texture* right_small_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_very_short_ground_.png");
    // int right_small_floating_width_int, right_small_floating_height_int;
    // SDL_QueryTexture(right_small_floating_texture, NULL, NULL, &right_small_floating_width_int, &right_small_floating_height_int);
    // float right_small_floating_width = (float) right_small_floating_width_int;
    // float right_small_floating_height = (float) right_small_floating_height_int;
    // Point right_small_floating_center(1100, -310);
    // std::vector<Point> right_small_floating_relative_hull = {Point(-right_small_floating_width/2, -right_small_floating_height/2), Point(right_small_floating_width/2, -right_small_floating_height/2), Point(right_small_floating_width/2, right_small_floating_height/2), Point(-right_small_floating_width/2, right_small_floating_height/2)};
    // Hitbox right_small_floating_hitbox(right_small_floating_center, right_small_floating_relative_hull);
    // Model right_small_floating_model(right_small_floating_center, right_small_floating_texture, right_small_floating_hitbox, right_small_floating_width, right_small_floating_height);
    // std::list<Model> right_small_floating_model_cycle_list = {right_small_floating_model};
    // ModelCycle right_small_floating_model_cycle(right_small_floating_model_cycle_list);
    // std::unordered_map<State, ModelCycle&> right_small_floating_hashmap = {{IDLE, right_small_floating_model_cycle}};
    // ModelCollection right_small_floating_model_collection(right_small_floating_hashmap);
    // Object right_small_floating(renderer, right_small_floating_center, right_small_floating_model_collection, SOLID);
    // object_map.add_to_map(right_small_floating);
    // all_objects.push_back(&right_small_floating);

    // SDL_Texture* minature_left_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_very_short_ground_.png");
    // int minature_left_floating_width_int, minature_left_floating_height_int;
    // SDL_QueryTexture(minature_left_floating_texture, NULL, NULL, &minature_left_floating_width_int, &minature_left_floating_height_int);
    // float minature_left_floating_width = (float) minature_left_floating_width_int;
    // float minature_left_floating_height = (float) minature_left_floating_height_int;
    // Point minature_left_floating_center(175, -520);
    // std::vector<Point> minature_left_floating_relative_hull = {Point(-minature_left_floating_width/2, -minature_left_floating_height/2), Point(minature_left_floating_width/2, -minature_left_floating_height/2), Point(minature_left_floating_width/2, minature_left_floating_height/2), Point(-minature_left_floating_width/2, minature_left_floating_height/2)};
    // Hitbox minature_left_floating_hitbox(minature_left_floating_center, minature_left_floating_relative_hull);
    // Model minature_left_floating_model(minature_left_floating_center, minature_left_floating_texture, minature_left_floating_hitbox, minature_left_floating_width, minature_left_floating_height);
    // std::list<Model> minature_left_floating_model_cycle_list = {minature_left_floating_model};
    // ModelCycle minature_left_floating_model_cycle(minature_left_floating_model_cycle_list);
    // std::unordered_map<State, ModelCycle&> minature_left_floating_hashmap = {{IDLE, minature_left_floating_model_cycle}};
    // ModelCollection minature_left_floating_model_collection(minature_left_floating_hashmap);
    // Object minature_left_floating(renderer, minature_left_floating_center, minature_left_floating_model_collection, SOLID);
    // object_map.add_to_map(minature_left_floating);
    // all_objects.push_back(&minature_left_floating);

    // SDL_Texture* minature_right_floating_texture = IMG_LoadTexture(renderer, "resources/short_and_very_short_ground_.png");
    // int minature_right_floating_width_int, minature_right_floating_height_int;
    // SDL_QueryTexture(minature_right_floating_texture, NULL, NULL, &minature_right_floating_width_int, &minature_right_floating_height_int);
    // float minature_right_floating_width = (float) minature_right_floating_width_int;
    // float minature_right_floating_height = (float) minature_right_floating_height_int;
    // Point minature_right_floating_center(1250, -520);
    // std::vector<Point> minature_right_floating_relative_hull = {Point(-minature_right_floating_width/2, -minature_right_floating_height/2), Point(minature_right_floating_width/2, -minature_right_floating_height/2), Point(minature_right_floating_width/2, minature_right_floating_height/2), Point(-minature_right_floating_width/2, minature_right_floating_height/2)};
    // Hitbox minature_right_floating_hitbox(minature_right_floating_center, minature_right_floating_relative_hull);
    // Model minature_right_floating_model(minature_right_floating_center, minature_right_floating_texture, minature_right_floating_hitbox, minature_right_floating_width, minature_right_floating_height);
    // std::list<Model> minature_right_floating_model_cycle_list = {minature_right_floating_model};
    // ModelCycle minature_right_floating_model_cycle(minature_right_floating_model_cycle_list);
    // std::unordered_map<State, ModelCycle&> minature_right_floating_hashmap = {{IDLE, minature_right_floating_model_cycle}};
    // ModelCollection minature_right_floating_model_collection(minature_right_floating_hashmap);
    // Object minature_right_floating(renderer, minature_right_floating_center, minature_right_floating_model_collection, SOLID);
    // object_map.add_to_map(minature_right_floating);
    // all_objects.push_back(&minature_right_floating);

    // SDL_Texture* first_hl_texture = IMG_LoadTexture(renderer, "resources/vertical_long_block.png"); // first vertical long
    // int first_hl_width_, first_hl_height_;
    // SDL_QueryTexture(first_hl_texture, NULL, NULL, &first_hl_width_, &first_hl_height_);
    // float first_hl_width = (float) first_hl_width_;
    // float first_hl_height = (float) first_hl_height_;
    // Point first_hl_center(first_hl_width*(0.6), -first_hl_height*(0.6));
    // std::vector<Point> first_hl_hitbox_relative_hull = {Point(-first_hl_width/2,-first_hl_height/2), Point(first_hl_width/2,-first_hl_height/2), Point(first_hl_width/2,first_hl_height/2), Point(-first_hl_width/2,first_hl_height/2)};
    // Hitbox first_hl_hitbox(first_hl_center, first_hl_hitbox_relative_hull);
    // Model first_hl_model(first_hl_center, first_hl_texture, first_hl_hitbox, first_hl_width, first_hl_height);
    // std::list<Model> first_hl_list;
    // first_hl_list.push_back(first_hl_model);
    // ModelCycle first_hl_model_cycle(first_hl_list);
    // std::unordered_map<State, ModelCycle&> first_hl_map = {{IDLE, first_hl_model_cycle}};
    // ModelCollection first_hl_model_collection(first_hl_map);
    // Object first_hl(renderer, first_hl_center, first_hl_model_collection, SOLID);
    // object_map.add_to_map(first_hl);
    // all_objects.push_back(&first_hl);

    // SDL_Texture* second_hl_texture = IMG_LoadTexture(renderer, "resources/horizontal_short_block.png"); // second horizontal short 
    // int second_hl_width_, second_hl_height_;
    // SDL_QueryTexture(second_hl_texture, NULL, NULL, &second_hl_width_, &second_hl_height_);
    // float second_hl_width = (float) second_hl_width_;
    // float second_hl_height = (float) second_hl_height_;
    // Point second_hl_center(300, -200);
    // std::vector<Point> second_hl_hitbox_relative_hull = {Point(-second_hl_width/2,-second_hl_height/2), Point(second_hl_width/2,-second_hl_height/2), Point(second_hl_width/2,second_hl_height/2), Point(-second_hl_width/2,second_hl_height/2)};
    // Hitbox second_hl_hitbox(second_hl_center, second_hl_hitbox_relative_hull);
    // Model second_hl_model(second_hl_center, second_hl_texture, second_hl_hitbox, second_hl_width, second_hl_height);
    // std::list<Model> second_hl_list;
    // second_hl_list.push_back(second_hl_model);
    // ModelCycle second_hl_model_cycle(second_hl_list);
    // std::unordered_map<State, ModelCycle&> second_hl_map = {{IDLE, second_hl_model_cycle}};
    // ModelCollection second_hl_model_collection(second_hl_map);
    // Object second_hl(renderer, second_hl_center, second_hl_model_collection, SOLID);
    // object_map.add_to_map(second_hl);
    // all_objects.push_back(&second_hl);

    // SDL_Texture* third_hl_texture = IMG_LoadTexture(renderer, "resources/horizontal_long_block.png"); // third horizontal long 
    // int third_hl_width_, third_hl_height_;
    // SDL_QueryTexture(third_hl_texture, NULL, NULL, &third_hl_width_, &third_hl_height_);
    // float third_hl_width = (float) third_hl_width_;
    // float third_hl_height = (float) third_hl_height_;
    // Point third_hl_center(460, -680);
    // std::vector<Point> third_hl_hitbox_relative_hull = {Point(-third_hl_width/2,-third_hl_height/2), Point(third_hl_width/2,-third_hl_height/2), Point(third_hl_width/2,third_hl_height/2), Point(-third_hl_width/2,third_hl_height/2)};
    // Hitbox third_hl_hitbox(third_hl_center, third_hl_hitbox_relative_hull);
    // Model third_hl_model(third_hl_center, third_hl_texture, third_hl_hitbox, third_hl_width, third_hl_height);
    // std::list<Model> third_hl_list;
    // third_hl_list.push_back(third_hl_model);
    // ModelCycle third_hl_model_cycle(third_hl_list);
    // std::unordered_map<State, ModelCycle&> third_hl_map = {{IDLE, third_hl_model_cycle}};
    // ModelCollection third_hl_model_collection(third_hl_map);
    // Object third_hl(renderer, third_hl_center, third_hl_model_collection, SOLID);
    // object_map.add_to_map(third_hl);
    // all_objects.push_back(&third_hl);

    // SDL_Texture* fourth_hl_texture = IMG_LoadTexture(renderer, "resources/horizontal_long_block.png"); // fourth horizontal long 
    // int fourth_hl_width_, fourth_hl_height_;
    // SDL_QueryTexture(fourth_hl_texture, NULL, NULL, &fourth_hl_width_, &fourth_hl_height_);
    // float fourth_hl_width = (float) fourth_hl_width_;
    // float fourth_hl_height = (float) fourth_hl_height_;
    // Point fourth_hl_center(560, -680);
    // std::vector<Point> fourth_hl_hitbox_relative_hull = {Point(-fourth_hl_width/2,-fourth_hl_height/2), Point(fourth_hl_width/2,-fourth_hl_height/2), Point(fourth_hl_width/2,fourth_hl_height/2), Point(-fourth_hl_width/2,fourth_hl_height/2)};
    // Hitbox fourth_hl_hitbox(fourth_hl_center, fourth_hl_hitbox_relative_hull);
    // Model fourth_hl_model(fourth_hl_center, fourth_hl_texture, fourth_hl_hitbox, fourth_hl_width, fourth_hl_height);
    // std::list<Model> fourth_hl_list;
    // fourth_hl_list.push_back(fourth_hl_model);
    // ModelCycle fourth_hl_model_cycle(fourth_hl_list);
    // std::unordered_map<State, ModelCycle&> fourth_hl_map = {{IDLE, fourth_hl_model_cycle}};
    // ModelCollection fourth_hl_model_collection(fourth_hl_map);
    // Object fourth_hl(renderer, fourth_hl_center, fourth_hl_model_collection, SOLID);
    // object_map.add_to_map(fourth_hl);
    // all_objects.push_back(&fourth_hl);

    // SDL_Texture* fifth_hl_texture = IMG_LoadTexture(renderer, "resources/horizontal_long_block.png"); // fifth horizontal long 
    // int fifth_hl_width_, fifth_hl_height_;
    // SDL_QueryTexture(fifth_hl_texture, NULL, NULL, &fifth_hl_width_, &fifth_hl_height_);
    // float fifth_hl_width = (float) fifth_hl_width_;
    // float fifth_hl_height = (float) fifth_hl_height_;
    // Point fifth_hl_center(680, -680);
    // std::vector<Point> fifth_hl_hitbox_relative_hull = {Point(-fifth_hl_width/2,-fifth_hl_height/2), Point(fifth_hl_width/2,-fifth_hl_height/2), Point(fifth_hl_width/2,fifth_hl_height/2), Point(-fifth_hl_width/2,fifth_hl_height/2)};
    // Hitbox fifth_hl_hitbox(fifth_hl_center, fifth_hl_hitbox_relative_hull);
    // Model fifth_hl_model(fifth_hl_center, fifth_hl_texture, fifth_hl_hitbox, fifth_hl_width, fifth_hl_height);
    // std::list<Model> fifth_hl_list;
    // fifth_hl_list.push_back(fifth_hl_model);
    // ModelCycle fifth_hl_model_cycle(fifth_hl_list);
    // std::unordered_map<State, ModelCycle&> fifth_hl_map = {{IDLE, fifth_hl_model_cycle}};
    // ModelCollection fifth_hl_model_collection(fifth_hl_map);
    // Object fifth_hl(renderer, fifth_hl_center, fifth_hl_model_collection, SOLID);
    // object_map.add_to_map(fifth_hl);
    // all_objects.push_back(&fifth_hl);

    // SDL_Texture* sixth_hl_texture = IMG_LoadTexture(renderer, "resources/horizontal_long_block.png"); // sixth horizontal long 
    // int sixth_hl_width_, sixth_hl_height_;
    // SDL_QueryTexture(sixth_hl_texture, NULL, NULL, &sixth_hl_width_, &sixth_hl_height_);
    // float sixth_hl_width = (float) sixth_hl_width_;
    // float sixth_hl_height = (float) sixth_hl_height_;
    // Point sixth_hl_center(760, -680);
    // std::vector<Point> sixth_hl_hitbox_relative_hull = {Point(-sixth_hl_width/2,-sixth_hl_height/2), Point(sixth_hl_width/2,-sixth_hl_height/2), Point(sixth_hl_width/2,sixth_hl_height/2), Point(-sixth_hl_width/2,sixth_hl_height/2)};
    // Hitbox sixth_hl_hitbox(sixth_hl_center, sixth_hl_hitbox_relative_hull);
    // Model sixth_hl_model(sixth_hl_center, sixth_hl_texture, sixth_hl_hitbox, sixth_hl_width, sixth_hl_height);
    // std::list<Model> sixth_hl_list;
    // sixth_hl_list.push_back(sixth_hl_model);
    // ModelCycle sixth_hl_model_cycle(sixth_hl_list);
    // std::unordered_map<State, ModelCycle&> sixth_hl_map = {{IDLE, sixth_hl_model_cycle}};
    // ModelCollection sixth_hl_model_collection(sixth_hl_map);
    // Object sixth_hl(renderer, sixth_hl_center, sixth_hl_model_collection, SOLID);
    // object_map.add_to_map(sixth_hl);
    // all_objects.push_back(&sixth_hl);

    // SDL_Texture* seventh_hl_texture = IMG_LoadTexture(renderer, "resources/vertical_short_block.png"); // seventh horizontal long 
    // int seventh_hl_width_, seventh_hl_height_;
    // SDL_QueryTexture(seventh_hl_texture, NULL, NULL, &seventh_hl_width_, &seventh_hl_height_);
    // float seventh_hl_width = (float) seventh_hl_width_;
    // float seventh_hl_height = (float) seventh_hl_height_;
    // Point seventh_hl_center(880, -680);
    // std::vector<Point> seventh_hl_hitbox_relative_hull = {Point(-seventh_hl_width/2,-seventh_hl_height/2), Point(seventh_hl_width/2,-seventh_hl_height/2), Point(seventh_hl_width/2,seventh_hl_height/2), Point(-seventh_hl_width/2,seventh_hl_height/2)};
    // Hitbox seventh_hl_hitbox(seventh_hl_center, seventh_hl_hitbox_relative_hull);
    // Model seventh_hl_model(seventh_hl_center, seventh_hl_texture, seventh_hl_hitbox, seventh_hl_width, seventh_hl_height);
    // std::list<Model> seventh_hl_list;
    // seventh_hl_list.push_back(seventh_hl_model);
    // ModelCycle seventh_hl_model_cycle(seventh_hl_list);
    // std::unordered_map<State, ModelCycle&> seventh_hl_map = {{IDLE, seventh_hl_model_cycle}};
    // ModelCollection seventh_hl_model_collection(seventh_hl_map);
    // Object seventh_hl(renderer, seventh_hl_center, seventh_hl_model_collection, SOLID);
    // object_map.add_to_map(seventh_hl);
    // all_objects.push_back(&seventh_hl);

    // SDL_Texture* eighth_hl_texture = IMG_LoadTexture(renderer, "resources/horizontal_short_block.png"); // eighth horizontal long 
    // int eighth_hl_width_, eighth_hl_height_;
    // SDL_QueryTexture(eighth_hl_texture, NULL, NULL, &eighth_hl_width_, &eighth_hl_height_);
    // float eighth_hl_width = (float) eighth_hl_width_;
    // float eighth_hl_height = (float) eighth_hl_height_;
    // Point eighth_hl_center(260, -740);
    // std::vector<Point> eighth_hl_hitbox_relative_hull = {Point(-eighth_hl_width/2,-eighth_hl_height/2), Point(eighth_hl_width/2,-eighth_hl_height/2), Point(eighth_hl_width/2,eighth_hl_height/2), Point(-eighth_hl_width/2,eighth_hl_height/2)};
    // Hitbox eighth_hl_hitbox(eighth_hl_center, eighth_hl_hitbox_relative_hull);
    // Model eighth_hl_model(eighth_hl_center, eighth_hl_texture, eighth_hl_hitbox, eighth_hl_width, eighth_hl_height);
    // std::list<Model> eighth_hl_list;
    // eighth_hl_list.push_back(eighth_hl_model);
    // ModelCycle eighth_hl_model_cycle(eighth_hl_list);
    // std::unordered_map<State, ModelCycle&> eighth_hl_map = {{IDLE, eighth_hl_model_cycle}};
    // ModelCollection eighth_hl_model_collection(eighth_hl_map);
    // Object eighth_hl(renderer, eighth_hl_center, eighth_hl_model_collection, SOLID);
    // object_map.add_to_map(eighth_hl);
    // all_objects.push_back(&eighth_hl);

    ItemManager item_manager;

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
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    std::cout << "Mouse at: " << event.button.x << ", " << event.button.y << std::endl;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    for (int i=0; i<all_objects.size(); i++) {
                        std::vector<Point> current_hull = all_objects[i]->get_current_hitbox().get_current_hull();
                        std::cout << "Object number " << i << " hull:" << std::endl;
                        for (int j=0; j<current_hull.size(); j++) {
                            std::cout << current_hull[j] << ", ";
                        }
                        std::cout << std::endl;
                    }
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  
        testPlayer.update_targeted_point(Point((float) mouse_x, (float) -mouse_y));   
        testPlayer.read_inputs();   
        testPlayer.run_scheduled();      
        for (Object* o : all_objects) {
            (*o).redraw_object();
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
        framerate_cap_thread.join();
        SDL_RenderPresent(renderer);
    }

    // SDL_DestroyTexture(texture);
    // SDL_DestroyTexture(first_hl_texture);
    // SDL_DestroyTexture(second_hl_texture);
    // SDL_DestroyTexture(third_hl_texture);
    // SDL_DestroyTexture(fourth_hl_texture);
    // SDL_DestroyTexture(fifth_hl_texture);
    // SDL_DestroyTexture(sixth_hl_texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}


int main() {
    return runGame();
}
