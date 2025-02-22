#ifndef OFFLINE_ENGINE_H
#define OFFLINE_ENGINE_H

#include "player.h"
#include "elevator.h"
#include "engine_clock.h"
#include "interactable_manager.h"
#include "thrusting_weapon.h"
#include "button.h"
#include "player_ui.h"
#include "firearm.h"
#include "projectile_factory.h"
#include "projectile_manager.h"
#include "utility_functions.h"
#include "structs_and_enums.h"
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
    
    Point windowUpperLeftCorner_;
    Point auxWindowUpperLeftAnchor_;
    Rectangle windowRelativeRectangle_;
    Rectangle windowRectangle_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    bool anchoredOnPlayerCenter_;

    EngineClock sessionEngineClock_;

    std::mutex objectCreationMutex_;

    int hitboxIdCounter_;

    std::list<ObjectEntry> allObjects_;

    std::list<MobileObject*> mobileObjectPtrs_;

    Player* playerPtr_;
    PlayerUi playerUi_;

    Player* player2Ptr_;
    
    ObjectMap objectMap_;
    InteractableManager interactableManager_;
    ProjectileFactory projectileFactory_;
    ProjectileManager projectileManager_;

    std::unordered_map<int, Hitbox*> allHitboxes_;

    std::list<SDL_Texture*> textures_;

protected:

    void updateWindowUpperLeftCorner();

public:

    OfflineEngine(int windowWidth, int windowHeight, GridOrganizerCreationArgs gridCreationArgs);

    void setAnchoredOnPoint(const Point& newLeftUpperAnchor);
    void setAnchoredOnPlayerCenter();

    SDL_Texture* createTexture(const char* filepath);
    SDL_Texture* createTexture(const std::string filepath);

    static std::pair<float, float> readTexturesWidthAndHeight(SDL_Texture* texture);

    StaticHitbox* makeStaticHitbox(const std::vector<Point>& hull);
    MobileHitbox* makeMobileHitbox(const std::vector<Point>& hull);

    int registerNewProjectileArchetype(ModelCollection& newProjectileArchetypeModelCollection, 
                                       MobileHitbox& leftTravellingHitbox, MobileHitbox& rightTravellingHitbox);

    Object* makeObject(const Point& center, ModelCollection& modelCollection, bool decorative = false);
    MobileObject* makeMobileObject(const Point& center, ModelCollection& modelCollection, float mass, bool decorative = false);
    
    FloatingPlatform* makeFloatingPlatform(const Point& center, ModelCollection& modelCollection, float mass, 
                                           const std::vector<Velocity>& movementModesVs, const std::vector<Point>& movementModesBorders, bool decorative = false);
    Elevator* makeElevator(const Point& center, ModelCollection& modelCollection, float mass, 
                                           const std::vector<Velocity>& movementModesVs, const std::vector<Point>& movementModesBorders);

    Button* makeButton(const Point& center, ModelCollection& modelCollection, std::function<void()>& buttonCommand);

    ThrustingWeapon* makeThrustingWeapon(const Point& center, ModelCollection& modelCollection, float mass, 
                                         int damage, int poiseDamage,
                                         unsigned short attackFrames, unsigned short recoveryFrames, 
                                         float attackExtendRange);

    Firearm* makeFirearm(const Point& center, ModelCollection& modelCollection, float mass, int usedAmmoTypeId,
                         std::optional<FirearmFireSpecs> fireSpecs, std::optional<FirearmFireSpecs> alternativeFireSpecs = std::nullopt);

    Creature* makeCreature(const Point& center, ModelCollection& modelCollection, float mass, int health);
    Player* makePlayer(const Point& center, ModelCollection& modelCollection, float mass, int health);
    Player* makePlayer2(const Point& center, ModelCollection& modelCollection, float mass, int health); // temp

    void run();

    ~OfflineEngine(); 

};

#endif