#pragma once
#include"SceneManager.h"
#include<memory>
#include<array>

#include"Object3d.h"
#include"CollisionManager.h"

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

#include "MatsumotoObj/TitleSceneObj/TitleText.h"
#include "MatsumotoObj/TitleSceneObj/BossDummy.h"
#include "MatsumotoObj/TitleSceneObj/LetterboxBars.h"
#include "MatsumotoObj/TitleSceneObj/ActionUI.h"


#include"BackGround/House/House.h"
#include"BackGround/BackGround.h"

#include"EmitterManager/EmitterManager.h"

#pragma endregion

class TitleScene :public SceneManager
{
public:
    TitleScene();
    ~TitleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
    void SceneChangeUpdate()override;

private:
    void UpdateCamera();
    void UpdateGameObject();
    void CheckAllCollision();

private:
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

    std::unique_ptr<TitleText> titleText_ = nullptr;
    std::unique_ptr<BossDummy> bossDummy_ = nullptr;
    std::unique_ptr<LetterboxBars> letterboxBars_ = nullptr;
    std::unique_ptr<ActionUI> actionUI_ = nullptr;
    //家追加しましたヨシダ
    std::unique_ptr<House>house_ = nullptr;
    std::unique_ptr<BackGround>backGround_ = nullptr;

    //エミッター
    std::unique_ptr<EmitterManager>emitterManager_ = nullptr;

#pragma endregion
    float eventTimer_ = 0.0f;

    std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
};

