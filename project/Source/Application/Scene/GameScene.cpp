#include "GameScene.h"
#include "AABB.h"
#include"Collision.h"
#include"Input.h"
#include"Model.h"
#include"DebugUI.h"
#include"DrawGrid.h"

const int winWidth = 1280;
const int winHeight = 720;

void GameScene::Initialize() {
    // メンバ変数への代入処理

    DrawGrid::Initialize();

    // カメラの初期化
    camera_.Initialize(winWidth, winHeight,Camera::PERSPECTIVE);

#ifdef _DEBUG
    // デバッグカメラの生成
    debugCamera_ = new DebugCamera();
    debugCamera_->Initialize(winWidth, winHeight);
#endif

    // 自キャラの生成
    player_ = new Player();
    Vector3 playerPosition = { 0.0f, 0.0f, 0.0f };
    // 自キャラの初期化
    player_->Initialize(&camera_, playerPosition);

    enemyModel_ = new Model();
    enemyModel_->Create(ModelManager::ENEMY);

    // 敵キャラ生成
    for (int32_t i = 0; i < kEnemyMax; ++i) {
        Enemy* newEnemy = new Enemy();
        Vector3 enemyPosition = { i * 5.0f + 10.0f, 0.0f, 0.0f };
        newEnemy->Initialize(enemyModel_, &camera_, enemyPosition);
        enemies_.push_back(newEnemy);
    }

    skyDomeModel_ = new Model();
    skyDomeModel_->Create(ModelManager::WORLD);
    skyDome_ = new Skydome();
    // 天球の生成
    skyDome_->Initialize(skyDomeModel_, &camera_);

    // 仮の生成処理。後で消す
    deathParticleModel_ = new Model();
    deathParticleModel_->Create(ModelManager::PARTICLE);
    deathParticles_ = new DeathParticles;
    deathParticles_->Initialize(deathParticleModel_, &camera_, playerPosition);

    // カメラ操作の初期化
    cameraController_ = new CameraController();
    cameraController_->Initialize(&camera_);
    cameraController_->SetTarget(player_);
    cameraController_->Reset();
    cameraController_->SetMovableArea({ 0.0f, 100.0f, 0.0f, 100.0f });

    //UI系
    uiManager_ = new UIManager();
    uiManager_->Initialize();

    // 地形
    stage_ = new Stage();
    stage_->Initialize(&camera_);

    player_->InitializeLife(uiManager_->GetMaxLife());
};

void GameScene::Update() {


    // ここにインゲームの更新処理を書く

    // 地形の更新処理
    stage_->Update();

    // 自キャラの更新処理
    player_->Update();

    // 敵キャラの更新処理
    for (Enemy* newEnemy : enemies_) {
        if (!newEnemy)
            // ガード節と呼ぶ。
            continue;
        newEnemy->Update();
    }

    uiManager_->SetLife(player_->GetLife());

    // デスパーティクルの更新処理
    if (deathParticles_) {
        deathParticles_->Update();
    }

    // 天球の更新処理
    skyDome_->Update();

    // カメラの処理
    if (isDebugCameraActive_) {
        // デバッグカメラの更新
        debugCamera_->UpdateMatrix();
        camera_.viewMat_ = debugCamera_->viewMat_;
        camera_.projectionMat_ = debugCamera_->projectionMat_;
    } else {
        // 行列更新
        cameraController_->Update();
    }

    // 全ての当たり判定を行う
    CheckAllCollisions();
    uiManager_->Update();
};

void GameScene::CheckAllCollisions() {

#pragma region // 自キャラと敵キャラの当たり判定
    // 自キャラと敵キャラの当たり判定

    AABB aabb1, aabb2;

    aabb1 = player_->GetAABB();

    for (Enemy* enemy : enemies_) {
        aabb2 = enemy->GetAABB();

        // AABB同士の交差判定
        if (IsCollision(aabb1, aabb2)) {
            // 自キャラ衝突時コールバックを呼び出す
            player_->OnCollision(enemy);
            // 敵弾の衝突時コールバックを呼び出す
            enemy->OnCollision(player_);
        }
    }

#pragma endregion

#pragma region // 自キャラとアイテムの当たり判定
    // 自キャラとアイテムの当たり判定
#pragma endregion

#pragma region // 自弾と敵キャラの当たり判定
// 自弾と敵キャラの当たり判定
#pragma endregion
}

void GameScene::Draw() {


    DrawGrid::Draw(camera_);
    // 天球の描画
    skyDome_->Draw();

    // 地形の描画
    stage_->Draw();

    // 自キャラの描画
    player_->Draw();

    // 敵キャラの描画
    for (Enemy* newEnemy : enemies_) {
        if (!newEnemy)
            // ガード節と呼ぶ。
            continue;
        newEnemy->Draw();
    }

    // デスパーティクルの描画処理
    if (deathParticles_) {
        deathParticles_->Draw();
    }
    uiManager_->Draw();
}
void GameScene::Debug()
{
    player_->Debug();
    if (ImGui::Button("ChangeCamera")) {
        // スペースキーを押すとデバッグカメラに切り替える
        isDebugCameraActive_ = isDebugCameraActive_ ? false : true;
    }
  
    DebugUI::CheckFlag(isDebugCameraActive_,"DebugCamera");
    //視点操作
    DebugUI::CheckCamera(camera_);
    uint32_t lightType = 0;
    DebugUI::CheckDirectionalLight(lightType);
}
;

GameScene::~GameScene() {
    delete player_;
    delete playerModel_;
    delete enemyModel_;
    delete skyDome_;
    delete skyDomeModel_;
    delete cameraController_;
    delete deathParticleModel_;
    // パーティクルモデルの解放

    // パーティクルの解放
    if (deathParticles_) {
        delete deathParticles_;
    }

#ifdef _DEBUG
    delete debugCamera_;
#endif

    // 敵delete
    for (Enemy* newEnemy : enemies_) {
        delete newEnemy;
    }

    // 敵のポインタの残骸を一掃
    enemies_.clear();
}