#include "GameScene.h"
#include "AABB.h"
#include "Collision.h"
#include "RigidBody.h"
#include "Input.h"

#include "DebugUI.h"
#include "DrawGrid.h"

const int winWidth = 1280;
const int winHeight = 720;

GameScene::GameScene() = default;

void GameScene::Initialize() {
    // メンバ変数への代入処理

    DrawGrid::Initialize();

    // カメラの初期化
    camera_ = std::make_unique<Camera>();
    camera_->Initialize(winWidth, winHeight, Camera::PERSPECTIVE);

#ifdef _DEBUG
    // デバッグカメラの生成
    debugCamera_ = std::make_unique<DebugCamera>();
    debugCamera_->Initialize(winWidth, winHeight);
#endif

    currentCamera_ = camera_.get();

    // 自キャラの生成
    player_ = new Player();
    Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };
    // 自キャラの初期化 //ここはmainCamera
    player_->Initialize(*camera_, playerPosition);

    // 敵キャラ生成
    for (int32_t i = 0; i < kEnemyMax; ++i) {
        Enemy* newEnemy = new Enemy();
        Vector3 enemyPosition = { i * 5.0f + 10.0f, 10.0f, 0.0f };
        newEnemy->Initialize(enemyPosition);
        enemies_.push_back(newEnemy);
    }

    skyDome_ = new Skydome();
    // 天球の生成
    skyDome_->Initialize();

    // パーティクル
    deathParticles_ = new DeathParticles();
    deathParticles_->Initialize(playerPosition);

    // カメラ操作の初期化
    cameraController_ = new CameraController();
    cameraController_->Initialize(currentCamera_);
    cameraController_->SetTarget(player_);
    cameraController_->Reset();
    cameraController_->SetMovableArea({ 0.0f, 100.0f, 0.0f, 100.0f });

    //UI系
    uiManager_ = new UIManager();
    uiManager_->Initialize();

    // 地形
    stage_ = new Stage();
    stage_->Initialize();

    player_->InitializeLife(uiManager_->GetMaxLife());

    // 平面のデバッグ描画の生成
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer = std::make_unique<PlaneRenderer>();
    }

    // 平面の初期化
    planeRenderers_[0]->SetPlane({ { 1.0f, 0.0f, 0.0f }, 50.0f });  // 右
    planeRenderers_[1]->SetPlane({ { -1.0f, 0.0f, 0.0f }, 50.0f });	// 左
    planeRenderers_[2]->SetPlane({ { 0.0f, 1.0f, 0.0f }, 50.0f });  // 上
    planeRenderers_[3]->SetPlane({ { 0.0f, -1.0f, 0.0f }, 0.0f });  // 下
    planeRenderers_[4]->SetPlane({ { 0.0f, 0.0f, 1.0f }, 50.0f });  // 前
    planeRenderers_[5]->SetPlane({ { 0.0f, 0.0f, -1.0f }, 50.0f });	// 奥

    // 平面のデバッグ描画の初期化と更新
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer->Initialize();
    }
};

void GameScene::Update() {
    // ここにインゲームの更新処理を書く
#ifdef _DEBUG
    currentCamera_->EditTransform("CurrentCamera");
    uint32_t planeCount = 0;
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer->Edit("plane" + std::to_string(planeCount));
        planeCount++;
    }
#endif // _DEBUG

    // 平面の更新処理
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer->Update();
    }

    // 地形の更新処理
    stage_->Update();

    // 自キャラの更新処理
    player_->Update();

    // 敵キャラの更新処理
    uint32_t enemyCount = 0;
    for (Enemy* newEnemy : enemies_) {
        if (!newEnemy)
            // ガード節と呼ぶ。
            continue;
        newEnemy->Update();
#ifdef _DEBUG
        newEnemy->Edit("Enemy" + std::to_string(enemyCount));
#endif // _DEBUG
        enemyCount++;
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
        //camera_->viewMat_ = debugCamera_->viewMat_;
        //camera_->projectionMat_ = debugCamera_->projectionMat_;
    } else {
        // 行列更新
        cameraController_->Update();
    }

    currentCamera_->UpdateMatrix();

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

#pragma region // 敵キャラと平面の当たり判定
    for (Enemy* enemy : enemies_) {
        if (!enemy)
            continue;
        Sphere sphere = enemy->GetSphere();
        for (auto& planeRenderer : planeRenderers_) {
            Plane& plane = planeRenderer->GetPlane();
            if (IsCollision(sphere, plane)) {
                float e = 0.8f;                                                 // 反発係数
                float distance = Distance(sphere, plane);                       // 球の中心から平面までの距離
                float penetration = sphere.radius - distance;                   // 貫入量を球の半径に設定
                RigidBody* rigidBody = enemy->GetRigidBody();                   // フルーツの剛体
                Vector3 velocity = rigidBody->GetVelocity();                    // フルーツの速度
                Vector3 reflected = Reflect(velocity, plane.normal);            // 反射ベクトルの計算
                Vector3 projectNormal = Project(reflected, plane.normal);       // 法線方向の投影
                Vector3 movingDirection = reflected - projectNormal;            // 移動方向の計算
                rigidBody->SetVelocity(projectNormal * e + movingDirection);    // 衝突後のフルーツの速度を更新
                enemy->AddTranslate(projectNormal * penetration);               // 貫入量分フルーツの位置を修正
            }
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


    DrawGrid::Draw(*currentCamera_);
    // 天球の描画
    skyDome_->Draw(*currentCamera_);

    // 地形の描画
    stage_->Draw(*currentCamera_);

    // 平面のデバッグ描画
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer->Draw(*currentCamera_);
    }

    // 自キャラの描画
    player_->Draw(*currentCamera_);

    // 敵キャラの描画
    for (Enemy* newEnemy : enemies_) {
        if (!newEnemy)
            // ガード節と呼ぶ。
            continue;
        newEnemy->Draw(*currentCamera_);
    }

    //デスパーティクルの描画処理
    if (deathParticles_) {
        deathParticles_->Draw(*currentCamera_);
    }
    uiManager_->Draw();
}
void GameScene::Debug() {
    player_->Debug();

    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    uint32_t lightType = 0;
    DebugUI::CheckDirectionalLight(lightType);
}
;

GameScene::~GameScene() {
    delete player_;
    delete skyDome_;
    delete cameraController_;
    // パーティクルモデルの解放

    // パーティクルの解放
    if (deathParticles_) {
        delete deathParticles_;
    }

    // 敵delete
    for (Enemy* newEnemy : enemies_) {
        delete newEnemy;
    }

    // 敵のポインタの残骸を一掃
    enemies_.clear();
}

bool GameScene::GetIsEndScene() { return isEndScene_;}

bool GameScene::GetIsGameOver() { return isGameOver; }
bool GameScene::GetIsGameClear() { return isGameClear; }