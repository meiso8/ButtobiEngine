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

const int winWidth = 1280;
const int winHeight = 720;

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
    deathParticles_ = new DeathParticles();
    // カメラ操作
    cameraController_ = std::make_unique <CameraController>();

    //UI操作
    uiManager_ = new UIManager();

    stage_ = std::make_unique <Stage>();

    // 平面のデバッグ描画の生成
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer = std::make_unique<PlaneRenderer>();
    }

    // OBBのデバッグ描画の生成
    for (auto& obbRenderer : obbRenderers_) {
        obbRenderer = std::make_unique<OBBRenderer>();
    }
};

void GameScene::Initialize() {

    // カメラの初期化
    camera_->Initialize(winWidth, winHeight, Camera::PERSPECTIVE);
#ifdef _DEBUG
    // デバッグカメラ
    debugCamera_->Initialize(winWidth, winHeight);
#endif

    currentCamera_ = camera_.get();

    Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };
    // 自キャラの初期化 //ここはmainCamera
    player_->Initialize(*camera_, playerPosition);

    skyDome_->Initialize();

    deathParticles_->Initialize(playerPosition);

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

#ifdef _DEBUG
	// 平面のデバッグ描画の生成と初期化
	for (auto &planeRenderer : planeRenderers_) {
		planeRenderer->Initialize();
	}
#endif // _DEBUG

	// 平面の初期化
	planes_[0] = std::make_unique<Plane>(Plane{ .normal{ 0.0f, 1.0f, 0.0f }, .distance = 50.0f });	// 上
	planes_[1] = std::make_unique<Plane>(Plane{ .normal{ 0.0f, -1.0f, 0.0f }, .distance = 0.0f });	// 下

#ifdef _DEBUG
	// OBBのデバッグ描画の生成と初期化
	for (auto &obbRenderer : obbRenderers_) {
		obbRenderer->Initialize();
	}
#endif // _DEBUG

	// OBBの初期化
	obbs_[0] = std::make_unique<OBB>(OBB{ .center{-80.0f, 20.0f, 0.0f}, .axis{}, .halfSizes{1.0f, 40.0f, 80.0f} });
	SetAxis({ 0.0f, 0.0f, std::numbers::pi_v<float> / 4.0f }, *obbs_[0]);

	obbs_[1] = std::make_unique<OBB>(OBB{ .center{80.0f, 20.0f, 0.0f}, .axis{}, .halfSizes{1.0f, 40.0f, 80.0f} });
	SetAxis({ 0.0f, 0.0f, -std::numbers::pi_v<float> / 4.0f }, *obbs_[1]);

	obbs_[2] = std::make_unique<OBB>(OBB{ .center{0.0f, 20.0f, -80.0f}, .axis{}, .halfSizes{80.0f, 40.0f, 1.0f} });
	SetAxis({ -std::numbers::pi_v<float> / 4.0f, 0.0f, 0.0f }, *obbs_[2]);

	obbs_[3] = std::make_unique<OBB>(OBB{ .center{0.0f, 20.0f, 80.0f}, .axis{}, .halfSizes{80.0f, 40.0f, 1.0f} });
	SetAxis({ std::numbers::pi_v<float> / 4.0f, 0.0f, 0.0f }, *obbs_[3]);

    //シーン切り替え時にエネミーを削除
    for (Enemy* newEnemy : enemies_) {
        delete newEnemy;
    }

    enemies_.clear();
};

void GameScene::Update() {
    // ここにインゲームの更新処理を書く
     //BGMを鳴らす
    Sound::PlayBGM(Sound::BGM1);

#ifdef _DEBUG
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    currentCamera_->EditTransform("CurrentCamera");

	// 平面の編集
	uint32_t planeCount = 0;
	for (size_t i = 0; i < planes_.size(); ++i) {
		EditPlane("plane" + std::to_string(planeCount), *planes_[i]);
		planeRenderers_[i]->Update(*planes_[i]);
		planeCount++;
	}

	// OBBの編集
	uint32_t obbCount = 0;
	for (size_t i = 0; i < obbs_.size(); ++i) {
		EditOBB("OBB" + std::to_string(obbCount), obbRotates_[i], *obbs_[i]);
		obbRenderers_[i]->Update(*obbs_[i]);
		obbCount++;
	}
#endif // _DEBUG

	// 地形の更新処理
	stage_->Update();

	// 自キャラの更新処理
	player_->Update();

    // 敵キャラの更新処理
    PopEnemy();

    // 敵が死亡している場合は削除
    enemies_.remove_if([](Enemy* enemy) {
        if (enemy->IsDead()) {

            delete enemy; // メモリ解放
            return true;   // 削除する
        }
        return false; // 削除しない
        });

	uint32_t enemyCount = 0;
	for (Enemy *newEnemy : enemies_) {
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
    for (Enemy* enemy : enemies_) {
        collisionManager_->AddCollider(enemy);
    }

    // 衝突判定と応答
    collisionManager_->CheckAllCollisions();

#pragma region // 自キャラと敵キャラの当たり判定
	// 自キャラと敵キャラの当たり判定

	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	for (Enemy *enemy : enemies_) {
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			// 敵弾の衝突時コールバックを呼び出す
			enemy->OnCollision(player_.get());
			// 自キャラ衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
		}
	}

#pragma endregion

#pragma region // 敵キャラと平面の当たり判定
	for (Enemy *enemy : enemies_) {
		if (!enemy)
			continue;
		Sphere sphere = enemy->GetSphere();
		for (auto &plane : planes_) {
			if (IsCollision(sphere, *plane)) {
				enemy->OnCollision(*plane);
			}
		}
	}
#pragma endregion

#pragma region // 敵キャラとOBBの当たり判定
	for (Enemy *enemy : enemies_) {
		if (!enemy)
			continue;
		Sphere sphere = enemy->GetSphere();
		for (auto &obb : obbs_) {
			if (IsCollision(*obb, sphere)) {
				enemy->OnCollision(*obb);
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

    DrawGrid::Draw(*currentCamera_,false);
#endif // _DEBUG


	// 平面のデバッグ描画
	for (auto &planeRenderer : planeRenderers_) {
		planeRenderer->Draw(*currentCamera_);
	}

	// OBBのデバッグ描画
	for (auto &obbRenderer : obbRenderers_) {
		obbRenderer->Draw(*currentCamera_);
	}
#endif // _DEBUG

	// 天球の描画
	skyDome_->Draw(*currentCamera_);

	// 地形の描画
	//stage_->Draw(*currentCamera_);

	// 自キャラの描画
	player_->Draw(*currentCamera_);

	// 敵キャラの描画
	for (Enemy *newEnemy : enemies_) {
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
    uiManager_->Debug();
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    uint32_t lightType = 0;
    DebugUI::CheckDirectionalLight(lightType);

}
;

GameScene::~GameScene() {

	// パーティクルモデルの解放

	// パーティクルの解放
	if (deathParticles_) {
		delete deathParticles_;
	}

	// 敵delete
	for (Enemy *newEnemy : enemies_) {
		delete newEnemy;
	}

    // 敵のポインタの残骸を一掃
    enemies_.clear();
}

bool GameScene::GetIsEndScene() { return isEndScene_; }

bool GameScene::GetIsGameOver() { return isGameOver; }
bool GameScene::GetIsGameClear() { return isGameClear; }