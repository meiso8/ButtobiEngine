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
#include"VibrateManager.h"
#include"MatsumotoObj/SceneStaticValue.h"

GameScene::GameScene()
{
    // 現在のカメラを設定
    currentCamera_ = camera_.get();
    cameraController_ = std::make_unique<CameraController>(*camera_);

#pragma region // オブジェクト生成
    //衝突判定管理
    collisionManager_ = std::make_unique<CollisionManager>();

    enemy_ = std::make_unique<Enemy>();
    floorGamePlayer_ = std::make_unique<FloorGamePlayer>();
    playerFloorStripManager_ = std::make_unique<PlayerFloorStripManager>(floorGamePlayer_.get());
    floorGameFloorManager_ = std::make_unique<FloorGameFloorManager>();
    floorStripManager_ = std::make_unique<FloorStripManager>(floorGamePlayer_.get(), floorGameFloorManager_.get(), playerFloorStripManager_.get());
    floorBulletManager_ = std::make_unique<FloorBulletManager>(enemy_.get());
    floorPlayerShotBulletManager_ = std::make_unique<FloorPlayerShotBulletManager>(floorGamePlayer_.get(), floorBulletManager_.get());
    floorPlayerStripTargetUI_ = std::make_unique<FloorPlayerStripTargetUI>(floorGamePlayer_.get());
    floorActionManager_ = std::make_unique<FloorActionManager>(floorGamePlayer_.get(), floorGameFloorManager_.get());
    floorGamePlayerAnimationManager_ = std::make_unique<FloorGamePlayerAnimationManager>(floorGamePlayer_.get(), floorGameFloorManager_.get());
    healItemSpawner_ = std::make_unique<HealItemSpawner>();
    actionUI_ = std::make_unique<ActionUI>(floorGamePlayer_.get());

    enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
    enemyShotBulletManager_ = std::make_unique<EnemyShotBulletManager>(enemy_.get(), enemyBulletManager_.get());
    enemyBombManager_ = std::make_unique<EnemyBombManager>();
    enemyShotBombManager_ = std::make_unique<EnemyShotBombManager>(enemy_.get(), enemyBombManager_.get(), floorGameFloorManager_.get());
    enemyShockWaveManager_ = std::make_unique<EnemyShockWaveManager>();
    enemyShotWaveManager_ = std::make_unique<EnemyShotWaveManager>(enemy_.get(), enemyShockWaveManager_.get(), floorGameFloorManager_.get());

    tree_ = std::make_unique<Tree>();

#pragma endregion

    uiManager_ = std::make_unique<UIManager>(*enemy_->GetHpsPtr(), *floorGamePlayer_->GetHpsPtr());

    emitterManager_ = std::make_unique<EmitterManager>(*floorGamePlayer_, *enemy_);
}

void GameScene::Initialize() {

    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);

    cameraController_->Initialize();

#pragma region // オブジェクト初期化
    floorGamePlayer_->Initialize();
    floorGameFloorManager_->Initialize();
    floorStripManager_->Initialize();
    floorBulletManager_->Initialize();
    floorPlayerShotBulletManager_->Initialize();
    floorPlayerStripTargetUI_->Initialize();
    floorActionManager_->Initialize();
    playerFloorStripManager_->Initialize();
    healItemSpawner_->Initialize();
    actionUI_->Initialize();

    healItemSpawner_->SpawnHealItem({ 2.0f,1.0f,2.0f });

    enemy_->Init();
    enemy_->SetTarget(floorGamePlayer_->body_.worldTransform_.translate_);
    enemy_->SetPlayerPos(floorGamePlayer_->body_.worldTransform_.translate_);
    enemy_->SetPlayerLookDirPos(floorGamePlayer_->GetLookDir());
    enemyBulletManager_->Initialize();
    enemyShotBulletManager_->Initialize();
    enemyBombManager_->Initialize();
    enemyShotBombManager_->Initialize();
    enemyShockWaveManager_->Initialize();
    enemyShotWaveManager_->Initialize();

    tree_->Initialize();

#pragma endregion
    collisionManager_->ClearColliders();

    uiManager_->Initialize();

    emitterManager_->Initialize();

}

void GameScene::Update() {

    if (PauseScreen::isRetry) {
        PauseScreen::isRetry = false;
        Initialize();
    }

    if (enemy_->IsDead()) {
        //ひたすらなんかする
    }

    if (PauseScreen::isBackToTitle || floorGamePlayer_->IsDead()||enemy_->IsOverKill()) {

        //if (enemy_->IsDead()) {
        //    SceneStaticValue::isClear = true;
        //}

        sceneChange_->SetState(SceneChange::kFadeIn, 30);
    }



    //仮に音声を鳴らす　全体のvolumeがあってオフセット分だけいじる
    Sound::PlayBGM(Sound::BGM1, 0.0f);

    if (!PauseScreen::isActive_) {
        UpdateCamera();
        UpdateGameObject();
        CheckAllCollision();
        //エミッター
        emitterManager_->Update(*currentCamera_);
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
    healItemSpawner_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    actionUI_->Draw();
    enemy_->Draw(*currentCamera_, kLightModeHalfL);
    enemyBulletManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    enemyBombManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    enemyShockWaveManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    tree_->Draw(*currentCamera_);
#pragma endregion
    //エミッター
    emitterManager_->Draw();

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
    emitterManager_->Debug();
#endif // !USE_IMGUI
}

void GameScene::UpdateCamera()
{

    if (isDebugCameraActive_) {
        debugCamera_->UpdateMatrix();
    } else {

        if (floorGamePlayer_->IsHit()) {
            cameraController_->StartShake(2.0f, 60);
        } else {
            cameraController_->SetShakeFalse();
        }
        cameraController_->Update();
    }

}

void GameScene::UpdateGameObject()
{
    // オブジェクト個人の更新
    floorGamePlayer_->Update();
    floorGameFloorManager_->Update();
    floorBulletManager_->Update();
    healItemSpawner_->Update();
    enemy_->Update();
    enemyBulletManager_->Update();
    enemyBombManager_->Update();
    enemyShockWaveManager_->Update();
    tree_->Update();


    // オブジェクト同士の干渉
    actionUI_->Update();
    floorStripManager_->Update();
    playerFloorStripManager_->Update();
    floorPlayerShotBulletManager_->Update();
    enemyShotBulletManager_->Update();
    enemyShotBombManager_->Update();
    enemyShotWaveManager_->Update();
    floorPlayerStripTargetUI_->Update();
    floorActionManager_->Update();
    if (enemy_->isReqestClearFloor_) {
        floorGameFloorManager_->Initialize();
        enemy_->isReqestClearFloor_ = false;
    }

    // アニメーション更新
    floorGamePlayerAnimationManager_->Update();

}

void GameScene::CheckAllCollision()
{

    collisionManager_->ClearColliders();

    collisionManager_->AddCollider(floorGamePlayer_.get());
    collisionManager_->AddCollider(enemy_.get());
    healItemSpawner_->AddCollider(collisionManager_.get());
    floorGameFloorManager_->AddCollider(collisionManager_.get());

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