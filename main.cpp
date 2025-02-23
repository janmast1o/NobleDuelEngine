#include "source_code/offline_engine.h"
#include "source_code/utility_functions.h"
#include "source_code/thrusting_weapon.h"
#include <thread>
#include <string>
#include <random>


void addGrounds(OfflineEngine& engine) {
    std::unordered_map<const char*, std::vector<Point>> collisionMeshesForObjects = {
        {"bridge", {{320, -90}, {286, -55}, {246, -29}, {72, -29}, {39, -48}, {0, -90}}},
        {"even_bottom_ground", {{0, -70}, {380, -70}, {380, -11}, {0, -11}}},
        {"left_facing_beam", {{0, 0}, {0, -10}, {109, -60}, {120, -60}, {120, 0}}},
        {"right_facing_beam", {{0, 0}, {0, -60}, {10, -60}, {120, -10}, {120, 0}}},
        {"left_log", {{0, 0}, {0, -25}, {217, -190}, {250, -190}, {250, -164}, {25, 0}}},
        {"right_log", {{0, -190}, {32, -190}, {250, -26}, {250, 0}, {223, 0}, {0, -166}}},
        {"uneven_bottom_ground", {{0, -32}, {24, -61}, {68, -73}, {258, -76}, {402, -58}, {430, -22}, {430, -8}, {0, -8}}},
        {"uneven_top_ground", {{0, -20}, {0, -60}, {540, -60}, {540, -20}, {463, -8}, {105, -8}}},
        {"uneven_top_ground_no_grass", {{0, -50}, {500, -50}, {500, -12}, {432, 0}, {87, 0}, {0, -11}}},
        {"even_bottom_ground_no_grass", {{0, -120}, {900, -120}, {900, 0}, {0, 0}}},
        {"platform_support", {{0, 0}, {0, -50}, {400, -50}, {400, 0}}},
        {"triangular_right_facing_stone", {{0, -280}, {380, -280}, {380, 0}}},
    };

    const std::string resDir = "resources/new/";

    std::unordered_map<const char*, SDL_Texture*> texturesForObjects = {
        {"bridge", engine.createTexture(resDir + "bridge.png")},
        {"even_bottom_ground", engine.createTexture(resDir + "even_bottom_ground.png")},
        {"even_bottom_ground_no_grass", engine.createTexture(resDir + "even_bottom_ground_no_grass.png")},
        {"left_facing_beam", engine.createTexture(resDir + "left_beam_support.png")},
        {"right_facing_beam", engine.createTexture(resDir + "right_beam_support.png")},
        {"left_log", engine.createTexture(resDir + "left_log.png")},
        {"right_log", engine.createTexture(resDir + "right_log.png")},
        {"uneven_bottom_ground", engine.createTexture(resDir + "uneven_bottom_ground.png")},
        {"uneven_top_ground", engine.createTexture(resDir + "uneven_top_ground.png")},
        {"uneven_top_ground_no_grass", engine.createTexture(resDir + "uneven_top_ground_no_grass.png")},
        {"platform_support", engine.createTexture(resDir + "platform_support.png")},
        {"triangular_right_facing_stone", engine.createTexture(resDir + "triangular_right_facing_stone.png")}
    };

    float width, height;
    std::tie(width, height) = engine.readTexturesWidthAndHeight(texturesForObjects["even_bottom_ground"]);
    Model firstEBGModel(texturesForObjects["even_bottom_ground"], nullptr, {0,0});
    StaticHitbox* firstEBGCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["even_bottom_ground"]);
    ModelCycle firstEBGModelCycle(firstEBGCollisonMesh);
    firstEBGModelCycle.addModelAndResetIterator(firstEBGModel, 1);
    ModelCollection firstEBGModelCollection;
    firstEBGModelCollection.addModelCycleForState(IDLE, firstEBGModelCycle);
    Object* firstEBG = engine.makeObject({20, -2000}, firstEBGModelCollection);

    Model firstEBGNGModel(texturesForObjects["even_bottom_ground_no_grass"], nullptr, {0, 0});
    StaticHitbox* firstEBGNGCollisionMesh = engine.makeStaticHitbox(collisionMeshesForObjects["even_bottom_ground_no_grass"]);
    ModelCycle firstEBGNGModelCycle(firstEBGNGCollisionMesh);
    firstEBGNGModelCycle.addModelAndResetIterator(firstEBGNGModel, 1);
    ModelCollection firstEBGNGModelCollection;
    firstEBGNGModelCollection.addModelCycleForState(IDLE, firstEBGNGModelCycle);
    Object* firstEBGNG = engine.makeObject({1150, -2000}, firstEBGNGModelCollection);

    Model thirdEBGModel(texturesForObjects["even_bottom_ground"], nullptr, {0,0});
    StaticHitbox* thirdEBGCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["even_bottom_ground"]);
    ModelCycle thirdEBGModelCycle(thirdEBGCollisonMesh);
    thirdEBGModelCycle.addModelAndResetIterator(thirdEBGModel, 1);
    ModelCollection thirdEBGModelCollection;
    thirdEBGModelCollection.addModelCycleForState(IDLE, thirdEBGModelCycle);
    Object* thirdEBG = engine.makeObject({-100, -2250}, thirdEBGModelCollection);

    Model firstUBGModel(texturesForObjects["uneven_bottom_ground"], nullptr, {0, 0});
    StaticHitbox* firstUBGCollisionMesh = engine.makeStaticHitbox(collisionMeshesForObjects["uneven_bottom_ground"]);
    ModelCycle firstUBGModelCycle(firstUBGCollisionMesh);
    firstUBGModelCycle.addModelAndResetIterator(firstUBGModel, 1);
    ModelCollection firstUBGModelCollection;
    firstUBGModelCollection.addModelCycleForState(IDLE, firstUBGModelCycle);
    Object* firstUBG = engine.makeObject({530, -1985}, firstUBGModelCollection);

    Model bridgeModel(texturesForObjects["bridge"], nullptr, {0,0});
    StaticHitbox* bridgeCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["bridge"]);
    ModelCycle bridgeModelCycle(bridgeCollisonMesh);
    bridgeModelCycle.addModelAndResetIterator(bridgeModel, 1);
    ModelCollection bridgeModelCollection;
    bridgeModelCollection.addModelCycleForState(IDLE, bridgeModelCycle);
    Object* bridge = engine.makeObject({925, -1915}, bridgeModelCollection);

    Model firstTrRFSModel(texturesForObjects["triangular_right_facing_stone"], nullptr, {0, 0});
    StaticHitbox* firstTrRFSCollsionMesh = engine.makeStaticHitbox(collisionMeshesForObjects["triangular_right_facing_stone"]);
    ModelCycle firstTrRFSModelCycle(firstTrRFSCollsionMesh);
    firstTrRFSModelCycle.addModelAndResetIterator(firstTrRFSModel, 1);
    ModelCollection firstTrRFSModelCollection;
    firstTrRFSModelCollection.addModelCycleForState(IDLE, firstTrRFSModelCycle);
    Object* firstTrRFS = engine.makeObject({1600, -1720}, firstTrRFSModelCollection);

    Model firstUTGNoGrassModel(texturesForObjects["uneven_top_ground_no_grass"], nullptr, {0,0});
    StaticHitbox* firstUTGNoGrassCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["uneven_top_ground_no_grass"]);
    ModelCycle firstUTGNoGrassModelCycle(firstUTGNoGrassCollisonMesh);
    firstUTGNoGrassModelCycle.addModelAndResetIterator(firstUTGNoGrassModel, 1);
    ModelCollection firstUTGNoGrassModelCollection;
    firstUTGNoGrassModelCollection.addModelCycleForState(IDLE, firstUTGNoGrassModelCycle);
    Object* firstUTGNoGrass = engine.makeObject({1250, -1670}, firstUTGNoGrassModelCollection);

    Model secondUTGNoGrassModel(texturesForObjects["uneven_top_ground_no_grass"], nullptr, {0,0});
    StaticHitbox* secondUTGNoGrassCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["uneven_top_ground_no_grass"]);
    ModelCycle secondUTGNoGrassModelCycle(secondUTGNoGrassCollisonMesh);
    secondUTGNoGrassModelCycle.addModelAndResetIterator(secondUTGNoGrassModel, 1);
    ModelCollection secondUTGNoGrassModelCollection;
    secondUTGNoGrassModelCollection.addModelCycleForState(IDLE, secondUTGNoGrassModelCycle);
    Object* secondUTGNoGrass = engine.makeObject({655, -1450}, secondUTGNoGrassModelCollection);

    Model supportBeamsModel(texturesForObjects["platform_support"], nullptr, {0,0});
    StaticHitbox* supportBeamsCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["platform_support"]);
    ModelCycle supportBeamsModelCycle(supportBeamsCollisonMesh);
    supportBeamsModelCycle.addModelAndResetIterator(supportBeamsModel, 1);
    ModelCollection supportBeamsModelCollection;
    supportBeamsModelCollection.addModelCycleForState(IDLE, supportBeamsModelCycle);
    Object* supportBeams = engine.makeObject({700, -1500}, supportBeamsModelCollection);

    Model secondEBGModel(texturesForObjects["even_bottom_ground"], nullptr, {0,0});
    StaticHitbox* secondEBGCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["even_bottom_ground"]);
    ModelCycle secondEBGModelCycle(secondEBGCollisonMesh);
    secondEBGModelCycle.addModelAndResetIterator(secondEBGModel, 1);
    ModelCollection secondEBGModelCollection;
    secondEBGModelCollection.addModelCycleForState(IDLE, secondEBGModelCycle);
    Object* secondEBG = engine.makeObject({120, -1710}, secondEBGModelCollection);

    Model leftLogModel(texturesForObjects["left_log"], nullptr, {0,0});
    StaticHitbox* leftLogCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["left_log"]);
    ModelCycle leftLogModelCycle(leftLogCollisonMesh);
    leftLogModelCycle.addModelAndResetIterator(leftLogModel, 1);
    ModelCollection leftLogModelCollection;
    leftLogModelCollection.addModelCycleForState(IDLE, leftLogModelCycle);
    Object* leftLog = engine.makeObject({-100, -1570}, leftLogModelCollection);

    Model secondUBGModel(texturesForObjects["uneven_bottom_ground"], nullptr, {0,0});
    StaticHitbox* secondUBGCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["uneven_bottom_ground"]);
    ModelCycle secondUBGModelCycle(secondUBGCollisonMesh);
    secondUBGModelCycle.addModelAndResetIterator(secondUBGModel, 1);
    ModelCollection secondUBGModelCollection;
    secondUBGModelCollection.addModelCycleForState(IDLE, secondUBGModelCycle);
    Object* secondUBG = engine.makeObject({-530, -1560}, secondUBGModelCollection);

    Model fourthEBGModel(texturesForObjects["even_bottom_ground"], nullptr, {0,0});
    StaticHitbox* fourthEBGCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["even_bottom_ground"]);
    ModelCycle fourthEBGModelCycle(fourthEBGCollisonMesh);
    fourthEBGModelCycle.addModelAndResetIterator(fourthEBGModel, 1);
    ModelCollection fourthEBGModelCollection;
    fourthEBGModelCollection.addModelCycleForState(IDLE, fourthEBGModelCycle);
    Object* fourthEBG = engine.makeObject({-100, -1390}, fourthEBGModelCollection);

    Model secondEBGNGModel(texturesForObjects["even_bottom_ground_no_grass"], nullptr, {0,0});
    StaticHitbox* secondEBGNGCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["even_bottom_ground_no_grass"]);
    ModelCycle secondEBGNGModelCycle(secondEBGNGCollisonMesh);
    secondEBGNGModelCycle.addModelAndResetIterator(secondEBGNGModel, 1);
    ModelCollection secondEBGNGModelCollection;
    secondEBGNGModelCollection.addModelCycleForState(IDLE, secondEBGNGModelCycle);
    Object* secondEBGNG = engine.makeObject({200, -1220}, secondEBGNGModelCollection);

    Model rightLogModel(texturesForObjects["right_log"], nullptr, {0,0});
    StaticHitbox* rightLogCollisonMesh = engine.makeStaticHitbox(collisionMeshesForObjects["right_log"]);
    ModelCycle rightLogModelCycle(rightLogCollisonMesh);
    rightLogModelCycle.addModelAndResetIterator(rightLogModel, 1);
    ModelCollection rightLogModelCollection;
    rightLogModelCollection.addModelCycleForState(IDLE, rightLogModelCycle);
    Object* rightLog = engine.makeObject({-50, -1245}, rightLogModelCollection);

}


