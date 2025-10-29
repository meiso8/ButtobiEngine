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
#include"Shutter.h"
#include"Score.h"
#include"Effect.h"

#include"MakeMatrix.h"

constexpr int winWidth = 1280;
constexpr int winHeight = 720;

GameScene::GameScene() {

	camera_ = std::make_unique<Camera>();
#ifdef _DEBUG

    debugCamera_ = std::make_unique<DebugCamera>();
    lineMesh_ = std::make_unique<LineMesh>();
    lineMesh_->Create(Texture::GetHandle(Texture::WHITE_1X1));
#endif
	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();

	// 自キャラの生成
	player_ = std::make_unique<Player>();

	// 天球の生成
	skyDome_ = std::make_unique <Skydome>();

	//背景
	backGround_ = std::make_unique <BackGround>();
	backGround_->Initialize();

	// パーティクル
	CreateParticleMesh();


	// カメラ操作
	cameraController_ = std::make_unique <CameraController>();

	//UI操作
	uiManager_ = std::make_unique < UIManager>();

	stage_ = std::make_unique <Stage>();

	//力の矢印
	forceArrow_ = std::make_unique<Arrow>();
	//シャッターを作成
	shutter_ = std::make_unique<Shutter>();
	shutter_->Create();
}

GameScene::~GameScene() {

};
void GameScene::Initialize() {

	Sound::PlaySE(Sound::ANNOUNCE);
	isAnnounce_ = false;

	sceneChange_.Initialize();

	//シャッターの初期化
	shutter_->Initialize();

	//カメラの初期化
	InitializeCamera();

	// 衝突マネージャにスコアポインタを設定
	collisionManager_->SetScorePointer(&score_);

    collisionManager_->SetComboPointer(uiManager_->GetComboPointer());
    collisionManager_->SetComboTimerPtr(uiManager_->GetComboTimerPtr());
    collisionManager_->SetScorePointer(uiManager_->GetSpeedPointer());
    collisionManager_->SetIsScoreUp(uiManager_->GetScorePtr()->GetIsScoreUPPtr());
    collisionManager_->SetJuiceMeter(uiManager_->GetJuiceMeter());
    collisionManager_->SetIsComboSparkle(uiManager_->GetSparkleIsAlive(1));

	Vector3 playerPosition = { 0.0f, 10.0f, 0.0f };
	// 自キャラの初期化 //ここはmainCamera
	player_->Initialize(*camera_, playerPosition);

	skyDome_->Initialize();

	//UI系
	uiManager_->Initialize();

	// 地形
	stage_->Initialize();

	//力の矢印
	forceArrow_->Initialize();

	//敵を削除
	enemies_.clear();

	player_->InitializeLife(uiManager_->GetMaxLife());

	effect_->Initialize();

	isGameClear = false;
	isGameOver = false;

	chargeParticleColor_ = { 1.0f,0.0f,0.0f,1.0f };


};

void GameScene::CreateParticleMesh() {

	particle_ = std::make_unique<ChargeParticle>();
	particle_->Create(Texture::GetHandle(Texture::PARTICLE));
	particle_->useBillboard_ = true;
	particle_->emitter_.cont = 10;

	flashParticle_ = std::make_unique<FlashParticle>();
	flashParticle_->Create(Texture::GetHandle(Texture::FLASH_PARTICLE));
	flashParticle_->useBillboard_ = true;
	flashParticle_->emitter_.cont = 1;

	crashParticle_ = std::make_unique<AppleCrashParticle>();
	crashParticle_->Create(Texture::GetHandle(Texture::FLASH_PARTICLE));
	crashParticle_->useBillboard_ = true;
	crashParticle_->emitter_.cont = 10;

	//パーティクルをセットする
	player_->SetParticle(flashParticle_.get());

	//エフェクト
	effect_ = std::make_unique <Effect>(player_->GetWorldTransform());

}

