#ifndef OFFLINE_ENGINE_H
#define OFFLINE_ENGINE_H

#include "player.h"
#include <mutex>
#include <functional>

class OfflineEngine {
    
private:

    enum ObjectTypeSpecifier {
        OBJECT_TYPE,
        MOBILE_OBJECT_TYPE,
        CREATURE_TYPE,
        PLAYER_TYPE
    };

    struct ObjectCreationArgs {
        SDL_Renderer* renderer;
        Point* center;
        ModelCollection* modelCollection;
        ObjectMap* objectMap;
        int health;

        ObjectCreationArgs(SDL_Renderer* renderer, Point* center, ModelCollection* modelCollection, ObjectMap* objectMap, int health) :
            renderer(renderer),
            center(center),
            modelCollection(modelCollection),
            objectMap(objectMap),
            health(health) {}

    };

    struct ObjectEntry {
        Object* object;
        std::list<int> tiedHitboxes;
        bool previouslyDead;

        ObjectEntry(const ObjectCreationArgs& objectCreationArgs, ObjectTypeSpecifier specifier) :
            previouslyDead(false),
            object(
                (specifier == OBJECT_TYPE) ? new Object(
                    objectCreationArgs.renderer,
                    *objectCreationArgs.center,
                    *objectCreationArgs.modelCollection
                ) :
                (specifier == MOBILE_OBJECT_TYPE) ? new MobileObject(
                    objectCreationArgs.renderer,
                    *objectCreationArgs.center,
                    *objectCreationArgs.modelCollection,
                    *objectCreationArgs.objectMap
                ) :
                (specifier == CREATURE_TYPE) ? new Creature(
                    objectCreationArgs.renderer,
                    *objectCreationArgs.center,
                    *objectCreationArgs.modelCollection,
                    *objectCreationArgs.objectMap,
                    objectCreationArgs.health
                ) :
                (specifier == PLAYER_TYPE) ? new Player(
                    objectCreationArgs.renderer,
                    *objectCreationArgs.center,
                    *objectCreationArgs.modelCollection,
                    *objectCreationArgs.objectMap,
                    objectCreationArgs.health
                ) :
                new Object(
                    objectCreationArgs.renderer,
                    *objectCreationArgs.center,
                    *objectCreationArgs.modelCollection
                ))
            {
                tiedHitboxes = object->getAllTiedHitboxes();
            }

        ~ObjectEntry() {
            delete object;
        }    

    };
    
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    std::mutex objectCreationMutex_;

    int hitboxIdCounter_;

    std::list<ObjectEntry> allObjects_;

    std::list<MobileObject*> mobileObjectPtrs_;

    Player* playerPtr_;
    
    ObjectMap objectMap_;
    std::unordered_map<int, Hitbox*> allHitboxes_;

    std::list<SDL_Texture*> textures_;

public:

    OfflineEngine(int windowWidth, int windowHeight);

    SDL_Texture* createTexture(const char* filepath);

    static std::pair<float, float> readTexturesWidthAndHeight(SDL_Texture* texture);

    StaticHitbox* makeStaticHitbox(const std::vector<Point>& hull);
    MobileHitbox* makeMobileHitbox(const std::vector<Point>& hull);

    Object* makeObject(Point& center, ModelCollection& modelCollection);
    MobileObject* makeMobileObject(Point& center, ModelCollection& modelCollection);
    Creature* makeCreature(Point& center, ModelCollection& modelCollection, int health);
    Player* makePlayer(Point& center, ModelCollection& modelCollection, int health);

    void run();

    ~OfflineEngine(); 

};

#endif