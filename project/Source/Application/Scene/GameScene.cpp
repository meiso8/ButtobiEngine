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

#include"MyEngine.h"
#include"UI/PauseScreen.h"
GameScene::GameScene()
{
    // 現在のカメラを設定
    currentCamera_ = camera_.get();

#pragma region // オブジェクト生成
    //衝突判定管理
    collisionManager_ = std::make_unique<CollisionManager>();

    floorGamePlayer_ = std::make_unique<FloorGamePlayer>();
    playerFloorStripManager_ = std::make_unique<PlayerFloorStripManager>(floorGamePlayer_.get());
    floorGameFloorManager_ = std::make_unique<FloorGameFloorManager>();
    floorStripManager_ = std::make_unique<FloorStripManager>(floorGamePlayer_.get(), floorGameFloorManager_.get(), playerFloorStripManager_.get());
    floorBulletManager_ = std::make_unique<FloorBulletManager>();
    floorPlayerShotBulletManager_ = std::make_unique<FloorPlayerShotBulletManager>(floorGamePlayer_.get(), floorBulletManager_.get());
    floorPlayerStripTargetUI_ = std::make_unique<FloorPlayerStripTargetUI>(floorGamePlayer_.get());
    floorActionManager_ = std::make_unique<FloorActionManager>(floorGamePlayer_.get(), floorGameFloorManager_.get());
    floorGamePlayerAnimationManager_ = std::make_unique<FloorGamePlayerAnimationManager>(floorGamePlayer_.get(), floorGameFloorManager_.get());

    enemy_ = std::make_unique<Enemy>();
    enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
    enemyShotBulletManager_ = std::make_unique<EnemyShotBulletManager>(enemy_.get(), enemyBulletManager_.get());
    enemyBombManager_ = std::make_unique<EnemyBombManager>();
    enemyShotBombManager_ = std::make_unique<EnemyShotBombManager>(enemy_.get(), enemyBombManager_.get(), floorGameFloorManager_.get());
    enemyShockWaveManager_ = std::make_unique<EnemyShockWaveManager>();
    enemyShotWaveManager_ = std::make_unique<EnemyShotWaveManager>(enemy_.get(), enemyShockWaveManager_.get(), floorGameFloorManager_.get());

    uiManager_ = std::make_unique<UIManager>(*enemy_->GetHpsPtr(), *floorGamePlayer_->GetHpsPtr());

    particleEmitter_ = std::make_unique<ParticleEmitter>();

    particleEmitter_->SetName("box");
    particleEmitter_->emitter_.transform.Parent(enemy_->bodyPos_.worldTransform_);
#pragma endregion
}

void GameScene::Initialize() {

    MyEngine::GetDirectionalLightData()->direction = { 0.0f,0.0f,1.0f };

    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);
    camera_->Initialize();
    camera_->translate_ = { 0.0f, 14.0f,-6.0f };
    camera_->rotate_ = { 1.2f,0.0f,0.0f };
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

    enemy_->Init();
    enemy_->SetTarget(floorGamePlayer_->body_.worldTransform_.translate_);
    enemy_->SetPlayerPos(floorGamePlayer_->body_.worldTransform_.translate_);
    enemyBulletManager_->Initialize();
    enemyShotBulletManager_->Initialize();
    enemyBombManager_->Initialize();
    enemyShotBombManager_->Initialize();
    enemyShockWaveManager_->Initialize();
    enemyShotWaveManager_->Initialize();


#pragma endregion
    collisionManager_->ClearColliders();

    uiManager_->Initialize();

    particleEmitter_->Initialize();
}

void GameScene::Update() {



    if (/*enemy_->GetHpsPtr()->hp <= 0.0f||*/floorGamePlayer_->GetHpsPtr()->hp <= 0.0f) {

#ifdef _DEBUG
        Initialize();
#endif
        /* sceneChange_->SetState(SceneChange::kFadeIn, 30);*/
    }

    //仮に音声を鳴らす　全体のvolumeがあってオフセット分だけいじる
    Sound::PlayBGM(Sound::BGM1, 0.0f);

    UpdateCamera();

    if (!PauseScreen::isPause_) {
        UpdateGameObject();
        CheckAllCollision();
        particleEmitter_->Update(*currentCamera_);
    }

    uiManager_->Update();


}

void GameScene::Draw() {

#ifdef _DEBUG
    DrawGrid::Draw(*currentCamera_);
#endif

#pragma region // オブジェクト描画
    floorGamePlayer_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    floorGameFloorManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    floorBulletManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    floorPlayerStripTargetUI_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    playerFloorStripManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    enemy_->Draw(*currentCamera_, kLightModeHalfL);
    enemyBulletManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    enemyBombManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    enemyShockWaveManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);

#pragma endregion

    particleEmitter_->Draw();

    uiManager_->Draw();

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
    enemy_->Update();
    enemyBulletManager_->Update();
    enemyBombManager_->Update();
    enemyShockWaveManager_->Update();

    // オブジェクト同士の干渉
    floorStripManager_->Update();
    playerFloorStripManager_->Update();
    floorPlayerShotBulletManager_->Update();
    enemyShotBulletManager_->Update();
    enemyShotBombManager_->Update();
    enemyShotWaveManager_->Update();
    floorPlayerStripTargetUI_->Update();
    floorActionManager_->Update();

    // アニメーション更新
    floorGamePlayerAnimationManager_->Update();
}

void GameScene::CheckAllCollision()
{

    collisionManager_->ClearColliders();

    collisionManager_->AddCollider(floorGamePlayer_.get());
    collisionManager_->AddCollider(enemy_.get());

    for (auto& bullet : floorBulletManager_->GetBullets()) {
        if (bullet->isActive_) { collisionManager_->AddCollider(bullet.get()); }
    }

    for (auto& bullet : enemyBulletManager_->GetBullets()) {
        if (bullet->isActive_) { collisionManager_->AddCollider(bullet.get()); }
    }

    for (auto& bomb : enemyBombManager_->GetBombs()) {
        if (bomb->isActive_) { collisionManager_->AddCollider(bomb.get()); }
    }

    for (auto& wave : enemyShockWaveManager_->GetWaves()) {
        if (wave->isActive_) { collisionManager_->AddCollider(wave.get()); }
    }

    collisionManager_->CheckAllCollisions();

}

GameScene::~GameScene()
{
    camera_ = nullptr;
}