void GameScene::UpdateParticle() {

	//プレイヤーがチャージしているときだけ更新
	if (player_->IsCharge()) {

        if (player_->GetChargeTimer() == player_->kMaxChargeTime) {
            chargeParticleColor_ = { 1.0f,1.0f,0.0f,1.0f };
        } else {
            chargeParticleColor_ = { 0.0f,1.0f,1.0f,1.0f };
        }
        particle_->TimerUpdate(true, { 0.1f,0.1f,0.1f }, chargeParticleColor_);

		particle_->emitter_.transform.translate = player_->GetWorldPosition();
	}

	if (player_->IsCharge() || player_->IsAttack()) {
		effect_->Update(player_->IsCharge(), chargeParticleColor_);
	}
	particle_->Update(*currentCamera_);
	flashParticle_->Update(*currentCamera_);
	crashParticle_->Update(*currentCamera_);
}

void GameScene::UpdateSceneChange() {

	if (!sceneChange_.isSceneStart_) {
		sceneChange_.UpdateStart(60);
	}

    if (!isAnnounce_) {
        if (!Sound::IsPlaying(Sound::ANNOUNCE)) {
            Sound::PlaySE(Sound::ANNOUNCE_FRUIT, 0.25f);
            isAnnounce_ = true;
            for (int i = 0; i < 10; ++i) {
                PopEnemy();
            }
        }
    }

    if (uiManager_->GetTimer() <= 0.0f) {
        if (!isGameClear) {
            Sound::PlayOriginSE(Sound::ALARM);
            isGameClear = true;
        }
    }

    if (player_->GetLife() <= 0) {
        if (!isGameOver) {
            isGameOver = true;
        }
    }

    if (isGameClear || isGameOver) {
        player_->SetPhase(Player::kTimeUp);
        Sound::Stop(Sound::CHARGE);
        Sound::Stop(Sound::ANNOUNCE_FRUIT);
        //この時カメラが動く
    }

    //カメラがズームし終わった時
    if (cameraController_->zoomEnd_) {
        sceneChange_.UpdateEnd(120);
        //シャッターを閉める
        shutter_->Close(sceneChange_.endTimer_ * InverseFPS);
    }
}

void GameScene::InitializeCamera() {
	// カメラの初期化
	camera_->Initialize(winWidth, winHeight, Camera::PERSPECTIVE);
	cameraShakeTimer_ = 30;
	isShakeCamera_ = false;

#ifdef _DEBUG
	// デバッグカメラ
	debugCamera_->Initialize(winWidth, winHeight);
#endif

	currentCamera_ = camera_.get();

	// カメラ操作の初期化
	cameraController_->Initialize(camera_.get());
	cameraController_->SetTarget(player_.get());
	cameraController_->Reset();
	/*  cameraController_->SetMovableArea({ 0.0f, 100.0f, 0.0f, 100.0f });*/

}

