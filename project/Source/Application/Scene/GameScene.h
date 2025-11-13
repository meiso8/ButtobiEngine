#pragma once

#include"SceneManager.h"

#include<memory>
#include<array>

//Transformなどの情報が入っている
#include"Object3d.h"

#pragma region //ゲームオブジェクト
#include "MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include "MatsumotoObj/GameSceneObj/FloorGameFloorManager.h"
#include "MatsumotoObj/GameSceneObj/FloorStripManager.h"
#include "MatsumotoObj/GameSceneObj/FloorBulletManager.h"
#include "MatsumotoObj/GameSceneObj/FloorPlayerShotBulletManager.h"
#include "MatsumotoObj/GameSceneObj/FloorPlayerStripTargetUI.h"
#pragma endregion

class GameScene :public SceneManager {

public:
    GameScene();
    ~GameScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;

private:
    void UpdateCamera();
    void UpdateGameObject();
    //全衝突判定
    void CheckAllCollision();
private:
#pragma region//ゲームオブジェクト
	std::unique_ptr<FloorGamePlayer>floorGamePlayer_ = nullptr;
	std::unique_ptr<FloorGameFloorManager>floorGameFloorManager_ = nullptr;
	std::unique_ptr<FloorStripManager>floorStripManager_ = nullptr;
	std::unique_ptr<FloorBulletManager>floorBulletManager_ = nullptr;
	std::unique_ptr<FloorPlayerShotBulletManager>floorPlayerShotBulletManager_ = nullptr;
	std::unique_ptr<FloorPlayerStripTargetUI>floorPlayerStripTargetUI_ = nullptr;
#pragma endregion
};
