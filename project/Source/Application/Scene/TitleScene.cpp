#include "TitleScene.h"
#define NOMINMAX
//入力処理に必要なもの
#include "Input.h"

//Debug用のImGui表示セット
#include"DebugUI.h"

//グリッド表示
#include"DrawGrid.h"

//モデル読み込みに必要なもの
#include"Model.h"
#include"ModelManager.h"
//スプライトに必要なもの
#include"Texture.h"
#include"Sprite.h"
//音を鳴らすのに必要なもの
#include"Sound.h"

//球体のメッシュ
#include"SphereMesh.h"
//平面のメッシュ
#include"PlaneMesh.h"


//円のメッシュ
#include"CircleMesh.h"
//立方体のメッシュ（AABBでセットするか8頂点でセット);
#include"CubeMesh.h"

#include "ParticleEmitter.h"
#include"Particle.h"

#include"Random.h"
#include"MakeMatrix.h"

#include"Collision.h"
#include"Circle.h"

#include"MyEngine.h"

#include "MatsumotoObj/MY_Utility.h"
#include"Lights/PointLightManager.h"
#include"Lights/DirectionalLightManager.h"
#include"Lights/SpotLightManager.h"

#include"Lerp.h"

#include "MatsumotoObj/SceneStaticValue.h"
TitleScene::TitleScene()
{
	SceneStaticValue::isClear = false;
	SceneStaticValue::bossRound = 0;

    // 現在のカメラを設定
    currentCamera_ = camera_.get();

#pragma region // オブジェクト生成
    //衝突判定管理
    collisionManager_ = std::make_unique<CollisionManager>();

    floorGamePlayer_ = std::make_unique<FloorGamePlayer>();
    playerFloorStripManager_ = std::make_unique<PlayerFloorStripManager>(floorGamePlayer_.get());
    floorGameFloorManager_ = std::make_unique<FloorGameFloorManager>();
    floorStripManager_ = std::make_unique<FloorStripManager>(floorGamePlayer_.get(), floorGameFloorManager_.get(), playerFloorStripManager_.get());
    floorBulletManager_ = std::make_unique<FloorBulletManager>(nullptr);
    floorPlayerShotBulletManager_ = std::make_unique<FloorPlayerShotBulletManager>(floorGamePlayer_.get(), floorBulletManager_.get());
    floorPlayerStripTargetUI_ = std::make_unique<FloorPlayerStripTargetUI>(floorGamePlayer_.get());
    floorActionManager_ = std::make_unique<FloorActionManager>(floorGamePlayer_.get(), floorGameFloorManager_.get());
    floorGamePlayerAnimationManager_ = std::make_unique<FloorGamePlayerAnimationManager>(floorGamePlayer_.get(), floorGameFloorManager_.get());

	titleText_ = std::make_unique<TitleText>();
	bossDummy_ = std::make_unique<BossDummy>();
	letterboxBars_ = std::make_unique<LetterboxBars>();
	actionUI_ = std::make_unique<ActionUI>(floorGamePlayer_.get());
    //家追加
    house_ = std::make_unique<House>();

    house_->SetHitCounts(titleText_->GetHitCount(), titleText_->GetMaxHitCount());
    backGround_ = std::make_unique<BackGround>();

    SpotLightManager::GetData()->intensity = 1.0f;
    SpotLightManager::GetData()->cosAngle = 0.99f;
    SpotLightManager::GetData()->direction = { 0.0f,0.0f,1.0f };
    SpotLightManager::GetData()->position = { 0.0f,2.0f,-4.0f };

    PointLightManager::GetPointLightData(0).color = { 17.0f / 255.0f,68.0f / 255.0f,134.0f / 255.0f };
    PointLightManager::GetPointLightData(0).intensity = 6.1f;
    PointLightManager::GetPointLightData(0).radius = 94.0f;
    PointLightManager::GetPointLightData(0).decay = 6.0f;

    PointLightManager::GetPointLightData(1).color = { 0.9f,0.5f,0.0f,1.0f };
    PointLightManager::GetPointLightData(1).radius = 94.0f;
    PointLightManager::GetPointLightData(1).decay = 0.9f;
    PointLightManager::GetPointLightData(1).position = {0.0f,10.0f,7.0f};

    PointLightManager::GetPointLightData(2).color = {1.0f,1.0f,1.0f,1.0f };
    PointLightManager::GetPointLightData(2).radius = 15.0f;
    PointLightManager::GetPointLightData(2).decay = 0.56f;
    PointLightManager::GetPointLightData(2).position = { 0.0f,0.0f,0.0f };
    PointLightManager::GetPointLightData(2).intensity = 1.0f;
#pragma endregion

	eventTimer_ = 0.0f;

    emitterManager_ = std::make_unique<EmitterManager>();

    emitterManager_->SetPlayer(*floorGamePlayer_);
    //emitterManager_->SetEnemy(*enemy_);
    //emitterManager_->SetEnemyWaveShockManager(*enemyShockWaveManager_);
    emitterManager_->SetFloorBuletManager(*floorBulletManager_);
    emitterManager_->SetFloorGameFloorManager(*floorGameFloorManager_);
    emitterManager_->Create();
    emitterManager_->SetHouse(*house_);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    Sound::bgmVolume_ = 0.375f;

    Sound::StopAllSound();
    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);

    DirectionalLightManager::GetDirectionalLightData()->direction = Normalize({ 0.7f,-0.24f,-0.64f });
    DirectionalLightManager::GetDirectionalLightData()->intensity = 1.0f;

    PointLightManager::GetPointLightData(1).intensity = 0.0f;
    PointLightManager::GetPointLightData(0).intensity = 1.0f;

    camera_->Initialize();
    camera_->translate_ = { 0.0f, 8.5f,-16.0f };
    camera_->rotate_ = { 0.4f,0.0f,0.0f };
    camera_->UpdateMatrix();

