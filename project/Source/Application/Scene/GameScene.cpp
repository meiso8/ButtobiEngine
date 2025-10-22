#include "GameScene.h"
#include "AABB.h"
#include "OBB.h"
#include "CollisionManager.h"
#include "Collision.h"
#include "RigidBody.h"
#include "Random.h"
#include "Input.h"

#include "DebugUI.h"
#include "DrawGrid.h"
#include"Sound.h"
#include"Texture.h"

constexpr int winWidth = 1280;
constexpr int winHeight = 720;

GameScene::GameScene() {

    camera_ = std::make_unique<Camera>();
#ifdef _DEBUG
    DrawGrid::Initialize();
    debugCamera_ = std::make_unique<DebugCamera>();
#endif
    // 衝突マネージャの生成
    collisionManager_ = std::make_unique<CollisionManager>();

    // 自キャラの生成
    player_ = std::make_unique<Player>();


    // 天球の生成
    skyDome_ = std::make_unique <Skydome>();
    // パーティクル
    deathParticles_ = std::make_unique < DeathParticles>();
    // カメラ操作
    cameraController_ = std::make_unique <CameraController>();

    //UI操作
    uiManager_ = std::make_unique < UIManager>();

    stage_ = std::make_unique <Stage>();
}

GameScene::~GameScene() = default;
void GameScene::Initialize() {

    isEndScene_ = false;

    // カメラの初期化
    camera_->Initialize(winWidth, winHeight, Camera::PERSPECTIVE);
#ifdef _DEBUG
    // デバッグカメラ
    debugCamera_->Initialize(winWidth, winHeight);
#endif

    currentCamera_ = camera_.get();

    // 衝突マネージャにスコアポインタを設定
    collisionManager_->SetScorePointer(&score_);
    collisionManager_->SetComboPointer(uiManager_->GetComboPointer());
    collisionManager_->SetComboTimerPtr(uiManager_->GetComboTimerPtr());


    Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };
    // 自キャラの初期化 //ここはmainCamera
    player_->Initialize(*camera_, playerPosition);

    skyDome_->Initialize();

    deathParticles_->Initialize(playerPosition);

    particle_.Initialize(Texture::GetHandle(Texture::PARTICLE));
    particle_.useBillboard_ = true;
    particle_.emitter_.cont = 1;

    // カメラ操作の初期化
    cameraController_->Initialize(camera_.get());
    cameraController_->SetTarget(player_.get());
    cameraController_->Reset();
    cameraController_->SetMovableArea({ 0.0f, 100.0f, 0.0f, 100.0f });

    //UI系
    uiManager_->Initialize();

    // 地形
    stage_->Initialize();

    player_->InitializeLife(uiManager_->GetMaxLife());
};

void GameScene::Update() {
    // ここにインゲームの更新処理を書く
     //BGMを鳴らす
    Sound::PlayBGM(Sound::BGM1);

#ifdef _DEBUG
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    currentCamera_->EditTransform("CurrentCamera");
    ImGui::Text("Score: %u", score_);
#endif // _DEBUG

    // 地形の更新処理
    stage_->Update();

    // 自キャラの更新処理
    player_->Update();
    if (player_->IsAttack()) {
        //アタックしているときコンボタイマーをカウントダウンする
        uiManager_->SetIsUpdateComboTimer(true);
    }

    // 敵が死亡している場合は削除
    enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });

    // 敵キャラの更新処理
    PopEnemy();

    uint32_t enemyCount = 0;
    for (auto& newEnemy : enemies_) {
        if (!newEnemy)
            // ガード節と呼ぶ。
            continue;
#ifdef _DEBUG
        newEnemy->Edit("Enemy" + std::to_string(enemyCount));
#endif // _DEBUG
        newEnemy->Update();
        enemyCount++;
    }

    uiManager_->SetLife(player_->GetLife());

    // デスパーティクルの更新処理
    if (deathParticles_) {
        deathParticles_->Update();
    }

    //プレイヤーがチャージしているときだけ更新
    //if (player_->IsCharge()) {
    //    particle_.EmitterTimerUpdate({ 1.0f,1.0f,0.0f,1.0f });
    //    particle_.emitter_.transform.translate = player_->GetWorldPosition();
    //}

    //particle_.Update(*currentCamera_);

    // 天球の更新処理
    skyDome_->Update();

    // カメラの処理
    if (!isDebugCameraActive_) {
        // 行列更新
        cameraController_->Update();
    }

    currentCamera_->UpdateMatrix();

    // 全ての当たり判定を行う
    CheckAllCollisions();
    uiManager_->Update();
};

