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
	healItemSpawner_ = std::make_unique<HealItemSpawner>();
	actionUI_ = std::make_unique<ActionUI>(floorGamePlayer_.get());

    enemy_ = std::make_unique<Enemy>();
    enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
    enemyShotBulletManager_ = std::make_unique<EnemyShotBulletManager>(enemy_.get(), enemyBulletManager_.get());
    enemyBombManager_ = std::make_unique<EnemyBombManager>();
    enemyShotBombManager_ = std::make_unique<EnemyShotBombManager>(enemy_.get(), enemyBombManager_.get(), floorGameFloorManager_.get());
    enemyShockWaveManager_ = std::make_unique<EnemyShockWaveManager>();
    enemyShotWaveManager_ = std::make_unique<EnemyShotWaveManager>(enemy_.get(), enemyShockWaveManager_.get(), floorGameFloorManager_.get());

    uiManager_ = std::make_unique<UIManager>(*enemy_->GetHpsPtr(), *floorGamePlayer_->GetHpsPtr());

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter = std::make_unique<ParticleEmitter>();
    }

    particleEmitters_[kPlayerEmitter]->SetName("playerWalkParticle");
    particleEmitters_[kPlayerEmitter]->emitter_.transform.Parent(floorGamePlayer_->GetWorldBodyTransform());

    particleEmitters_[kEnemyEmitter]->SetName("enemyHitParticle");
    particleEmitters_[kEnemyEmitter]->emitter_.transform.Parent(enemy_->bodyPos_.worldTransform_);
#pragma endregion
}

void GameScene::Initialize() {

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
	healItemSpawner_->Initialize();
	actionUI_->Initialize();

	healItemSpawner_->SpawnHealItem({ 2.0f,1.0f,2.0f });

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

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->Initialize();
    }

    particleEmitters_[kPlayerEmitter]->emitter_.count = 10;
    particleEmitters_[kPlayerEmitter]->emitter_.movement = ParticleMovements::kParticleNormal;
    particleEmitters_[kPlayerEmitter]->emitter_.isRandomTranslate = true;
    particleEmitters_[kPlayerEmitter]->emitter_.isRandomRotate = false;
    particleEmitters_[kPlayerEmitter]->emitter_.frequency = 0.3f;
    particleEmitters_[kPlayerEmitter]->emitter_.blendMode = kBlendModeSubtract;
    particleEmitters_[kPlayerEmitter]->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };

    particleEmitters_[kEnemyEmitter]->emitter_.transform.translate_.y = -0.75f;
    particleEmitters_[kEnemyEmitter]->emitter_.count = 5;
    particleEmitters_[kEnemyEmitter]->emitter_.movement = ParticleMovements::kParticleSphere;
    particleEmitters_[kEnemyEmitter]->emitter_.radius = 2.0f;
    //particleEmitters_[kEnemyEmitter]->emitter_.isRandomRotate = false;
}

void GameScene::Update() {

    if (PauseScreen::isRetry || floorGamePlayer_->GetHpsPtr()->hp <= 0.0f) {
        PauseScreen::isRetry = false;
#ifdef _DEBUG
        Initialize();
#endif
        /* sceneChange_->SetState(SceneChange::kFadeIn, 30);*/
    }

    if (PauseScreen::isBackToTitle) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
    }

    //仮に音声を鳴らす　全体のvolumeがあってオフセット分だけいじる
    Sound::PlayBGM(Sound::BGM1, 0.0f);

    UpdateCamera();

    if (!PauseScreen::isPause_) {
        UpdateGameObject();
        CheckAllCollision();

        if (floorGamePlayer_->isMove_) {
            particleEmitters_[kPlayerEmitter]->UpdateTimer();
        } else {
            particleEmitters_[kPlayerEmitter]->InitTimer();
        }

        if (enemy_->IsHit()) {
            particleEmitters_[kEnemyEmitter]->Emit();
            /*  particleEmitters_[kEnemyEmitter]->UpdateTimer();*/

        }

        for (auto& particleEmitter : particleEmitters_) {
            particleEmitter->Update(*currentCamera_);
        }


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

#pragma endregion

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->Draw();
    }

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
    DebugUI::CheckParticle(*particleEmitters_[kEnemyEmitter], "Enemy");
    DebugUI::CheckParticle(*particleEmitters_[kPlayerEmitter], "Player");
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
	healItemSpawner_->Update();
    enemy_->Update();
    enemyBulletManager_->Update();
    enemyBombManager_->Update();
    enemyShockWaveManager_->Update();

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