#pragma region // オブジェクト初期化
    floorGamePlayer_->Initialize();
    floorGameFloorManager_->Initialize();
    floorStripManager_->Initialize();
    floorBulletManager_->Initialize();
    floorPlayerShotBulletManager_->Initialize();
    floorPlayerStripTargetUI_->Initialize();
    floorActionManager_->Initialize();
    playerFloorStripManager_->Initialize();

	titleText_->Initialize();
	bossDummy_->Initialize();
	letterboxBars_->Initialize();
	actionUI_->Initialize();
    
    house_->Initialize();
    backGround_->Initialize();
#pragma endregion

	eventTimer_ = 0.0f;
    emitterManager_->Initialize();
    Sound::Stop(Sound::resultBGM);
}

void TitleScene::Update()
{
    UpdateCamera();

    UpdateGameObject();

    CheckAllCollision();

    //エミッター
    emitterManager_->Update(*currentCamera_);
}

void TitleScene::Draw()
{
#pragma region // オブジェクト描画    
    floorGameFloorManager_->Draw(*currentCamera_);

    //家
    house_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    backGround_->Draw(*currentCamera_);

    if (titleText_->GetIsBreak()) {
        bossDummy_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    } else {
        titleText_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
        floorGamePlayer_->Draw(*currentCamera_);
        floorBulletManager_->Draw(*currentCamera_);
        floorPlayerStripTargetUI_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
        playerFloorStripManager_->Draw(*currentCamera_);
    }
  
    //エミッター
    emitterManager_->Draw();

	letterboxBars_->Draw();
	actionUI_->Draw();

#pragma endregion

    sceneChange_->Draw();
}

void TitleScene::Debug()
{

#ifdef USE_IMGUI //ImGuiを使用する際はこれで囲んでください

    if (Input::IsTriggerKey(DIK_Q)) {
        SwitchCamera();
    }

    ImGui::Text("SwitchCamera : Q key");
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    DebugUI::CheckCamera(*camera_);
    emitterManager_->Debug();
#endif

}

void TitleScene::SceneChangeUpdate()
{
    if (titleText_->GetIsBreak()) {
		camera_->rotate_ = MY_Utility::SimpleEaseIn(camera_->rotate_, { 0.0f,0.0f,0.0f }, 0.1f);
		camera_->translate_ = MY_Utility::SimpleEaseIn(camera_->translate_, { 0.0f,1.5f,0.0f }, 0.1f);

        if (bossDummy_->isAnimEnd_) {
			sceneChange_->SetState(SceneChange::kFadeIn, 30);
		}
    }

#ifdef _DEBUG
    // 何かをしたらシーン遷移
    if (Input::IsTriggerKey(DIK_I)) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
    }
#endif
    sceneChange_->Update();
}

void TitleScene::UpdateCamera() {
    if (isDebugCameraActive_) {
        currentCamera_->UpdateMatrix();
    } else {
        camera_->UpdateMatrix();
    }
}

void TitleScene::UpdateGameObject() {


    // オブジェクト個人の更新
    if (titleText_->GetIsBreak()) {
        Sound::Stop(Sound::titleBGM);
        Sound::PlayBGM(Sound::inGameBGM01);
        bossDummy_->Update();
		actionUI_->isHide_ = true;

    } else {
    /*    Sound::Stop(Sound::inGameBGM01);*/
        Sound::PlayBGM(Sound::titleBGM);
        floorGamePlayer_->Update();
        floorGameFloorManager_->Update();
        floorBulletManager_->Update();
        titleText_->Update();
    }

    //家の更新
    house_->Update();
    backGround_->Update();

	letterboxBars_->isOpen_ = titleText_->GetIsBreak();
	letterboxBars_->Update();
	actionUI_->Update();

    // オブジェクト同士の干渉
    floorStripManager_->Update();
    playerFloorStripManager_->Update();
    floorPlayerShotBulletManager_->Update();
    floorPlayerStripTargetUI_->Update();
    floorActionManager_->Update();

    // アニメーション更新
    floorGamePlayerAnimationManager_->Update();


}

void TitleScene::CheckAllCollision() {
    collisionManager_->ClearColliders();

    collisionManager_->AddCollider(floorGamePlayer_.get());
	collisionManager_->AddCollider(titleText_.get());

    if (!house_->isWallBrake_) {

        for (auto& [type, house] : house_->GetColliders()) {
            collisionManager_->AddCollider(house.get());
        }
    }


    for (auto& bullet : floorBulletManager_->GetBullets()) {
        if (bullet->isActive_) { collisionManager_->AddCollider(bullet.get()); }
    }

    collisionManager_->CheckAllCollisions();
}
