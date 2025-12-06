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

#include"CollisionConfig.h"
#include"Lerp.h"

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
	gameOverEvent_ = std::make_unique<GameOverEvent>(floorGamePlayer_.get());
    actionUI_ = std::make_unique<ActionUI>(floorGamePlayer_.get());
	letterboxBars_ = std::make_unique<LetterboxBars>();

    enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
    enemyShotBulletManager_ = std::make_unique<EnemyShotBulletManager>(enemy_.get(), enemyBulletManager_.get());
    enemyBombManager_ = std::make_unique<EnemyBombManager>();
    enemyShotBombManager_ = std::make_unique<EnemyShotBombManager>(enemy_.get(), enemyBombManager_.get(), floorGameFloorManager_.get());
    enemyShockWaveManager_ = std::make_unique<EnemyShockWaveManager>();
    enemyShotWaveManager_ = std::make_unique<EnemyShotWaveManager>(enemy_.get(), enemyShockWaveManager_.get(), floorGameFloorManager_.get());

    tree_ = std::make_unique<Tree>();
    ground_ = std::make_unique<Ground>();
    nest_ = std::make_unique<Nest>();
#pragma endregion

    uiManager_ = std::make_unique<UIManager>(
        *enemy_->GetHpsPtr(), 
        *floorGamePlayer_->GetHpsPtr(),
        floorGamePlayer_->IsHit(),
        enemy_->IsHit(),
        enemy_->isKnockBack_
    );

    emitterManager_ = std::make_unique<EmitterManager>(*floorGamePlayer_, *enemy_,*enemyShockWaveManager_,*floorBulletManager_);
}

void GameScene::Initialize() {

    Sound::bgmVolume_ = bgmMaxVol_;
    Sound::Stop(Sound::titleBGM);
    isSoundGameOverBGM_ = false;

    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 90);

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
	letterboxBars_->Initialize();
	gameOverEvent_->Initialize();

    //healItemSpawner_->SpawnHealItem({ 2.0f,1.0f,2.0f });

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
    ground_->Init();
    nest_->Init();
#pragma endregion
    collisionManager_->ClearColliders();

    uiManager_->Initialize();

    emitterManager_->Initialize();

	gameclearTimer_ = 0.0f;
    gameOverTimer_ = 0.0f;
}

void GameScene::Update() {

    UpdateBGM();

    if (gameclearTimer_ > 2.0f) {
		SceneStaticValue::isClear = true;
		sceneChange_->SetState(SceneChange::kFadeIn, 30);
    }

    if (PauseScreen::isRetry) {
        PauseScreen::isRetry = false;
        Initialize();
    }

	if (gameOverEvent_->IsReqestedAction()) {

		if (gameOverEvent_->IsRetrySelected()) {
			Initialize();
			
        } else {
            sceneChange_->SetState(SceneChange::kFadeIn, 60);
        }
	}

    if (PauseScreen::isBackToTitle) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
    }


    if (!PauseScreen::isActive_) {
        UpdateCamera();
        UpdateGameObject();
        CheckAllCollision();
        //エミッター
        emitterManager_->Update(*currentCamera_);

    }

	letterboxBars_->Update();
    uiManager_->Update();

}

void GameScene::Draw() {

#ifdef _DEBUG
    DrawGrid::Draw(*currentCamera_);
#endif

#pragma region // オブジェクト描画
    floorGamePlayer_->Draw(*currentCamera_);
    floorGameFloorManager_->Draw(*currentCamera_);
    floorBulletManager_->Draw(*currentCamera_);
    floorPlayerStripTargetUI_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    playerFloorStripManager_->Draw(*currentCamera_);
    healItemSpawner_->Draw(*currentCamera_);
	floorActionManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
    actionUI_->Draw();
	letterboxBars_->Draw();
	gameOverEvent_->Draw();
    enemy_->Draw(*currentCamera_);
    enemyBulletManager_->Draw(*currentCamera_);
    enemyBombManager_->Draw(*currentCamera_);
    enemyShockWaveManager_->Draw(*currentCamera_);
   
    tree_->Draw(*currentCamera_);
    ground_->Draw(*currentCamera_);
    nest_->Draw(*currentCamera_);
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
        }

        if (enemy_->isFaseChange_) {
            cameraController_->FocusTarget(&enemy_->bodyPos_.worldTransform_.translate_);
        } else {
            cameraController_->ResetFocusTarget();
        }

        if (floorGamePlayer_->GetDamageStruct().hps.hp <= 0) {
			cameraController_->ResetFocusTarget();
            cameraController_->FocusTarget(&floorGamePlayer_->body_.worldTransform_.translate_);
        }

		if (enemy_->IsDead()) {
			cameraController_->StartEnemyLethal();
            if (enemy_->IsOverKill()) {
                cameraController_->ResetFocusTarget();
                cameraController_->FocusTarget(&enemy_->bodyPos_.worldTransform_.translate_);
            }
		}

        cameraController_->Update();
    }

}

