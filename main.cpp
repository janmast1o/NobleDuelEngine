#include "source_code/offline_engine.h"

void prepareGame(int windowWidth, int windowHeight, OfflineEngine& engine) {
    SDL_Texture* baseGroundTexture = engine.createTexture("resources/base_ground_.png");
    std::pair<float, float> baseGroundWAH = engine.readTexturesWidthAndHeight(baseGroundTexture);
    float baseGroundW = baseGroundWAH.first;
    float baseGroundH = baseGroundWAH.second;
    Model baseGroundModel(baseGroundTexture, nullptr);
    StaticHitbox* baseGroundMesh = engine.makeStaticHitbox({{-baseGroundW/2, -baseGroundH/2}, {baseGroundW/2, -baseGroundH/2}, {baseGroundW/2, baseGroundH/2}, {-baseGroundW/2, baseGroundH/2}});
    ModelCycle baseGroundModelCycle(baseGroundMesh);
    baseGroundModelCycle.addModelAndResetIterator(baseGroundModel, 1);
    ModelCollection baseGroundModelCollection;
    baseGroundModelCollection.addModelCycleForState(IDLE, baseGroundModelCycle);
    Point baseGroundCenter = {windowWidth/2, -(windowHeight+25)};
    Object* baseGround = engine.makeObject(baseGroundCenter, baseGroundModelCollection);
    
    SDL_Texture* playerTexture = engine.createTexture("resources/first_sprite_tbg_2.png");
    std::pair<float, float> playerWAH = engine.readTexturesWidthAndHeight(playerTexture);
    float playerW = playerWAH.first;
    float playerH = playerWAH.second;
    MobileHitbox* playerHitbox = engine.makeMobileHitbox({{-playerW/2, -playerH/2}, {playerW/2, -playerH/2}, {playerW/2, playerH/2}, {-playerW/2, playerH/2}});
    Model playerModel(playerTexture, playerHitbox);
    MobileHitbox* playerMesh = engine.makeMobileHitbox({{-playerW/2, -playerH/2}, {playerW/2, -playerH/2}, {playerW/2, playerH/2}, {-playerW/2, playerH/2}});
    ModelCycle playerModelCycle(playerMesh);
    playerModelCycle.addModelAndResetIterator(playerModel, 1);
    ModelCollection playerModelCollection;
    playerModelCollection.addModelCycleForState(IDLE, playerModelCycle);
    Point playerCenter = {windowWidth/2, 0};
    Player* player = engine.makePlayer(playerCenter, playerModelCollection, 100);

}


int main() {
    int windowWidth = 1400;
    int windowHeight = 900;
    OfflineEngine engine(windowWidth, windowHeight);
    prepareGame(windowWidth, windowHeight, engine);
    engine.run();
    return 0;
}