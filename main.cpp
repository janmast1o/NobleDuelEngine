#include "source_code/offline_engine.h"
#include "source_code/utility_functions.h"
#include <thread>

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

    SDL_Texture* upperCentralFloatingTexture = engine.createTexture("resources/short_and_long_ground_.png");
    std::pair<float, float> upperCentralFloatingWAH = engine.readTexturesWidthAndHeight(upperCentralFloatingTexture);
    float upperCentralFloatingW = upperCentralFloatingWAH.first;
    float upperCentralFloatingH = upperCentralFloatingWAH.second;
    Model upperCentralFloatingModel(upperCentralFloatingTexture, nullptr);
    StaticHitbox* upperCentralFloatingMesh = engine.makeStaticHitbox({{-upperCentralFloatingW/2, -upperCentralFloatingH/2}, {upperCentralFloatingW/2, -upperCentralFloatingH/2}, {upperCentralFloatingW/2, upperCentralFloatingH/2}, {-upperCentralFloatingW/2, upperCentralFloatingH/2}});
    ModelCycle upperCentralFloatingModelCycle(upperCentralFloatingMesh);
    upperCentralFloatingModelCycle.addModelAndResetIterator(upperCentralFloatingModel, 1);
    ModelCollection upperCentralFloatingModelCollection;
    upperCentralFloatingModelCollection.addModelCycleForState(IDLE, upperCentralFloatingModelCycle);
    Point upperCentralFloatingCenter = {windowWidth/2, -345};
    Object* upperCentralFloating = engine.makeObject(upperCentralFloatingCenter, upperCentralFloatingModelCollection);

    SDL_Texture* leftMediumFloatingTexture = engine.createTexture("resources/short_and_short_ground_.png");
    std::pair<float, float> leftMediumFloatingWAH = engine.readTexturesWidthAndHeight(leftMediumFloatingTexture);
    float leftMediumFloatingW = leftMediumFloatingWAH.first;
    float leftMediumFloatingH = leftMediumFloatingWAH.second;
    Model leftMediumFloatingModel(leftMediumFloatingTexture, nullptr);
    StaticHitbox* leftMediumFloatingMesh = engine.makeStaticHitbox({{-leftMediumFloatingW/2, -leftMediumFloatingH/2}, {leftMediumFloatingW/2, -leftMediumFloatingH/2}, {leftMediumFloatingW/2, leftMediumFloatingH/2}, {-leftMediumFloatingW/2, leftMediumFloatingH/2}});
    ModelCycle leftMediumFloatingModelCycle(leftMediumFloatingMesh);
    leftMediumFloatingModelCycle.addModelAndResetIterator(leftMediumFloatingModel, 1);
    ModelCollection leftMediumFloatingModelCollection;
    leftMediumFloatingModelCollection.addModelCycleForState(IDLE, leftMediumFloatingModelCycle);
    Point leftMediumFloatingCenter = {300, -500};
    Object* leftMediumFloating = engine.makeObject(leftMediumFloatingCenter, leftMediumFloatingModelCollection);

    SDL_Texture* rightMediumFloatingTexture = engine.createTexture("resources/short_and_short_ground_.png");
    std::pair<float, float> rightMediumFloatingWAH = engine.readTexturesWidthAndHeight(rightMediumFloatingTexture);
    float rightMediumFloatingW = rightMediumFloatingWAH.first;
    float rightMediumFloatingH = rightMediumFloatingWAH.second;
    Model rightMediumFloatingModel(rightMediumFloatingTexture, nullptr);
    StaticHitbox* rightMediumFloatingMesh = engine.makeStaticHitbox({{-rightMediumFloatingW/2, -rightMediumFloatingH/2}, {rightMediumFloatingW/2, -rightMediumFloatingH/2}, {rightMediumFloatingW/2, rightMediumFloatingH/2}, {-rightMediumFloatingW/2, rightMediumFloatingH/2}});
    ModelCycle rightMediumFloatingModelCycle(rightMediumFloatingMesh);
    rightMediumFloatingModelCycle.addModelAndResetIterator(rightMediumFloatingModel, 1);
    ModelCollection rightMediumFloatingModelCollection;
    rightMediumFloatingModelCollection.addModelCycleForState(IDLE, rightMediumFloatingModelCycle);
    Point rightMediumFloatingCenter = {1100, -500};
    Object* rightMediumFloating = engine.makeObject(rightMediumFloatingCenter, rightMediumFloatingModelCollection);

    SDL_Texture* leftSmallFloatingTexture = engine.createTexture("resources/short_and_very_short_ground_.png");
    std::pair<float, float> leftSmallFloatingWAH = engine.readTexturesWidthAndHeight(leftSmallFloatingTexture);
    float leftSmallFloatingW = leftSmallFloatingWAH.first;
    float leftSmallFloatingH = leftSmallFloatingWAH.second;
    Model leftSmallFloatingModel(leftSmallFloatingTexture, nullptr);
    StaticHitbox* leftSmallFloatingMesh = engine.makeStaticHitbox({{-leftSmallFloatingW/2, -leftSmallFloatingH/2}, {leftSmallFloatingW/2, -leftSmallFloatingH/2}, {leftSmallFloatingW/2, leftSmallFloatingH/2}, {-leftSmallFloatingW/2, leftSmallFloatingH/2}});
    ModelCycle leftSmallFloatingModelCycle(leftSmallFloatingMesh);
    leftSmallFloatingModelCycle.addModelAndResetIterator(leftSmallFloatingModel, 1);
    ModelCollection leftSmallFloatingModelCollection;
    leftSmallFloatingModelCollection.addModelCycleForState(IDLE, leftSmallFloatingModelCycle);
    Point leftSmallFloatingCenter = {275, -300};
    Object* leftSmallFloating = engine.makeObject(leftSmallFloatingCenter, leftSmallFloatingModelCollection);

    SDL_Texture* rightSmallFloatingTexture = engine.createTexture("resources/short_and_very_short_ground_.png");
    std::pair<float, float> rightSmallFloatingWAH = engine.readTexturesWidthAndHeight(rightSmallFloatingTexture);
    float rightSmallFloatingW = rightSmallFloatingWAH.first;
    float rightSmallFloatingH = rightSmallFloatingWAH.second;
    Model rightSmallFloatingModel(rightSmallFloatingTexture, nullptr);
    StaticHitbox* rightSmallFloatingMesh = engine.makeStaticHitbox({{-rightSmallFloatingW/2, -rightSmallFloatingH/2}, {rightSmallFloatingW/2, -rightSmallFloatingH/2}, {rightSmallFloatingW/2, rightSmallFloatingH/2}, {-rightSmallFloatingW/2, rightSmallFloatingH/2}});
    ModelCycle rightSmallFloatingModelCycle(rightSmallFloatingMesh);
    rightSmallFloatingModelCycle.addModelAndResetIterator(rightSmallFloatingModel, 1);
    ModelCollection rightSmallFloatingModelCollection;
    rightSmallFloatingModelCollection.addModelCycleForState(IDLE, rightSmallFloatingModelCycle);
    Point rightSmallFloatingCenter = {1125, -300};
    Object* rightSmallFloating = engine.makeObject(rightSmallFloatingCenter, rightSmallFloatingModelCollection);

    SDL_Texture* leftBranchTexture = engine.createTexture("resources/left_tree_branch_.png");
    std::pair<float, float> leftBranchWAH = engine.readTexturesWidthAndHeight(leftBranchTexture);
    float leftBranchW = leftBranchWAH.first;
    float leftBranchH = leftBranchWAH.second;
    Model leftBranchModel(leftBranchTexture, nullptr, {0,0});
    StaticHitbox* leftBranchMesh = engine.makeStaticHitbox({Point(0,-17), Point(0,-43), Point(143,-46), Point(192,-20), Point(199,-7), Point(77,0)});
    ModelCycle leftBranchModelCycle(leftBranchMesh);
    leftBranchModelCycle.addModelAndResetIterator(leftBranchModel, 1);
    ModelCollection leftBranchModelCollection;
    leftBranchModelCollection.addModelCycleForState(IDLE, leftBranchModelCycle);
    Point leftBranchCenter = {0, -160};
    Object* leftBranch = engine.makeObject(leftBranchCenter, leftBranchModelCollection);

    SDL_Texture* rightBranchTexture = engine.createTexture("resources/right_tree_branch_.png");
    std::pair<float, float> rightBranchWAH = engine.readTexturesWidthAndHeight(rightBranchTexture);
    float rightBranchW = rightBranchWAH.first;
    float rightBranchH = rightBranchWAH.second;
    Model rightBranchModel(rightBranchTexture, nullptr, {0,0});
    StaticHitbox* rightBranchMesh = engine.makeStaticHitbox({Point(122,0), Point(0,-7), Point(7,-20), Point(56,-46), Point(199,-43), Point(199,-17)});
    ModelCycle rightBranchModelCycle(rightBranchMesh);
    rightBranchModelCycle.addModelAndResetIterator(rightBranchModel, 1);
    ModelCollection rightBranchModelCollection;
    rightBranchModelCollection.addModelCycleForState(IDLE, rightBranchModelCycle);
    Point rightBranchCenter = {1201, -160};
    Object* rightBranch = engine.makeObject(rightBranchCenter, rightBranchModelCollection);

    SDL_Texture* leftTrStoneTexture = engine.createTexture("resources/triangular_left_facing_stone_.png");
    std::pair<float, float> leftTrStoneWAH = engine.readTexturesWidthAndHeight(leftTrStoneTexture);
    float leftTrStoneW = leftTrStoneWAH.first;
    float leftTrStoneH = leftTrStoneWAH.second;
    Model leftTrStoneModel(leftTrStoneTexture, nullptr);
    StaticHitbox* leftTrStoneMesh = engine.makeStaticHitbox({{-leftTrStoneW/2, -leftTrStoneH/2}, {leftTrStoneW/2, -leftTrStoneH/2}, {-leftTrStoneW/2, leftTrStoneH/2}});
    ModelCycle leftTrStoneModelCycle(leftTrStoneMesh);
    leftTrStoneModelCycle.addModelAndResetIterator(leftTrStoneModel, 1);
    ModelCollection leftTrStoneModelCollection;
    leftTrStoneModelCollection.addModelCycleForState(IDLE, leftTrStoneModelCycle);
    Point leftTrStoneCenter = {200, -735};
    Object* leftTrStone = engine.makeObject(leftTrStoneCenter, leftTrStoneModelCollection);

    SDL_Texture* rightTrStoneTexture = engine.createTexture("resources/triangular_right_facing_stone_.png");
    std::pair<float, float> rightTrStoneWAH = engine.readTexturesWidthAndHeight(rightTrStoneTexture);
    float rightTrStoneW = rightTrStoneWAH.first;
    float rightTrStoneH = rightTrStoneWAH.second;
    Model rightTrStoneModel(rightTrStoneTexture, nullptr);
    StaticHitbox* rightTrStoneMesh = engine.makeStaticHitbox({{-rightTrStoneW/2, -rightTrStoneH/2}, {rightTrStoneW/2, -rightTrStoneH/2}, {rightTrStoneW/2, rightTrStoneH/2}});
    ModelCycle rightTrStoneModelCycle(rightTrStoneMesh);
    rightTrStoneModelCycle.addModelAndResetIterator(rightTrStoneModel, 1);
    ModelCollection rightTrStoneModelCollection;
    rightTrStoneModelCollection.addModelCycleForState(IDLE, rightTrStoneModelCycle);
    Point rightTrStoneCenter = {1200, -735};
    Object* rightTrStone = engine.makeObject(rightTrStoneCenter, rightTrStoneModelCollection);

    SDL_Texture* leftStoneTexture = engine.createTexture("resources/right_facing_stone_.png");
    std::pair<float, float> leftStoneWAH = engine.readTexturesWidthAndHeight(leftStoneTexture);
    float leftStoneW = leftStoneWAH.first;
    float leftStoneH = leftStoneWAH.second;
    Model leftStoneModel(leftStoneTexture, nullptr, {0,0});
    StaticHitbox* leftStoneMesh = engine.makeStaticHitbox({Point(250,0), Point(163,-25), Point(95,-64), Point(0,-200), Point(250,-200)});
    ModelCycle leftStoneModelCycle(leftStoneMesh);
    leftStoneModelCycle.addModelAndResetIterator(leftStoneModel, 1);
    ModelCollection leftStoneModelCollection;
    leftStoneModelCollection.addModelCycleForState(IDLE, leftStoneModelCycle);
    Point leftStoneCenter = {(windowWidth/2)-250, -700};
    Object* leftStone = engine.makeObject(leftStoneCenter, leftStoneModelCollection);

    SDL_Texture* rightStoneTexture = engine.createTexture("resources/left_facing_stone_.png");
    std::pair<float, float> rightStoneWAH = engine.readTexturesWidthAndHeight(rightStoneTexture);
    float rightStoneW = rightStoneWAH.first;
    float rightStoneH = rightStoneWAH.second;
    Model rightStoneModel(rightStoneTexture, nullptr, {0,0});
    StaticHitbox* rightStoneMesh = engine.makeStaticHitbox({Point(0,-200), Point(250,-200), Point(155,-64), Point(87,-25), Point(0,0)});
    ModelCycle rightStoneModelCycle(rightStoneMesh);
    rightStoneModelCycle.addModelAndResetIterator(rightStoneModel, 1);
    ModelCollection rightStoneModelCollection;
    rightStoneModelCollection.addModelCycleForState(IDLE, rightStoneModelCycle);
    Point rightStoneCenter = {(windowWidth/2), -700};
    Object* rightStone = engine.makeObject(rightStoneCenter, rightStoneModelCollection);

    SDL_Texture* lowerCentralFloatingTexture = engine.createTexture("resources/short_and_slightly_longer_ground_.png");
    std::pair<float, float> lowerCentralFloatingWAH = engine.readTexturesWidthAndHeight(lowerCentralFloatingTexture);
    float lowerCentralFloatingW = lowerCentralFloatingWAH.first;
    float lowerCentralFloatingH = lowerCentralFloatingWAH.second;
    Model lowerCentralFloatingModel(lowerCentralFloatingTexture, nullptr);
    StaticHitbox* lowerCentralFloatingMesh = engine.makeStaticHitbox({{-lowerCentralFloatingW/2, -lowerCentralFloatingH/2}, {lowerCentralFloatingW/2, -lowerCentralFloatingH/2}, {lowerCentralFloatingW/2, lowerCentralFloatingH/2}, {-lowerCentralFloatingW/2, lowerCentralFloatingH/2}});
    ModelCycle lowerCentralFloatingModelCycle(lowerCentralFloatingMesh);
    lowerCentralFloatingModelCycle.addModelAndResetIterator(lowerCentralFloatingModel, 1);
    ModelCollection lowerCentralFloatingModelCollection;
    lowerCentralFloatingModelCollection.addModelCycleForState(IDLE, lowerCentralFloatingModelCycle);
    Point lowerCentralFloatingCenter = {windowWidth/2, -575};
    Object* lowerCentralFloating = engine.makeObject(lowerCentralFloatingCenter, lowerCentralFloatingModelCollection);

    SDL_Texture* obstacleTexture = engine.createTexture("resources/vertical_short_block.png");
    std::pair<float, float> obstacleWAH = engine.readTexturesWidthAndHeight(obstacleTexture);
    float obstacleW = obstacleWAH.first;
    float obstacleH = obstacleWAH.second;
    Model obstacleModel(obstacleTexture, nullptr);
    StaticHitbox* obstacleMesh = engine.makeStaticHitbox({{-obstacleW/2, -obstacleH/2}, {obstacleW/2, -obstacleH/2}, {obstacleW/2, obstacleH/2}, {-obstacleW/2, obstacleH/2}});
    ModelCycle obstacleModelCycle(obstacleMesh);
    obstacleModelCycle.addModelAndResetIterator(obstacleModel, 1);
    ModelCollection obstacleModelCollection;
    obstacleModelCollection.addModelCycleForState(IDLE, obstacleModelCycle);
    Point obstacleCenter = {1120, -285};
    Object* obstacle = engine.makeObject(obstacleCenter, obstacleModelCollection);

    SDL_Texture* obstacle1Texture = engine.createTexture("resources/vertical_short_block.png");
    std::pair<float, float> obstacle1WAH = engine.readTexturesWidthAndHeight(obstacle1Texture);
    float obstacle1W = obstacle1WAH.first;
    float obstacle1H = obstacle1WAH.second;
    Model obstacle1Model(obstacle1Texture, nullptr);
    StaticHitbox* obstacle1Mesh = engine.makeStaticHitbox({{-obstacle1W/2, -obstacle1H/2}, {obstacle1W/2, -obstacle1H/2}, {obstacle1W/2, obstacle1H/2}, {-obstacleW/2, obstacleH/2}});
    ModelCycle obstacle1ModelCycle(obstacle1Mesh);
    obstacle1ModelCycle.addModelAndResetIterator(obstacle1Model, 1);
    ModelCollection obstacle1ModelCollection;
    obstacle1ModelCollection.addModelCycleForState(IDLE, obstacle1ModelCycle);
    Point obstacle1Center = {270, -285};
    Object* obstacle1 = engine.makeObject(obstacle1Center, obstacle1ModelCollection);

    SDL_Texture* simpleLightCreateTexture = engine.createTexture("resources/simple_crate.png");
    std::pair<float, float> simpleLightCreateWAH = engine.readTexturesWidthAndHeight(simpleLightCreateTexture);
    float simpleLightCreateW = simpleLightCreateWAH.first;
    float simpleLightCreateH = simpleLightCreateWAH.second;
    MobileHitbox* simpleLightCreateHitbox = engine.makeMobileHitbox({{-simpleLightCreateW/2, -simpleLightCreateH/2}, {simpleLightCreateW/2, -simpleLightCreateH/2}, {simpleLightCreateW/2, simpleLightCreateH/2}, {-simpleLightCreateW/2, simpleLightCreateH/2}});
    Model simpleLightCreateModel(simpleLightCreateTexture, simpleLightCreateHitbox);
    MobileHitbox* simpleLightCreateMesh = engine.makeMobileHitbox({{-simpleLightCreateW/2, -simpleLightCreateH/2}, {simpleLightCreateW/2, -simpleLightCreateH/2}, {simpleLightCreateW/2, simpleLightCreateH/2}, {-simpleLightCreateW/2, simpleLightCreateH/2}});
    ModelCycle simpleLightCreateModelCycle(simpleLightCreateMesh);
    simpleLightCreateModelCycle.addModelAndResetIterator(simpleLightCreateModel, 1);
    ModelCollection simpleLightCreateModelCollection;
    simpleLightCreateModelCollection.addModelCycleForState(IDLE, simpleLightCreateModelCycle);
    Point simpleLightCreateCenter = {650, 0};
    MobileObject* simpleLightCreate = engine.makeMobileObject(simpleLightCreateCenter, simpleLightCreateModelCollection, 25);
    
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
    Point playerCenter = {windowWidth/2+50, 0};
    Player* player = engine.makePlayer(playerCenter, playerModelCollection, 50, 100);

    SDL_Texture* player2Texture = engine.createTexture("resources/first_sprite_tbg_2.png");
    std::pair<float, float> player2WAH = engine.readTexturesWidthAndHeight(player2Texture);
    float player2W = player2WAH.first;
    float player2H = player2WAH.second;
    MobileHitbox* player2Hitbox = engine.makeMobileHitbox({{-player2W/2, -player2H/2}, {player2W/2, -player2H/2}, {player2W/2, player2H/2}, {-player2W/2, player2H/2}});
    Model player2Model(player2Texture, player2Hitbox);
    MobileHitbox* player2Mesh = engine.makeMobileHitbox({{-player2W/2, -player2H/2}, {player2W/2, -player2H/2}, {player2W/2, player2H/2}, {-player2W/2, player2H/2}});
    ModelCycle player2ModelCycle(player2Mesh);
    player2ModelCycle.addModelAndResetIterator(player2Model, 1);
    ModelCollection player2ModelCollection;
    player2ModelCollection.addModelCycleForState(IDLE, player2ModelCycle);
    Point player2Center = {windowWidth/2+100, 0};
    Player* player2 = engine.makePlayer2(player2Center, player2ModelCollection, 250, 100);
    PlayerSpecificKeyMapping& player2KeyMapping = player2->getKeyMappingRef();
    player2KeyMapping.moveLeftMapped = SDL_SCANCODE_LEFT;
    player2KeyMapping.moveRightMapped = SDL_SCANCODE_RIGHT;
    player2KeyMapping.jumpMapped = SDL_SCANCODE_UP;
    player2KeyMapping.sprintModifierMapped = SDL_SCANCODE_RCTRL;
    player2KeyMapping.slowWalkModifierMapped = SDL_SCANCODE_RSHIFT;
    player2->setSprintMaxHorizontalV(500);
    player2->setTrueMaxHorizontalV(500);
}


