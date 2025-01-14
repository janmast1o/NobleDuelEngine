#include "offline_engine.h"
#include <unordered_set>
#include <SDL2/SDL_image.h>
#include <thread>
#include "constants.h"


OfflineEngine::OfflineEngine(int windowWidth, int windowHeight, GridOrganizerCreationArgs gridCreationArgs) : 
    objectMap_(gridCreationArgs.celledRectangle, gridCreationArgs.numOfRows, gridCreationArgs.numOfCols),
    interactableManager_(gridCreationArgs.celledRectangle, gridCreationArgs.numOfRows, gridCreationArgs.numOfCols),
    hitboxIdCounter_(0), projectileFactory_(projectileManager_), anchoredOnPlayerCenter_(false) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed" << std::endl;
        return;
    }

    windowUpperLeftCorner_ = {0,0};
    windowRelativeRectangle_ = {{0,-windowHeight}, {windowWidth,0}};
    windowRectangle_ = windowRelativeRectangle_;
    auxWindowUpperLeftAnchor_ = {-windowRelativeRectangle_.getWidth()/2, windowRelativeRectangle_.getHeight()/2};
    
    window_ = SDL_CreateWindow("Noble Duel Engine Showcase", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
    SDL_SetWindowResizable(window_, SDL_FALSE);
    SDL_SetWindowFullscreen(window_, 0);

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

    SDL_RenderSetLogicalSize(renderer_, windowWidth, windowHeight);
    playerUi_ = {renderer_, playerPtr_, {10, 10}, 400, 25, 3, {10, 40}, 400, 25, 3};

}


void OfflineEngine::setAnchoredOnPoint(const Point& newUpperLeftAnchor) {
    windowUpperLeftCorner_ = newUpperLeftAnchor;
    windowRectangle_ = windowRelativeRectangle_+windowUpperLeftCorner_;
}


void OfflineEngine::setAnchoredOnPlayerCenter() {
    if (playerPtr_ != nullptr) {
        anchoredOnPlayerCenter_ = true;
    }
}


void OfflineEngine::updateWindowUpperLeftCorner() {
    if (anchoredOnPlayerCenter_ && playerPtr_ != nullptr) {
        windowUpperLeftCorner_ = playerPtr_->getCenter() + auxWindowUpperLeftAnchor_;
        windowRectangle_ = windowRelativeRectangle_+windowUpperLeftCorner_;
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


int OfflineEngine::registerNewProjectileArchetype(ModelCollection& newProjectileArchetypeModelCollection, 
                                                  MobileHitbox& leftTravellingHitbox, MobileHitbox& rightTravellingHitbox) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    Projectile newProjectileArchetype(renderer_, {0,0}, newProjectileArchetypeModelCollection,
                                      leftTravellingHitbox, rightTravellingHitbox,
                                      sessionEngineClock_, objectMap_, 1, {0,0}); 
    return projectileFactory_.registerNewProjectileArchetype(newProjectileArchetype);
}


Object* OfflineEngine::makeObject(Point& center, ModelCollection& modelCollection) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    Object* newObject = new Object(renderer_, center, modelCollection, sessionEngineClock_);
    allObjects_.emplace_back(newObject);
    objectMap_.addNewObject(*newObject);
    return newObject;
}


MobileObject* OfflineEngine::makeMobileObject(Point& center, ModelCollection& modelCollection, float mass) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    MobileObject* newMobileObject = new MobileObject(renderer_, center, modelCollection, sessionEngineClock_, objectMap_, mass);
    allObjects_.emplace_back(newMobileObject);
    mobileObjectPtrs_.push_back(newMobileObject);
    objectMap_.addNewObject(*newMobileObject);
    return newMobileObject;
}


FloatingPlatform* OfflineEngine::makeFloatingPlatform(Point& center, ModelCollection& modelCollection, float mass,
                                                      const std::vector<Velocity>& movementModesVs, const std::vector<Point>& movementModesBorders) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    FloatingPlatform* newFloatingPlatform = new FloatingPlatform(renderer_, center, modelCollection, sessionEngineClock_, objectMap_, mass,
                                                                 movementModesVs, movementModesBorders);
    allObjects_.emplace_back(newFloatingPlatform);
    mobileObjectPtrs_.push_back(newFloatingPlatform);
    objectMap_.addNewObject(*newFloatingPlatform);
    return newFloatingPlatform; 
}


Elevator* OfflineEngine::makeElevator(Point& center, ModelCollection& modelCollection, float mass, 
                                      const std::vector<Velocity>& movementModesVs, const std::vector<Point>& movementModesBorders) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    Elevator* newElevator = new Elevator(renderer_, center, modelCollection, sessionEngineClock_, objectMap_, mass,
                                         movementModesVs, movementModesBorders);
    allObjects_.emplace_back(newElevator);
    mobileObjectPtrs_.push_back(newElevator);
    objectMap_.addNewObject(*newElevator);
    return newElevator;                                    
}


Button* OfflineEngine::makeButton(Point& center, ModelCollection& modelCollection, std::function<void()>& buttonCommand) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    Button* newButton = new Button(renderer_, center, modelCollection, sessionEngineClock_, buttonCommand);
    allObjects_.emplace_back(newButton);
    objectMap_.addNewObject(*newButton);
    interactableManager_.addNewInteractable(*newButton);
    return newButton;
}


