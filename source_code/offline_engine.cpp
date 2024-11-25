#include "offline_engine.h"
#include <unordered_set>
#include <SDL2/SDL_image.h>
#include <thread>
#include "constants.h"


OfflineEngine::OfflineEngine(int windowWidth, int windowHeight) : hitboxIdCounter_(0) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed" << std::endl;
        return;
    }

    window_ = SDL_CreateWindow("Noble Duel Engine Showcase", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);

    if (!window_) {
        std::cerr << "SDL_CreateWindow failed" << std::endl;
        return;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);;

    if (!renderer_) {
        std::cerr << "SDL_CreateRendered failed" << std::endl;
        return;
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init failed" << std::endl;
        return;
    } 
}


SDL_Texture* OfflineEngine::createTexture(const char* filepath) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer_, filepath);
    if (!newTexture) {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        return nullptr;
    }
    textures_.push_back(newTexture);
    return newTexture;
}


std::pair<float, float> OfflineEngine::readTexturesWidthAndHeight(SDL_Texture* texture) {
    int iwidth, iheight;
    SDL_QueryTexture(texture, NULL, NULL, &iwidth, &iheight);
    return {iwidth, iheight};
}


StaticHitbox* OfflineEngine::makeStaticHitbox(const std::vector<Point>& hull) {
    StaticHitbox* newStaticHitbox = new StaticHitbox(hitboxIdCounter_, hull);
    allHitboxes_[hitboxIdCounter_] = newStaticHitbox;
    ++hitboxIdCounter_;
    return newStaticHitbox;
}


MobileHitbox* OfflineEngine::makeMobileHitbox(const std::vector<Point>& hull) {
    MobileHitbox* newMobileHitbox = new MobileHitbox(hitboxIdCounter_, hull);
    allHitboxes_[hitboxIdCounter_] = newMobileHitbox;
    ++hitboxIdCounter_;
    return newMobileHitbox;
}


Object* OfflineEngine::makeObject(Point& center, ModelCollection& modelCollection) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    ObjectCreationArgs newObjectCreationArgs(renderer_, &center, &modelCollection, nullptr, 1, 1);
    allObjects_.emplace_back(newObjectCreationArgs, OBJECT_TYPE);
    objectMap_.addNewObject(*allObjects_.back().object);
    return allObjects_.back().object;
}


MobileObject* OfflineEngine::makeMobileObject(Point& center, ModelCollection& modelCollection, float mass) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    ObjectCreationArgs newMobileObjectCreationArgs(renderer_, &center, &modelCollection, &objectMap_, mass, 1);
    allObjects_.emplace_back(newMobileObjectCreationArgs, MOBILE_OBJECT_TYPE);
    mobileObjectPtrs_.push_back(dynamic_cast<MobileObject*>(allObjects_.back().object));
    objectMap_.addNewObject(*allObjects_.back().object);
    return mobileObjectPtrs_.back();
}


Creature* OfflineEngine::makeCreature(Point& center, ModelCollection& modelCollection, float mass, int health) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    ObjectCreationArgs newCreatureCreationArgs(renderer_, &center, &modelCollection, &objectMap_, mass, health);
    allObjects_.emplace_back(newCreatureCreationArgs, CREATURE_TYPE);
    mobileObjectPtrs_.push_back(dynamic_cast<MobileObject*>(allObjects_.back().object));
    objectMap_.addNewObject(*allObjects_.back().object);
    return dynamic_cast<Creature*>(mobileObjectPtrs_.back());
}


Player* OfflineEngine::makePlayer(Point& center, ModelCollection& modelCollection, float mass, int health) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    ObjectCreationArgs newPlayerCreationArgs(renderer_, &center, &modelCollection, &objectMap_, mass, health);
    allObjects_.emplace_back(newPlayerCreationArgs, PLAYER_TYPE);
    playerPtr_ = dynamic_cast<Player*>(allObjects_.back().object);
    objectMap_.addNewObject(*allObjects_.back().object);
    return playerPtr_;
}


void OfflineEngine::run() {
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 255);
    std::thread framerateCapThread;
    int mouseX, mouseY;
    bool running = true;
    SDL_Event event;

    while (running) {
        framerateCapThread = std::thread([](){
            std::this_thread::sleep_for(std::chrono::milliseconds((int) 1000.0/FPS));
        });
        SDL_GetMouseState(&mouseX, &mouseY);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_RenderClear(renderer_);
        
        if (playerPtr_ != nullptr) {
            if (!playerPtr_->isAlive()) {
                playerPtr_ = nullptr;
            } else {
                playerPtr_->updateTargetedPoint(Point((float) mouseX, (float) -mouseY));
                playerPtr_->readInputs(SDL_GetKeyboardState(NULL));
            }
        }

        for (auto it = mobileObjectPtrs_.begin(); it != mobileObjectPtrs_.end(); ) {
            if (!(*it)->isAlive()) {
                objectMap_.removeObject(**it);
                it = mobileObjectPtrs_.erase(it);
            } else {
                (*it)->runScheduled();
                ++it;
            }
        }

        if (playerPtr_ != nullptr) {
            playerPtr_->runScheduled();
        }

        for (auto it = allObjects_.begin(); it != allObjects_.end(); ) {
            if (!(*it).object->isAlive()) {
                if (!(*it).previouslyDead) {
                    (*it).previouslyDead = true;
                    ++it;
                } else {
                    for (int h : (*it).tiedHitboxes) {
                        if (allHitboxes_.find(h) != allHitboxes_.end() && allHitboxes_[h] != nullptr) {
                            delete allHitboxes_[h];
                            allHitboxes_.erase(h);
                        } 
                    }
                    it = allObjects_.erase(it);
                }
            } else {
                (*it).object->redrawObject();
                ++it;
            }
        }

        framerateCapThread.join();
        SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 255);
        SDL_RenderPresent(renderer_);
    }

}


OfflineEngine::~OfflineEngine() {
    for (ObjectEntry& oe : allObjects_) {
        for (int h : oe.tiedHitboxes) {
            if (allHitboxes_.find(h) != allHitboxes_.end() && allHitboxes_[h] != nullptr) {
                delete allHitboxes_[h];
                allHitboxes_.erase(h);
            } 
        }
    }

    for (SDL_Texture* t : textures_) {
        SDL_DestroyTexture(t);
    }

    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
}