void GameScene::CheckAllCollisions() {
    // 衝突マネージャのコライダーをクリア
    collisionManager_->ClearColliders();

    // コライダーをリストに登録
    for (auto& enemy : enemies_) {
        collisionManager_->AddCollider(enemy.get());
    }

    // 衝突判定と応答
    collisionManager_->CheckAllCollisions();

#pragma region // 自キャラと敵キャラの当たり判定
    // 自キャラと敵キャラの当たり判定

    AABB aabb1, aabb2;

    aabb1 = player_->GetAABB();

    for (auto& enemy : enemies_) {
        aabb2 = enemy->GetAABB();

        // AABB同士の交差判定
        if (IsCollision(aabb1, aabb2)) {
            // 敵弾の衝突時コールバックを呼び出す
            enemy->OnCollision(player_.get());
            // 自キャラ衝突時コールバックを呼び出す
            player_->OnCollision(enemy.get());
        }
    }

#pragma endregion

#pragma region // 敵キャラと平面の当たり判定
    for (auto& enemy : enemies_) {
        if (!enemy)
            continue;
        Sphere sphere = enemy->GetSphere();
        for (uint32_t i = 0; i < Stage::kMaxPlane; i++) {
            if (IsCollision(sphere, stage_->GetPlane(i))) {
                enemy->OnCollision(stage_->GetPlane(i));
            }
        }
    }
#pragma endregion

#pragma region // 敵キャラとOBBの当たり判定
    for (auto& enemy : enemies_) {
        if (!enemy)
            continue;
        Sphere sphere = enemy->GetSphere();
        for (uint32_t i = 0; i < Stage::kMaxOBB; i++) {
            if (IsCollision(stage_->GetOBB(i), sphere)) {
                enemy->OnCollision(stage_->GetOBB(i));
            }
        }
    }
#pragma endregion
}

void GameScene::PopEnemy() {
    // 待機処理
    if (isWaitingToPop_) {
        waitToPopTimer_--;
        if (waitToPopTimer_ <= 0) {
            isWaitingToPop_ = false;	// 待機完了
        }
        return;
    }

    // 敵の出現処理
    Enemy* newEnemy = new Enemy();
    Random::SetMinMax(-40.0f, 40.0f);
    std::array<Vector3, 4> enemyPositions = {
        Vector3{ -80.0f, 40.0f, Random::Get() },
        Vector3{ 80.0f, 40.0f, Random::Get() },
        Vector3{ Random::Get(), 40.0f, -80.0f },
        Vector3{ Random::Get(), 40.0f, 80.0f }
    };
    Random::SetMinMax(0.0f, 4.0f);
    newEnemy->Initialize(enemyPositions[static_cast<uint32_t>(Random::Get())]);
    enemies_.emplace_back(newEnemy);
    isWaitingToPop_ = true;
    waitToPopTimer_ = 60;
}

void GameScene::Draw() {
#ifdef _DEBUG
    // グリッドの描画
    DrawGrid::Draw(*currentCamera_, false);
#endif // _DEBUG

    // 天球の描画
    skyDome_->Draw(*currentCamera_);

    // 地形の描画
    stage_->Draw(*currentCamera_);

    // 自キャラの描画
    player_->Draw(*currentCamera_);

    // 敵キャラの描画
    for (auto& newEnemy : enemies_) {
        if (!newEnemy)
            // ガード節と呼ぶ。
            continue;
        newEnemy->Draw(*currentCamera_);
    }

    //デスパーティクルの描画処理
    if (deathParticles_) {
        deathParticles_->Draw(*currentCamera_);
    }

    //プレイヤーがチャージしているときにパーティクルを描画
    if (player_->IsCharge()) {
        particle_.Draw(kBlendModeAdd);

    }


    uiManager_->Draw();
}
void GameScene::Debug() {
    player_->Debug();
    uiManager_->Debug();
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    uint32_t lightType = 0;
    DebugUI::CheckDirectionalLight(lightType);
    DebugUI::CheckParticle(particle_, "chargeParticles");
}

bool GameScene::GetIsEndScene() { return isEndScene_; }

bool GameScene::GetIsGameOver() { return isGameOver; }
bool GameScene::GetIsGameClear() { return isGameClear; }