int main() {
    int windowWidth = 1400;
    int windowHeight = 900;
    OfflineEngine engine(windowWidth, windowHeight);
    prepareGame(windowWidth, windowHeight, engine);
    engine.run();
    // std::vector<Point> S1 = {Point(21,1), Point(20,3), Point(18,5), Point(14,6), Point(7,5), Point(4,4), Point(2,2)};
    // std::vector<Point> R1 = {Point(21, 10), Point(20,9), Point(19,8), Point(12,7), Point(7,7), Point(4,8), Point(3,10)};
    // std::vector<Point> P1 = {{5,11}, {6,9}, {7,7}, {10,7}, {15,9}, {23,14}};

    // std::cout << minVertDistance(S1, R1) << std::endl;
    // std::cout << minVertDistance(S1, P1) << std::endl;
    // std::cout << minVertDistance(R1, S1) << std::endl;
    // std::cout << minVertDistance(P1, S1) << std::endl;
    // std::cout << minVertDistance(P1, R1) << std::endl;

    // std::vector<Point> S = {{593.98, -332.5}, {642.98, -332.5}, {642.98, -283.5}, {593.98, -283.5}};
    // std::vector<Point> R = {{631, -332.5}, {669, -332.5}, {669, -292.5}, {631, -292.5}};
    // std::cout << gjk(S, R) << std::endl;

    return 0;
}