ThrustingWeapon* OfflineEngine::makeThrustingWeapon(Point& center, ModelCollection& modelCollection, float mass, 
                                                    int damage, int poiseDamage,
                                                    unsigned short attackFrames, unsigned short recoveryFrames, 
                                                    float attackExtendRange) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    ThrustingWeapon* newThrustingWeapon = new ThrustingWeapon(renderer_, center, modelCollection, sessionEngineClock_, objectMap_,
                                                              mass, damage, poiseDamage, attackFrames, recoveryFrames, attackExtendRange);
    allObjects_.emplace_back(newThrustingWeapon);
    mobileObjectPtrs_.push_back(newThrustingWeapon);
    objectMap_.addNewObject(*newThrustingWeapon);
    interactableManager_.addNewInteractable(*newThrustingWeapon);
    return newThrustingWeapon;
}


Firearm* OfflineEngine::makeFirearm(Point& center, ModelCollection& modelCollection, float mass, int usedAmmoTypeId,
                                    std::optional<FirearmFireSpecs> fireSpecs, std::optional<FirearmFireSpecs> alternativeFireSpecs) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    Firearm* newFirearm = new Firearm(renderer_, center, modelCollection, sessionEngineClock_, objectMap_, mass,
                                      projectileFactory_, usedAmmoTypeId, fireSpecs, alternativeFireSpecs);
    allObjects_.emplace_back(newFirearm);
    mobileObjectPtrs_.push_back(newFirearm);
    objectMap_.addNewObject(*newFirearm);
    interactableManager_.addNewInteractable(*newFirearm);
    return newFirearm;                                  
}


Creature* OfflineEngine::makeCreature(Point& center, ModelCollection& modelCollection, float mass, int health) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    Creature* newCreature = new Creature(renderer_, center, modelCollection, sessionEngineClock_, objectMap_, mass, health, interactableManager_);
    allObjects_.emplace_back(newCreature);
    mobileObjectPtrs_.push_back(newCreature);
    return newCreature;
}


Player* OfflineEngine::makePlayer(Point& center, ModelCollection& modelCollection, float mass, int health) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    Player* newPlayer = new Player(renderer_, center, modelCollection, sessionEngineClock_, objectMap_, mass, health, interactableManager_);
    allObjects_.emplace_back(newPlayer);
    playerPtr_ = newPlayer;
    objectMap_.addNewObject(*newPlayer);
    playerUi_.setPlayerPtr(playerPtr_);
    return newPlayer;
}


Player* OfflineEngine::makePlayer2(Point& center, ModelCollection& modelCollection, float mass, int health) {
    std::lock_guard<std::mutex> lock(objectCreationMutex_);
    Player* newPlayer2 = new Player(renderer_, center, modelCollection, sessionEngineClock_, objectMap_, mass, health, interactableManager_);
    allObjects_.emplace_back(newPlayer2);
    player2Ptr_ = newPlayer2;
    objectMap_.addNewObject(*newPlayer2);
    return newPlayer2;
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

        auto start = std::chrono::high_resolution_clock::now();

        Uint32 mouseButtonEvent = SDL_GetMouseState(&mouseX, &mouseY);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_RenderClear(renderer_);

        if (playerPtr_ != nullptr) {
            if (!playerPtr_->isAlive()) {
                playerPtr_ = nullptr;
                playerUi_.setPlayerPtr(nullptr);
            } else {
                playerPtr_->updateTargetedPoint(Point(((float) mouseX)+windowUpperLeftCorner_.x, ((float) -mouseY)+windowUpperLeftCorner_.y));
                playerPtr_->readInputs(SDL_GetKeyboardState(NULL), mouseButtonEvent & SDL_BUTTON(SDL_BUTTON_LEFT), mouseButtonEvent & SDL_BUTTON(SDL_BUTTON_RIGHT));
            }
        }

        if (player2Ptr_ != nullptr) {
            if (!player2Ptr_->isAlive()) {
                player2Ptr_ = nullptr;
            } else {
                player2Ptr_->updateTargetedPoint(Point((float) mouseX, (float) -mouseY));
                player2Ptr_->readInputs(SDL_GetKeyboardState(NULL), mouseButtonEvent & SDL_BUTTON(SDL_BUTTON_LEFT), false);
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

        projectileManager_.simulateProjectiles();

        if (playerPtr_ != nullptr) {
            playerPtr_->runScheduled();
        }

        if (player2Ptr_ != nullptr) {
            player2Ptr_->runScheduled();
        }

        updateWindowUpperLeftCorner();
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
                    objectMap_.removeObject(*(*it).object);
                    it = allObjects_.erase(it);
                }
            } else {
                // (*it).object->redrawObject();
                (*it).object->redrawObject(windowRectangle_);              
                ++it;
            }
        }

        for (MobileObject* mo : mobileObjectPtrs_) {
            if (mo->isAlive()) mo->updatePreviousFrameCenter();
        }

        projectileManager_.redrawProjectiles(windowRectangle_);
        playerUi_.redrawBars();

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
        // std::cout << elapsed.count() << "ms" << std::endl;

        framerateCapThread.join();
        SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 255);
        SDL_RenderPresent(renderer_);
        sessionEngineClock_.incrementEngineClock();
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