void addFlowers(OfflineEngine& engine) {
    float flowerHeight = 40;
    int numberOfFlowersInCluster = 25;
    std::string resDir = "resources/new/";
    std::vector<SDL_Texture*> flowerTextures = {
        {engine.createTexture(resDir + "blue_bellflowers.png")},
        {engine.createTexture(resDir + "orange_bellis.png")},
        {engine.createTexture(resDir + "pink_bellflowers.png")},
        {engine.createTexture(resDir + "pink_bellis.png")},
        {engine.createTexture(resDir + "pink_rose.png")},
        {engine.createTexture(resDir + "red_rose.png")}
    };

    std::vector<std::pair<std::pair<float, float>, float>> flowerAreas = {
        {{40, 300}, -2015+flowerHeight},
        {{500, 870}, -1995+flowerHeight},
        {{1380, 1600}, -1670+flowerHeight}
    };

    std::random_device rd;
    std::mt19937 gen(rd());

    float start, end, height;
    for (auto& fa : flowerAreas) {
        std::tie(start, end) = fa.first;
        height = fa.second; 
        std::uniform_real_distribution<float> distrubtion(start, end);

        for (int i=0; i<numberOfFlowersInCluster; ++i) {
            Model flowerModel(flowerTextures[rand()%flowerTextures.size()], nullptr, {0, 0});
            StaticHitbox* flowerMesh = engine.makeStaticHitbox({});
            ModelCycle flowerModelCycle(flowerMesh);
            flowerModelCycle.addModelAndResetIterator(flowerModel, 1);
            ModelCollection flowerModelCollection;
            flowerModelCollection.addModelCycleForState(IDLE, flowerModelCycle);
            Object* flower = engine.makeObject({distrubtion(gen), height}, flowerModelCollection, true);
        }

    }

}


