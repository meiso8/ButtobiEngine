#pragma once

#include"SceneManager.h"

#include<memory>
#include<array>

#pragma region //ゲームオブジェクト
#include "MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include "MatsumotoObj/GameSceneObj/FloorGameFloorManager.h"
#include "MatsumotoObj/GameSceneObj/FloorStripManager.h"
#include "MatsumotoObj/GameSceneObj/FloorBulletManager.h"
#include "MatsumotoObj/GameSceneObj/FloorPlayerShotBulletManager.h"
#include "MatsumotoObj/GameSceneObj/FloorPlayerStripTargetUI.h"
#include "MatsumotoObj/GameSceneObj/FloorActionManager.h"
#include "MatsumotoObj/GameSceneObj/FloorGamePlayerAnimationManager.h"
#include "MatsumotoObj/GameSceneObj/PlayerStripFloorManager.h"
#include "MatsumotoObj/GameSceneObj/HealItemSpawner.h"
#include "MatsumotoObj/GameSceneObj/GameOverEvent.h"
#include "MatsumotoObj/TitleSceneObj/ActionUI.h"
#include "MatsumotoObj/TitleSceneObj/LetterboxBars.h"

#include "Enemy/Enemy.h"
#include "Enemy/EnemyBulletManager.h"
#include "Enemy/EnemyShotBulletManager.h"
#include "Enemy/EnemyBombManager.h"
#include "Enemy/EnemyShotBombManager.h"
#include "Enemy/EnemyShockWaveManager.h"
#include "Enemy/EnemyShotWaveManager.h"
#include"Tree/Tree.h"
#include"Ground.h"
#pragma endregion

#include"CollisionManager.h"
#include"UI/UIManager.h"
#include"EmitterManager/EmitterManager.h"
#include"CameraController.h"


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

    std::unique_ptr<CameraController>cameraController_ = nullptr;

#pragma region//ゲームオブジェクト
    std::unique_ptr<FloorGamePlayer>floorGamePlayer_ = nullptr;
    std::unique_ptr<FloorGameFloorManager>floorGameFloorManager_ = nullptr;
    std::unique_ptr<FloorStripManager>floorStripManager_ = nullptr;
    std::unique_ptr<FloorBulletManager>floorBulletManager_ = nullptr;
    std::unique_ptr<FloorPlayerShotBulletManager>floorPlayerShotBulletManager_ = nullptr;
    std::unique_ptr<FloorPlayerStripTargetUI>floorPlayerStripTargetUI_ = nullptr;
	std::unique_ptr<FloorActionManager>floorActionManager_ = nullptr;
	std::unique_ptr<FloorGamePlayerAnimationManager>floorGamePlayerAnimationManager_ = nullptr;
	std::unique_ptr<PlayerFloorStripManager>playerFloorStripManager_ = nullptr;
	std::unique_ptr<HealItemSpawner>healItemSpawner_ = nullptr;
	std::unique_ptr<GameOverEvent>gameOverEvent_ = nullptr;
	std::unique_ptr<ActionUI>actionUI_ = nullptr;
	std::unique_ptr<LetterboxBars>letterboxBars_ = nullptr;

    std::unique_ptr<Enemy>enemy_ = nullptr;
    std::unique_ptr<EnemyBulletManager>enemyBulletManager_ = nullptr;
    std::unique_ptr<EnemyShotBulletManager>enemyShotBulletManager_ = nullptr;
    std::unique_ptr<EnemyBombManager>enemyBombManager_ = nullptr;
    std::unique_ptr<EnemyShotBombManager>enemyShotBombManager_ = nullptr;
    std::unique_ptr<EnemyShockWaveManager>enemyShockWaveManager_ = nullptr;
    std::unique_ptr<EnemyShotWaveManager>enemyShotWaveManager_ = nullptr;

    std::unique_ptr<Tree>tree_ = nullptr;
    std::unique_ptr<Ground> ground_ = nullptr;
#pragma endregion

#pragma region//UI
    std::unique_ptr<UIManager>uiManager_ = nullptr;
#pragma endregion
    std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
    std::unique_ptr<EmitterManager>emitterManager_ = nullptr;

	float gameclearTimer_ = 0.0f;
};
