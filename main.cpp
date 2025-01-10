#include "source_code/offline_engine.h"
#include "source_code/utility_functions.h"
#include "source_code/thrusting_weapon.h"
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

    SDL_Texture* blueFloatingPlatformTexture = engine.createTexture("resources/horizontal_short_block.png");
    std::pair<float, float> blueFloatingPlatformWAH = engine.readTexturesWidthAndHeight(blueFloatingPlatformTexture);
    float blueFloatingPlatformW = blueFloatingPlatformWAH.first;
    float blueFloatingPlatformH = blueFloatingPlatformWAH.second;
    Model blueFloatingPlatformModel(blueFloatingPlatformTexture, nullptr);
    MobileHitbox* blueFloatingPlatformCollisionMesh = engine.makeMobileHitbox({{-blueFloatingPlatformW/2, -blueFloatingPlatformH/2}, {blueFloatingPlatformW/2, -blueFloatingPlatformH/2}, {blueFloatingPlatformW/2, blueFloatingPlatformH/2}, {-blueFloatingPlatformW/2, blueFloatingPlatformH/2}});
    ModelCycle blueFloatingPlatformModelCycle(blueFloatingPlatformCollisionMesh);
    blueFloatingPlatformModelCycle.addModelAndResetIterator(blueFloatingPlatformModel, 1);
    ModelCollection blueFloatingPlatformModelCollection;
    blueFloatingPlatformModelCollection.addModelCycleForState(IDLE, blueFloatingPlatformModelCycle);
    Point blueFloatingPlatformCenter = {700, -100};
    const std::vector<Velocity> blueFloatingPlatformVs = {{50, 0}, {0, 20}, {-50, 0}, {0, -20}};
    const std::vector<Point> blueFloatingPlatformBorders = {{900, -200}, {900, -50}, {500, -50}, {500, -200}};
    FloatingPlatform* blueFloatingPlatform = engine.makeFloatingPlatform(blueFloatingPlatformCenter, blueFloatingPlatformModelCollection, 500,
                                                                         blueFloatingPlatformVs, blueFloatingPlatformBorders);
    blueFloatingPlatform->setStartToSlowDownDistance(30);
    blueFloatingPlatform->setSlowDownPerSecondRate(10);  

    SDL_Texture* blueElevatorTexture = engine.createTexture("resources/horizontal_short_block.png"); 
    std::pair<float, float> blueElevatorWAH = engine.readTexturesWidthAndHeight(blueElevatorTexture);
    float blueElevatorW = blueElevatorWAH.first;
    float blueElevatorH = blueElevatorWAH.second;
    Model blueElevatorModel(blueElevatorTexture, nullptr);
    MobileHitbox* blueElevatorCollisionMesh = engine.makeMobileHitbox({{-blueElevatorW/2, -blueElevatorH/2}, {blueElevatorW/2, -blueElevatorH/2}, {blueElevatorW/2, blueElevatorH/2}, {-blueElevatorW/2, blueElevatorH/2}});
    ModelCycle blueElevatorModelCycle(blueElevatorCollisionMesh);
    blueElevatorModelCycle.addModelAndResetIterator(blueElevatorModel, 1);
    ModelCollection blueElevatorModelCollection;
    blueElevatorModelCollection.addModelCycleForState(IDLE, blueElevatorModelCycle);
    Point blueElevatorCenter = {700, -500};
    const std::vector<Velocity> blueElevatorVs = {{50, 0}, {-50, 0}};
    const std::vector<Point> blueElevatorBorders = {{900, -500}, {300, -500}};
    Elevator* blueElevator = engine.makeElevator(blueElevatorCenter, blueElevatorModelCollection, 500,
                                                                        blueElevatorVs, blueElevatorBorders);
    blueElevator->setStartToSlowDownDistance(20);
    blueElevator->setSlowDownPerSecondRate(10);

    SDL_Texture* leverTexture = engine.createTexture("resources/lever.png");
    std::pair<float, float> leverWAH = engine.readTexturesWidthAndHeight(leverTexture);
    float leverW = leverWAH.first;
    float leverH = leverWAH.second;
    Model leverModel(leverTexture, nullptr);
    StaticHitbox* leverMesh = engine.makeStaticHitbox({{-leverW/2, -leverH/2}, {leverW/2, -leverH/2}, {leverW/2, leverH/2}, {-leverW/2, leverH/2}});
    ModelCycle leverModelCycle(leverMesh);
    leverModelCycle.addModelAndResetIterator(leverModel, 1);
    ModelCollection leverModelCollection;
    leverModelCollection.addModelCycleForState(IDLE, leverModelCycle);
    Point leverCenter = {800, -555};
    auto leverCommand = std::function<void()>([blueElevator](void){blueElevator->requestNewElevatorMovement(0);});
    // auto leverCommand = std::function<void()>([](void){;});
    Object* lever = engine.makeButton(leverCenter, leverModelCollection, leverCommand);                             

    SDL_Texture* simpleLightCrateTexture = engine.createTexture("resources/simple_crate.png");
    std::pair<float, float> simpleLightCrateWAH = engine.readTexturesWidthAndHeight(simpleLightCrateTexture);
    float simpleLightCrateW = simpleLightCrateWAH.first;
    float simpleLightCrateH = simpleLightCrateWAH.second;
    MobileHitbox* simpleLightCrateHitbox = engine.makeMobileHitbox({{-simpleLightCrateW/2, -simpleLightCrateH/2}, {simpleLightCrateW/2, -simpleLightCrateH/2}, {simpleLightCrateW/2, simpleLightCrateH/2}, {-simpleLightCrateW/2, simpleLightCrateH/2}});
    Model simpleLightCrateModel(simpleLightCrateTexture, simpleLightCrateHitbox);
    MobileHitbox* simpleLightCrateMesh = engine.makeMobileHitbox({{-simpleLightCrateW/2, -simpleLightCrateH/2}, {simpleLightCrateW/2, -simpleLightCrateH/2}, {simpleLightCrateW/2, simpleLightCrateH/2}, {-simpleLightCrateW/2, simpleLightCrateH/2}});
    ModelCycle simpleLightCrateModelCycle(simpleLightCrateMesh);
    simpleLightCrateModelCycle.addModelAndResetIterator(simpleLightCrateModel, 1);
    ModelCollection simpleLightCrateModelCollection;
    simpleLightCrateModelCollection.addModelCycleForState(IDLE, simpleLightCrateModelCycle);
    Point simpleLightCrateCenter = {650, 0};
    MobileObject* simpleLightCrate = engine.makeMobileObject(simpleLightCrateCenter, simpleLightCrateModelCollection, 45);

    SDL_Texture* simpleLightCrate2Texture = engine.createTexture("resources/simple_crate.png");
    std::pair<float, float> simpleLightCrate2WAH = engine.readTexturesWidthAndHeight(simpleLightCrate2Texture);
    float simpleLightCrate2W = simpleLightCrate2WAH.first;
    float simpleLightCrate2H = simpleLightCrate2WAH.second;
    MobileHitbox* simpleLightCrate2Hitbox = engine.makeMobileHitbox({{-simpleLightCrate2W/2, -simpleLightCrate2H/2}, {simpleLightCrate2W/2, -simpleLightCrate2H/2}, {simpleLightCrate2W/2, simpleLightCrate2H/2}, {-simpleLightCrate2W/2, simpleLightCrate2H/2}});
    Model simpleLightCrate2Model(simpleLightCrate2Texture, simpleLightCrate2Hitbox);
    MobileHitbox* simpleLightCrate2Mesh = engine.makeMobileHitbox({{-simpleLightCrate2W/2, -simpleLightCrate2H/2}, {simpleLightCrate2W/2, -simpleLightCrate2H/2}, {simpleLightCrate2W/2, simpleLightCrate2H/2}, {-simpleLightCrate2W/2, simpleLightCrate2H/2}});
    ModelCycle simpleLightCrate2ModelCycle(simpleLightCrate2Mesh);
    simpleLightCrate2ModelCycle.addModelAndResetIterator(simpleLightCrate2Model, 1);
    ModelCollection simpleLightCrate2ModelCollection;
    simpleLightCrate2ModelCollection.addModelCycleForState(IDLE, simpleLightCrate2ModelCycle);
    Point simpleLightCrate2Center = {550, 0};
    MobileObject* simpleLightCrate2 = engine.makeMobileObject(simpleLightCrate2Center, simpleLightCrate2ModelCollection, 45);

    SDL_Texture* rapierUnowndedTexture = engine.createTexture("resources/golden_icon.png");
    std::pair<float, float> rapierUnowndedWAH = engine.readTexturesWidthAndHeight(rapierUnowndedTexture);
    float rapierUnowndedW = rapierUnowndedWAH.first;
    float rapierUnowndedH = rapierUnowndedWAH.second;
    MobileHitbox* rapierUnowndedHitbox = engine.makeMobileHitbox({{-rapierUnowndedW/2, -rapierUnowndedH/2}, {rapierUnowndedW/2, -rapierUnowndedH/2}, {rapierUnowndedW/2, rapierUnowndedH/2}, {-rapierUnowndedW/2, rapierUnowndedH/2}});
    Model rapierUnowndedModel(rapierUnowndedTexture, rapierUnowndedHitbox);
    MobileHitbox* rapierUnowndedMesh = engine.makeMobileHitbox({{-rapierUnowndedW/2, -rapierUnowndedH/2}, {rapierUnowndedW/2, -rapierUnowndedH/2}, {rapierUnowndedW/2, rapierUnowndedH/2}, {-rapierUnowndedW/2, rapierUnowndedH/2}});
    ModelCycle rapierUnowndedModelCycle(rapierUnowndedMesh);
    rapierUnowndedModelCycle.addModelAndResetIterator(rapierUnowndedModel, 1);

    SDL_Texture* rapierLeftTexture = engine.createTexture("resources/nobles_rapier_left.png");
    std::pair<float, float> rapierLeftWAH = engine.readTexturesWidthAndHeight(rapierLeftTexture);
    float rapierLeftW = rapierLeftWAH.first;
    float rapierLeftH = rapierLeftWAH.second;
    MobileHitbox* rapierLeftHitbox = engine.makeMobileHitbox({{-rapierLeftW/2, -rapierLeftH/2}, {rapierLeftW/2, -rapierLeftH/2}, {rapierLeftW/2, rapierLeftH/2}, {-rapierLeftW/2, rapierLeftH/2}});
    Model rapierLeftModel(rapierLeftTexture, rapierLeftHitbox);
    MobileHitbox* rapierLeftMesh = engine.makeMobileHitbox({{-rapierLeftW/2, -rapierLeftH/2}, {rapierLeftW/2, -rapierLeftH/2}, {rapierLeftW/2, rapierLeftH/2}, {-rapierLeftW/2, rapierLeftH/2}});
    ModelCycle rapierLeftModelCycle(rapierLeftMesh);
    rapierLeftModelCycle.addModelAndResetIterator(rapierLeftModel, 1);

    SDL_Texture* rapierRightTexture = engine.createTexture("resources/nobles_rapier_right.png");
    std::pair<float, float> rapierRightWAH = engine.readTexturesWidthAndHeight(rapierRightTexture);
    float rapierRightW = rapierRightWAH.first;
    float rapierRightH = rapierRightWAH.second;
    MobileHitbox* rapierRightHitbox = engine.makeMobileHitbox({{-rapierRightW/2, -rapierRightH/2}, {rapierRightW/2, -rapierRightH/2}, {rapierRightW/2, rapierRightH/2}, {-rapierRightW/2, rapierRightH/2}});
    Model rapierRightModel(rapierRightTexture, rapierRightHitbox);
    MobileHitbox* rapierRightMesh = engine.makeMobileHitbox({{-rapierRightW/2, -rapierRightH/2}, {rapierRightW/2, -rapierRightH/2}, {rapierRightW/2, rapierRightH/2}, {-rapierRightW/2, rapierRightH/2}});
    ModelCycle rapierRightModelCycle(rapierRightMesh);
    rapierRightModelCycle.addModelAndResetIterator(rapierRightModel, 1);

    ModelCollection rapierModelCollection;
    rapierModelCollection.addModelCycleForState(IDLE, rapierUnowndedModelCycle);
    
    rapierModelCollection.addModelCycleForState(OWNED_LEFT, rapierLeftModelCycle);
    rapierModelCollection.addModelCycleForState(IN_USE_LEFT, rapierLeftModelCycle);
    rapierModelCollection.addModelCycleForState(IN_RECOVERY_LEFT, rapierLeftModelCycle);

    rapierModelCollection.addModelCycleForState(OWNED_RIGHT, rapierRightModelCycle);
    rapierModelCollection.addModelCycleForState(IN_USE_RIGHT, rapierRightModelCycle);
    rapierModelCollection.addModelCycleForState(IN_RECOVERY_RIGHT, rapierRightModelCycle);

    Point rapierCenter = {80, 0};
    ThrustingWeapon* rapier = engine.makeThrustingWeapon(rapierCenter, rapierModelCollection, 45, 10, 10, 16, 12, 26);
    rapier->setHasWindUp(true);
    rapier->setWindUpExtendRange(10);
    rapier->setWindUpFrames(5);

    SDL_Texture* orangeBulletTexture = engine.createTexture("resources/orange_bullet.png");
    std::pair<float, float> orangeBulletWAH = engine.readTexturesWidthAndHeight(orangeBulletTexture);
    float orangeBulletW = orangeBulletWAH.first;
    float orangeBulletH = orangeBulletWAH.second;
    MobileHitbox* orangeBulletHitbox = engine.makeMobileHitbox({{0,0}});
    Model orangeBulletModel(orangeBulletTexture, orangeBulletHitbox);
    MobileHitbox* orangeBulletMesh = engine.makeMobileHitbox({{0,0}});
    ModelCycle orangeBulletModelCycle(orangeBulletMesh);
    orangeBulletModelCycle.addModelAndResetIterator(orangeBulletModel, 1);
    ModelCollection orangeBulletModelCollection;
    orangeBulletModelCollection.addModelCycleForState(IDLE, orangeBulletModelCycle);
    orangeBulletModelCollection.addModelCycleForState(MOVING_LEFT, orangeBulletModelCycle);
    orangeBulletModelCollection.addModelCycleForState(MOVING_RIGHT, orangeBulletModelCycle);
    MobileHitbox leftTravellingHitbox(0, {{0,0}});
    MobileHitbox rightTravellingHitbox(0, {{0,0}});
    int orangeBulletIndex = engine.registerNewProjectileArchetype(orangeBulletModelCollection, leftTravellingHitbox, rightTravellingHitbox);
    // std::cout << orangeBulletIndex << std::endl;

    SDL_Texture* redGunUnownedTexture = engine.createTexture("resources/golden_icon.png");
    std::pair<float, float> redGunUnownedWAH = engine.readTexturesWidthAndHeight(redGunUnownedTexture);
    float redGunUnownedW = redGunUnownedWAH.first;
    float redGunUnownedH = redGunUnownedWAH.second;
    MobileHitbox* redGunUnownedHitbox = engine.makeMobileHitbox({{-redGunUnownedW/2, -redGunUnownedH/2}, {redGunUnownedW/2, -redGunUnownedH/2}, {redGunUnownedW/2, redGunUnownedH/2}, {-redGunUnownedW/2, redGunUnownedH/2}});
    Model redGunUnownedModel(redGunUnownedTexture, redGunUnownedHitbox);
    MobileHitbox* redGunUnownedMesh = engine.makeMobileHitbox({{-redGunUnownedW/2, -redGunUnownedH/2}, {redGunUnownedW/2, -redGunUnownedH/2}, {redGunUnownedW/2, redGunUnownedH/2}, {-redGunUnownedW/2, redGunUnownedH/2}});
    ModelCycle redGunUnownedModelCycle(redGunUnownedMesh);
    redGunUnownedModelCycle.addModelAndResetIterator(redGunUnownedModel, 1);

    SDL_Texture* redGunLeftFacingTexture = engine.createTexture("resources/red_rect.png");
    std::pair<float, float> redGunLeftFacingWAH = engine.readTexturesWidthAndHeight(redGunLeftFacingTexture);
    float redGunLeftFacingW = redGunLeftFacingWAH.first;
    float redGunLeftFacingH = redGunLeftFacingWAH.second;
    MobileHitbox* redGunLeftFacingHitbox = engine.makeMobileHitbox({{-redGunLeftFacingW/2, -redGunLeftFacingH/2}, {redGunLeftFacingW/2, -redGunLeftFacingH/2}, {redGunLeftFacingW/2, redGunLeftFacingH/2}, {-redGunLeftFacingW/2, redGunLeftFacingH/2}});
    Model redGunLeftFacingModel(redGunLeftFacingTexture, redGunLeftFacingHitbox);
    MobileHitbox* redGunLeftFacingMesh = engine.makeMobileHitbox({{-redGunLeftFacingW/2, -redGunLeftFacingH/2}, {redGunLeftFacingW/2, -redGunLeftFacingH/2}, {redGunLeftFacingW/2, redGunLeftFacingH/2}, {-redGunLeftFacingW/2, redGunLeftFacingH/2}});
    ModelCycle redGunLeftFacingModelCycle(redGunLeftFacingMesh);
    redGunLeftFacingModelCycle.addModelAndResetIterator(redGunLeftFacingModel, 1);

    SDL_Texture* redGunRightFacingTexture = engine.createTexture("resources/red_rect.png");
    std::pair<float, float> redGunRightFacingWAH = engine.readTexturesWidthAndHeight(redGunRightFacingTexture);
    float redGunRightFacingW = redGunRightFacingWAH.first;
    float redGunRightFacingH = redGunRightFacingWAH.second;
    MobileHitbox* redGunRightFacingHitbox = engine.makeMobileHitbox({{-redGunRightFacingW/2, -redGunRightFacingH/2}, {redGunRightFacingW/2, -redGunRightFacingH/2}, {redGunRightFacingW/2, redGunRightFacingH/2}, {-redGunRightFacingW/2, redGunRightFacingH/2}});
    Model redGunRightFacingModel(redGunRightFacingTexture, redGunRightFacingHitbox);
    MobileHitbox* redGunRightFacingMesh = engine.makeMobileHitbox({{-redGunRightFacingW/2, -redGunRightFacingH/2}, {redGunRightFacingW/2, -redGunRightFacingH/2}, {redGunRightFacingW/2, redGunRightFacingH/2}, {-redGunRightFacingW/2, redGunRightFacingH/2}});
    ModelCycle redGunRightFacingModelCycle(redGunRightFacingMesh);
    redGunRightFacingModelCycle.addModelAndResetIterator(redGunRightFacingModel, 1);

    ModelCollection redGunModelCollection;
    redGunModelCollection.addModelCycleForState(IDLE, redGunUnownedModelCycle);
    redGunModelCollection.addModelCycleForState(OWNED_LEFT, redGunLeftFacingModelCycle);
    redGunModelCollection.addModelCycleForState(OWNED_RIGHT, redGunRightFacingModelCycle);

    Point redGunCenter(1180, 0);
    FirearmFireSpecs redGunFireSpecs(1, 15);   
    FirearmFireSpecs redGunAltFireSpecs(3, 15);
    redGunAltFireSpecs.bulletSpreadFromCenter = {0, M_PI/6.0, - M_PI/6.0};
    Firearm* redGun = engine.makeFirearm(redGunCenter, redGunModelCollection, 10, orangeBulletIndex, redGunFireSpecs, redGunAltFireSpecs);
    // std::cout << redGun << std::endl;    

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
    Rectangle celledRectangle({-10, -1000}, {1500, 0});
    OfflineEngine engine(windowWidth, windowHeight, {celledRectangle, 4, 4});
    prepareGame(windowWidth, windowHeight, engine);
    engine.setAnchoredOnPlayerCenter();
    engine.run();
    // std::vector<Point> S1 = {Point(21,1), Point(20,3), Point(18,5), Point(14,6), Point(7,5), Point(4,4), Point(2,2)};
    // std::vector<Point> R1 = {Point(21, 10), Point(20,9), Point(19,8), Point(12,7), Point(7,7), Point(4,8), Point(3,10)};
    // std::vector<Point> P1 = {{5,11}, {6,9}, {7,7}, {10,7}, {15,9}, {23,14}};

    // std::cout << calculateMinVertDistance(S1, R1) << std::endl;
    // std::cout << calculateMinVertDistance(S1, P1) << std::endl;
    // std::cout << calculateMinVertDistance(R1, S1) << std::endl;
    // std::cout << calculateMinVertDistance(P1, S1) << std::endl;
    // std::cout << calculateMinVertDistance(P1, R1) << std::endl;

    // std::vector<Point> S = {{593.98, -332.5}, {642.98, -332.5}, {642.98, -283.5}, {593.98, -283.5}};
    // std::vector<Point> R = {{631, -332.5}, {669, -332.5}, {669, -292.5}, {631, -292.5}};
    // std::cout << gjk(S, R) << std::endl;

    return 0;
}