void createCrate(OfflineEngine& engine, SDL_Texture* crateTexture, const Point& startingCenterPoint, float mass) {
    float width, height;
    std::tie(width, height) = engine.readTexturesWidthAndHeight(crateTexture);
    MobileHitbox* crateMesh = engine.makeMobileHitbox({{-width/2, -height/2}, {width/2, -height/2}, {width/2, height/2}, {-width/2, height/2}});
    Model crateModel(crateTexture, nullptr);
    ModelCycle crateModelCycle(crateMesh);
    crateModelCycle.addModelAndResetIterator(crateModel, 1);
    ModelCollection crateModelCollection;
    crateModelCollection.addModelCycleForState(IDLE, crateModelCycle);
    MobileObject* newCrate = engine.makeMobileObject(startingCenterPoint, crateModelCollection, mass);
}



void addCrates(OfflineEngine& engine) {
    std::string resDir = "resources/new/";
    std::unordered_map<std::string, SDL_Texture*> crateTextures = {
        {"light_crate", engine.createTexture(resDir + "light_crate.png")},
        {"heavy_crate", engine.createTexture(resDir + "heavy_crate.png")},
        {"very_heavy_crate", engine.createTexture(resDir + "very_heavy_crate.png")}
    };

    createCrate(engine, crateTextures["light_crate"], {40, -1960}, 15);
    createCrate(engine, crateTextures["light_crate"], {-490, -800}, 15);
    createCrate(engine, crateTextures["light_crate"], {-380, -800}, 15);
    createCrate(engine, crateTextures["light_crate"], {250, -700}, 15);
    createCrate(engine, crateTextures["heavy_crate"], {-410, -800}, 40);
    createCrate(engine, crateTextures["heavy_crate"], {-410, -600}, 40);
    createCrate(engine, crateTextures["heavy_crate"], {570, -1960}, 40);
    createCrate(engine, crateTextures["heavy_crate"], {160, -1960}, 40);
    createCrate(engine, crateTextures["very_heavy_crate"], {1500, -1960}, 180);
    createCrate(engine, crateTextures["very_heavy_crate"], {-55, -2200}, 180);

}


