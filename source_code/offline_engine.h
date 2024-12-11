#ifndef OFFLINE_ENGINE_H
#define OFFLINE_ENGINE_H

#include "player.h"
#include "elevator.h"
#include "engine_clock.h"
#include "interactable_manager.h"
#include "button.h"
#include <mutex>
#include <functional>


class OfflineEngine {
    
private:

    struct ObjectEntry {
        Object* object;
        std::list<int> tiedHitboxes;
        bool previouslyDead;

        ObjectEntry(Object* newObject) : object(newObject), previouslyDead(false) {
            tiedHitboxes = object->getAllTiedHitboxes();
        }

        ~ObjectEntry() {
            delete object;
        }    

    };
    
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    EngineClock sessionEngineClock_;

    std::mutex objectCreationMutex_;

    int hitboxIdCounter_;

    std::list<ObjectEntry> allObjects_;

    std::list<MobileObject*> mobileObjectPtrs_;

    Player* playerPtr_;

    Player* player2Ptr_;
    
    ObjectMap objectMap_;
    InteractableManager interactableManager_;
    std::unordered_map<int, Hitbox*> allHitboxes_;

    std::list<SDL_Texture*> textures_;

public:

    OfflineEngine(int windowWidth, int windowHeight);

    SDL_Texture* createTexture(const char* filepath);

    static std::pair<float, float> readTexturesWidthAndHeight(SDL_Texture* texture);

    StaticHitbox* makeStaticHitbox(const std::vector<Point>& hull);
    MobileHitbox* makeMobileHitbox(const std::vector<Point>& hull);

    Object* makeObject(Point& center, ModelCollection& modelCollection);
    MobileObject* makeMobileObject(Point& center, ModelCollection& modelCollection, float mass);
    
    FloatingPlatform* makeFloatingPlatform(Point& center, ModelCollection& modelCollection, float mass, 
                                           const std::vector<Velocity>& movementModesVs, const std::vector<Point>& movementModesBorders);
    Elevator* makeElevator(Point& center, ModelCollection& modelCollection, float mass, 
                                           const std::vector<Velocity>& movementModesVs, const std::vector<Point>& movementModesBorders);

    Button* makeButton(Point& center, ModelCollection& modelCollection, std::function<void()>& buttonCommand);

    Creature* makeCreature(Point& center, ModelCollection& modelCollection, float mass, int health);
    Player* makePlayer(Point& center, ModelCollection& modelCollection, float mass, int health);
    Player* makePlayer2(Point& center, ModelCollection& modelCollection, float mass, int health); // temp

    void run();

    ~OfflineEngine(); 

};

#endif