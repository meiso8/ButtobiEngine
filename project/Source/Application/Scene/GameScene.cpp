#include "GameScene.h"
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

GameScene::GameScene()
{

    // 現在のカメラを設定
    currentCamera_ = camera_.get();

#pragma region // オブジェクト生成
    floorGamePlayer_ = std::make_unique<FloorGamePlayer>();
    floorGameFloorManager_ = std::make_unique<FloorGameFloorManager>();
    floorStripManager_ = std::make_unique<FloorStripManager>(floorGamePlayer_.get(), floorGameFloorManager_.get());
    floorBulletManager_ = std::make_unique<FloorBulletManager>();
    floorPlayerShotBulletManager_ = std::make_unique<FloorPlayerShotBulletManager>(floorGamePlayer_.get(), floorBulletManager_.get());
    floorPlayerStripTargetUI_ = std::make_unique<FloorPlayerStripTargetUI>(floorGamePlayer_.get());
#pragma endregion
}

void GameScene::Initialize() {

    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);
    camera_->Initialize();
    camera_->translate_ = { 0.0f, 14.0f,-8.0f };
    camera_->rotate_ = { 1.2f,0.0f,0.0f };
    camera_->UpdateMatrix();

#pragma region // オブジェクト初期化
    floorGamePlayer_->Initialize();
    floorGameFloorManager_->Initialize();
    floorStripManager_->Initialize();
    floorBulletManager_->Initialize();
    floorPlayerShotBulletManager_->Initialize();
    floorPlayerStripTargetUI_->Initialize();
#pragma endregion
}

void GameScene::Update() {

    sceneChange_->Update();

    //仮に音声を鳴らす　全体のvolumeがあってオフセット分だけいじる
    Sound::PlayBGM(Sound::BGM1, 0.1f);

    UpdateCamera();

    UpdateGameObject();

    CheckAllCollision();

}

void GameScene::Draw() {

#ifdef _DEBUG
    DrawGrid::Draw(*currentCamera_);
#endif

#pragma region // オブジェクト描画
    floorGamePlayer_->Draw(*currentCamera_, LightMode::kLightModeNone);
    floorGameFloorManager_->Draw(*currentCamera_, LightMode::kLightModeNone);
    floorBulletManager_->Draw(*currentCamera_, LightMode::kLightModeNone);
    floorPlayerStripTargetUI_->Draw(*currentCamera_, LightMode::kLightModeNone);
#pragma endregion

    //シーン遷移を描画する
    sceneChange_->Draw();
}

void GameScene::Debug()
{

    if (Input::IsTriggerKey(DIK_Q)) {
        SwitchCamera();
    }

#ifdef USE_IMGUI //ImGuiを使用する際はこれで囲んでください
    ImGui::Text("SwitchCamera : Q key");
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
#endif // !USE_IMGUI
}

void GameScene::UpdateCamera()
{
    if (isDebugCameraActive_) {
        currentCamera_->UpdateMatrix();
    } else {
        camera_->UpdateMatrix();
    }
}

void GameScene::UpdateGameObject()
{
    // オブジェクト個人の更新
    floorGamePlayer_->Update();
    floorGameFloorManager_->Update();
    floorBulletManager_->Update();

    // オブジェクト同士の干渉
    floorStripManager_->Update();
    floorPlayerShotBulletManager_->Update();
    floorPlayerStripTargetUI_->Update();
}

void GameScene::CheckAllCollision()
{
}

GameScene::~GameScene()
{
    camera_ = nullptr;
}