void prepareGame(int windowWidth, int windowHeight, OfflineEngine& engine) {
    SDL_Texture* platformTexture = engine.createTexture("resources/new/platform.png");
    MobileHitbox* platformMesh = engine.makeMobileHitbox({{0, -252}, {0, -270}, {180, -270}, {180, -252}});
    Model platformModel(platformTexture, nullptr, {0, 0});
    ModelCycle platformModelCycle(platformMesh);
    platformModelCycle.addModelAndResetIterator(platformModel, 1);
    ModelCollection platformModelCollection;
    platformModelCollection.addModelCycleForState(IDLE, platformModelCycle);
    FloatingPlatform* platform = engine.makeFloatingPlatform({730, -1460}, platformModelCollection, 1000, 
                                                             {{-65, 0}, {65, 0}}, {{700, -1460}, {940, -1460}});

    addGrounds(engine);

    SDL_Texture* playerTexture = engine.createTexture("resources/new/yellow_sprite.png");
    float playerW, playerH;
    std::tie(playerW, playerH) = engine.readTexturesWidthAndHeight(playerTexture);
    MobileHitbox* playerHitbox = engine.makeMobileHitbox({{-playerW/2, -playerH/2}, {playerW/2, -playerH/2}, {playerW/2, playerH/2}, {-playerW/2, playerH/2}});
    Model playerModel(playerTexture, playerHitbox);
    MobileHitbox* playerMesh = engine.makeMobileHitbox({{-playerW/2, -playerH/2}, {playerW/2, -playerH/2}, {playerW/2, playerH/2}, {-playerW/2, playerH/2}});
    ModelCycle playerModelCycle(playerMesh);
    playerModelCycle.addModelAndResetIterator(playerModel, 1);
    ModelCollection playerModelCollection;
    playerModelCollection.addModelCycleForState(IDLE, playerModelCycle);
    Point playerCenter(180, -1930);
    Player* player = engine.makePlayer(playerCenter, playerModelCollection, 50, 100);

    addFlowers(engine);
    addCrates(engine);

}


int main() {
    int windowWidth = 1400;
    int windowHeight = 900;
    Rectangle celledRectangle({-10, -2200}, {2500, 0});
    OfflineEngine engine(windowWidth, windowHeight, {celledRectangle, 4, 4});
    prepareGame(windowWidth, windowHeight, engine);
    engine.setAnchoredOnPlayerCenter();
    engine.run();

    return 0;
}