void GameScene::UpdateGameObject()
{
    if (enemy_->IsDead()) {
        floorGameFloorManager_->ForceChangeAllFloorType(FloorType::Strong);
        floorGamePlayer_->SetCollisionAttribute(kCollisionNone);
    }

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
    ground_->Update();
    nest_->Update();

    if (floorGamePlayer_->IsDead()) {
        gameOverTimer_ += InverseFPS;
    }


    // オブジェクト同士の干渉
    actionUI_->Update();

    if (floorGamePlayer_->GetDamageStruct().hps.hp <= 0) {
        actionUI_->isHide_ = true;
    } else {
        actionUI_->isHide_ = false;
    }
    
    floorStripManager_->Update();
    playerFloorStripManager_->Update();
    floorPlayerShotBulletManager_->Update();
    enemyShotBulletManager_->Update();
    enemyShotBombManager_->Update();
    enemyShotWaveManager_->Update();
    floorPlayerStripTargetUI_->Update();
    floorActionManager_->Update();
    if (enemy_->isReqestClearFloor_) {
        Sound::PlaySE(Sound::kFloorRespawn);
        floorGameFloorManager_->Initialize();
        enemy_->isReqestClearFloor_ = false;
    }

    if (enemy_->IsOverKill()) {
        enemy_->LeathalMoveUpdate();
		gameclearTimer_ += 0.016f;
    }


    // アニメーション更新
    floorGamePlayerAnimationManager_->Update();

    if (IsGameOverEventUpdate()) {
        // イベントシステム
        gameOverEvent_->Update();
    }

    if (floorGamePlayer_->GetDamageStruct().hps.hp <= 0 || enemy_->isFaseChange_ || enemy_->IsOverKill()) {
        letterboxBars_->isOpen_ = true;
    } else {
		letterboxBars_->isOpen_ = false;
    }
}

void GameScene::UpdateBGM()
{

    if (IsGameOverBGMSound()) {
        if (!isSoundGameOverBGM_) {
            isSoundGameOverBGM_ = true;
            Sound::PlaySE(Sound::gameOverBGM);
        }
    }

    if (floorGamePlayer_->IsDead()|| SceneStaticValue::isClear) {
        Sound::Stop(Sound::inGameBGM01);
        Sound::Stop(Sound::inGameBGM02);
        Sound::Stop(Sound::playerHP1BGM);
    } else {

        if (floorGamePlayer_->GetHpsPtr()->hp <= 20.0f) {

            Sound::Stop(Sound::inGameBGM01);
            Sound::Stop(Sound::inGameBGM02);

            Sound::PlayBGM(Sound::playerHP1BGM);
        } else {

            Sound::Stop(Sound::playerHP1BGM);

            if (enemy_->GetCurrentState() != "Third") {
                Sound::Stop(Sound::inGameBGM02);
                Sound::PlayBGM(Sound::inGameBGM01);
            } else {
                Sound::Stop(Sound::inGameBGM01);
                //仮に音声を鳴らす　全体のvolumeがあってオフセット分だけいじる
                Sound::PlayBGM(Sound::inGameBGM02);
            }
        }
    }


    if (PauseScreen::isPause_) {
        Sound::bgmVolume_ = Lerp(Sound::bgmVolume_, 0.125f, 0.5f);
    } else {
        Sound::bgmVolume_ = Lerp(Sound::bgmVolume_, bgmMaxVol_, 0.5f);
    }


}

void GameScene::CheckAllCollision()
{

    collisionManager_->ClearColliders();

    if (!enemy_->IsDead()) {
        collisionManager_->AddCollider(floorGamePlayer_.get());
    }
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