void GameScene::Update() {
	// ここにインゲームの更新処理を書く
	 //BGMを鳴らす
	Sound::PlayBGM(Sound::BGM1);

	UpdateSceneChange();

    // 地形の更新処理
    stage_->Update();

    for (uint32_t i = 0; i < 4; ++i) {
        if (player_->IsCharge()) {
            stage_->IsSetAlpha(false, i);
        } else {
            stage_->IsSetAlpha(true, i);
        }
    }

    //プレイヤーの操作 シーン切り替え時や判定完了時はしない
    if (sceneChange_.isSceneStart_&&!isGameClear && !isGameOver) {
        player_->InputMove();
        player_->InputAttack();

    } else {
        player_->ShutterCloseAnimation(sceneChange_.endTimer_ * InverseFPS);
    }

	// 自キャラの更新処理
	player_->Update();

	if (player_->IsAttack()) {
		//アタックしているときコンボタイマーをカウントダウンする
		uiManager_->SetIsUpdateComboTimer(true);
	}

	// 敵が死亡している場合は削除
	enemies_.remove_if([](const std::unique_ptr<Enemy> &enemy) { return enemy->IsDead(); });

    // 敵キャラの更新処理
    PopEnemyWait();

	uint32_t enemyCount = 0;
	for (auto &newEnemy : enemies_) {
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

	//力の矢印
	forceArrow_->Update(player_->GetWorldPosition(), player_->GetChargeTimer(), player_->GetWorldTransform().rotate_.y);

	//パーティクルの更新処理
	UpdateParticle();

	// 天球の更新処理
	skyDome_->Update();
	//カメラの更新処理
	UpdateCamera();

	// 全ての当たり判定を行う
	CheckAllCollisions();

	uiManager_->Update();
};

void GameScene::CheckAllCollisions() {
	// 衝突マネージャのコライダーをクリア
	collisionManager_->ClearColliders();

	// コライダーをリストに登録
	for (auto &enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();

#pragma region // 自機HP　と敵キャラの当たり判定

    if (!isGameClear && !isGameOver) {
        for (auto& enemy : enemies_) {
            // OBBとSphereの当たり判定
            if (IsCollision(player_->GetHPSphere(), enemy->GetSphere())) {

                //カメラを動かす
                if (!isShakeCamera_) {
                    isShakeCamera_ = true;
                    cameraShakeTimer_ = 0;
                }

                // 自キャラ衝突時コールバックを呼び出す
                player_->OnCollisionHP(enemy.get());
            }
        }
    }

#pragma region // 矢印と敵キャラの当たり判定
	// 矢印キャラと敵キャラの当たり判定
	OBB arrowOBB = forceArrow_->GetKickAreaOBB();
	for (auto &enemy : enemies_) {
		if (!enemy)
			continue;
		// OBBとSphereの当たり判定
		Capsule enemyCapsule = enemy->GetCapsule();
		if (IsCollision(enemyCapsule, arrowOBB)) {
			// 敵弾の衝突時コールバックを呼び出す
			enemy->OnCollision(player_.get());
			// 自キャラ衝突時コールバックを呼び出す
			player_->OnCollision(enemy.get());
		}
	}

#pragma endregion



#pragma region // 自キャラと平面の当たり判定
	Sphere playerSphere = player_->GetSphere();
	for (uint32_t i = 0; i < Stage::kMaxPlane; i++) {
		Plane stagePlane = stage_->GetPlane(i);
		if (IsCollision(playerSphere, stagePlane)) {
			player_->OnCollision(stagePlane);
		}
	}
#pragma endregion

#pragma region // 自キャラとOBBの当たり判定
    for (uint32_t i = 0; i < Stage::kMaxOBB; i++) {
        OBB stageOBB = stage_->GetOBB(i);
        if (IsCollision(playerSphere, stageOBB)) {
            player_->OnCollision(stageOBB);
        }
    }

    if (player_->isHitOBB_) {
        //CameraとOBBの当たり判定
        for (uint32_t i = 0; i < Stage::kMaxOBB; i++) {
            OBB stageOBB = stage_->GetOBB(i);
            Sphere cameraSphere = { .center = camera_->GetWorldPos(),.radius = 5.0f };
            if (IsCollision(cameraSphere, stageOBB)) {
                stage_->IsSetAlpha(true, i);
            }
        }
    }

#pragma endregion

#pragma region // 敵キャラと平面の当たり判定
	for (auto &enemy : enemies_) {
		if (!enemy)
			continue;
		Capsule enemyCapsule = enemy->GetCapsule();
		for (uint32_t i = 0; i < Stage::kMaxPlane; i++) {
			Plane stagePlane = stage_->GetPlane(i);
			if (IsCollision(enemyCapsule, stagePlane)) {
				enemy->OnCollision(stagePlane);
			}
		}
	}
#pragma endregion

#pragma region // 敵キャラとOBBの当たり判定
	for (auto &enemy : enemies_) {
		if (!enemy)
			continue;
		Capsule enemyCapsule = enemy->GetCapsule();
		for (uint32_t i = 0; i < Stage::kMaxOBB; i++) {
			OBB stageOBB = stage_->GetOBB(i);
			if (IsCollision(enemyCapsule, stageOBB)) {
				enemy->OnCollision(stageOBB);
			}
		}
	}
#pragma endregion

	for (auto &enemy : enemies_) {
		if (!enemy)
			continue;
		enemy->CollisionUpdate();
	}
}

void GameScene::PopEnemy() {
    // 敵の出現処理
    auto newEnemy = std::make_unique<Enemy>();
    Random::SetMinMax(-20.0f, 20.0f);
    std::array<Vector3, 4> enemyPositions = {
        Vector3{ -40.0f, 20.0f, Random::Get() },
        Vector3{ 40.0f, 20.0f, Random::Get() },
        Vector3{ Random::Get(), 20.0f, -40.0f },
        Vector3{ Random::Get(), 20.0f, 40.0f }
    };
    Random::SetMinMax(0.0f, 4.0f);
    newEnemy->Initialize(enemyPositions[static_cast<uint32_t>(Random::Get())]);
    newEnemy->SetCrashParticlePtr(crashParticle_.get());
    newEnemy->SetFlashParticlePtr(flashParticle_.get());

    enemies_.emplace_back(std::move(newEnemy));

}

void GameScene::PopEnemyWait()

{    // 待機処理
    if (isWaitingToPop_) {
        waitToPopTimer_--;
        if (waitToPopTimer_ <= 0) {
            isWaitingToPop_ = false;	// 待機完了
        }
        return;
    }

    PopEnemy();
    isWaitingToPop_ = true;
    waitToPopTimer_ = 60;
}

void GameScene::UpdateCamera() {
	if (isShakeCamera_) {

		cameraShakeTimer_++;

		if (cameraShakeTimer_ % 2 == 0) {
			Random::SetMinMax(-0.25f, 0.25f);
			camera_->offset_ = { Random::Get() ,Random::Get() };
		}

		if (cameraShakeTimer_ > 30) {
			isShakeCamera_ = false;
		}
	} else {
		camera_->offset_ = { 0.0f,0.0f };
	}

	// カメラの処理
	if (!isDebugCameraActive_) {
        if (isGameClear || isGameOver) {
            cameraController_->ZoomIn();
        } else {
            // 行列更新
            cameraController_->Update();
        }
    } else {
		currentCamera_->UpdateMatrix();
	}

}



void GameScene::Draw() {
#ifdef _DEBUG
    // グリッドの描画
    DrawGrid::Draw(*currentCamera_, false);
    lineMesh_->PreDraw();
    lineMesh_->Draw(*currentCamera_, MakeIdentity4x4());
#endif // _DEBUG

	// 天球の描画
	skyDome_->Draw(*currentCamera_);

    backGround_->Draw(*currentCamera_);

    // 地形の描画
    stage_->DrawPlane(*currentCamera_);
    // 自キャラの描画
    player_->Draw(*currentCamera_);

    //敵キャラの描画
    for (auto& newEnemy : enemies_) {
        if (!newEnemy)
            // ガード節と呼ぶ。
            continue;
        newEnemy->Draw(*currentCamera_);
    }

    if (player_->IsCharge() || player_->IsAttack()) {
        //パーティクルを描画
        particle_->Draw(kBlendModeAdd);
        effect_->Draw(*currentCamera_);
        //力を描画
        forceArrow_->Draw(*currentCamera_);
    }
  
	crashParticle_->Draw(kBlendModeNormal);

	flashParticle_->Draw(kBlendModeNormal);
    // 地形の描画
    stage_->DrawOBB(*currentCamera_);

	//UI
	uiManager_->Draw();

	//シャッターの描画処理
	if (cameraController_->zoomEnd_) {
		shutter_->Draw();
	}



}

void GameScene::Debug() {
#ifdef _DEBUG

    lineMesh_->SetVertexData(player_->GetWorldPosition(), forceArrow_->GetKickAreaOBB().center);


    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Score: %u", score_);

    player_->Debug();
    uiManager_->Debug();
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);

    uint32_t lightType = 0;
    DebugUI::CheckDirectionalLight(lightType);
    DebugUI::CheckParticle(*particle_, "chargeParticles");
    DebugUI::CheckParticle(*flashParticle_, "flashParticle");
#endif // _DEBUG
}

bool GameScene::GetIsGameOver() { return isGameOver; }
bool GameScene::GetIsGameClear() { return isGameClear; }