#include "GameScene.h"
#define NOMINMAX
//入力処理に必要なもの
#include "Input.h"

//Debug用のImGui表示セット
#include"DebugUI.h"
//ImGuiだけ使用したかったら以下をインクルードすること
//#include"ImGuiClass.h"

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
//void SetMinMax(const AABB& aabb);
//void SetVertex(const Vector3(&vertices)[8]);

#include "ParticleEmitter.h"
#include"Particle.h"

#include"Random.h"
#include"MakeMatrix.h"

#include"Collision.h"
#include"Circle/CircleMesh.h"
#include"Circle.h"

GameScene::GameScene()
{
    camera_ = std::make_unique<Camera>();
#ifdef _DEBUG
    // デバッグカメラの初期化
    debugCamera_ = std::make_unique<DebugCamera>();
#endif // _DEBUG

    // 現在のカメラを設定
    currentCamera_ = camera_.get();

#pragma region// Particle正常に機能するかは怪しい
    //パーティクル管理
    particleManager_ = ParticleManager::GetInstance();
    particleManager_->Create();
    particleManager_->CreateParticleGroup("numbers", Texture::NUMBERS);
    //エミッター
    particleEmitter_ = std::make_unique<ParticleEmitter>();
    particleEmitter_->SetName("numbers");
#pragma endregion

    player_ = std::make_unique<Player>();
    world_ = std::make_unique<World>();
    enemy_ = std::make_unique<Enemy>();
    enemy_->SetTarget(player_->GetCircle().center);
    filed_ = std::make_unique<Field>();
}

void GameScene::Initialize() {

    sceneChange_.Initialize();
    camera_->Initialize();
    camera_->UpdateMatrix();
    particleEmitter_->Initialize();
    player_->Init();
    world_->Init();
    enemy_->Init();
    filed_->Init();
}

void GameScene::Update() {

    if (isDebugCameraActive_) {
        //これを呼べばOK
        currentCamera_->UpdateMatrix();
        /*    camera_->UpdateMatrix();*/
    } else {
        //プレイヤーの視点にしている
        camera_->worldMat_ = player_->GetEyeMatrix();
        camera_->UpdateViewProjectionMatrix();
    }

    player_->Update();
    enemy_->Update();
    filed_->Update();
    world_->Update();

    particleEmitter_->Update();
    particleManager_->Update(*currentCamera_);

    CheckAllCollision();
}


GameScene::~GameScene()
{
    camera_ = nullptr;
}

void GameScene::Debug()
{

    if (Input::IsTriggerKey(DIK_Q)) {
        SwitchCamera();
    }

#ifdef USE_IMGUI

    ImGui::Text("SwitchCamera : Q key");
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    DebugUI::CheckParticle(*particleManager_, *particleEmitter_);

#endif // !USE_IMGUI
}

void GameScene::CheckAllCollision()
{

    //フィールドの外側に行ったかどうかを判定する
    if (IsCollisionInCircleLine(player_->GetCircle(), filed_->circle_)) {
        player_->OnCollision(filed_->circle_);
    };
    //playerとenemyの当たり判定
    if (IsCollision(player_->GetWorldAABB(), enemy_->GetWorldAABB())) {
        enemy_->OnCollision();
    };
}

void GameScene::Draw() {

#ifdef _DEBUG
    DrawGrid::Draw(*currentCamera_);
#endif
    world_->Draw(*currentCamera_);
    filed_->Draw(*currentCamera_);
    enemy_->Draw(*currentCamera_);
    player_->Draw(*currentCamera_, kLightModeHalfL);
    particleManager_->Draw();
}
