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
#include"SoundManager.h"
#include"Sound.h"
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

	  // 衝突マネージャの生成
	  collisionManager_ = std::make_unique<CollisionManager>();

    // 自キャラの生成
    player_ = std::make_unique<Player>();
    Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };
    // 自キャラの初期化 //ここはmainCamera
    player_->Initialize(*camera_, playerPosition);
  
    // 天球の生成
    skyDome_ = std::make_unique <Skydome>();
    skyDome_->Initialize();

    // パーティクル
    deathParticles_ = new DeathParticles();
    deathParticles_->Initialize(playerPosition);

    // カメラ操作の初期化
    cameraController_ = std::make_unique <CameraController>();
    cameraController_->Initialize(camera_.get());
    cameraController_->SetTarget(player_.get());
    cameraController_->Reset();
    cameraController_->SetMovableArea({ 0.0f, 100.0f, 0.0f, 100.0f });

    //UI系
    uiManager_ = new UIManager();
    uiManager_->Initialize();

    // 地形
    stage_ = std::make_unique <Stage>();
    stage_->Initialize();

    player_->InitializeLife(uiManager_->GetMaxLife());

    // 平面のデバッグ描画の生成
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer = std::make_unique<PlaneRenderer>();
    }

	  // 平面の初期化
	  planeRenderers_[0]->SetPlane({ { 0.0f, 1.0f, 0.0f }, 50.0f });  // 上
	  planeRenderers_[1]->SetPlane({ { 0.0f, -1.0f, 0.0f }, 0.0f });  // 下

    // 平面のデバッグ描画の初期化
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer->Initialize();
    }
  
	  // OBBのデバッグ描画の生成
	  for (auto &obbRenderer : obbRenderers_) {
		  obbRenderer = std::make_unique<OBBRenderer>();
	  }

	  // OBBの初期化
	  obbRenderers_[0]->SetCenter({ -80.0f, 20.0f, 0.0f });
	  obbRenderers_[0]->SetAxis({ 0.0f, 0.0f, std::numbers::pi_v<float> / 4.0f });
	  obbRenderers_[0]->SetHalfSizes({ 1.0f, 40.0f, 80.0f });
	  obbRenderers_[1]->SetCenter({ 80.0f, 20.0f, 0.0f });
	  obbRenderers_[1]->SetAxis({ 0.0f, 0.0f, -std::numbers::pi_v<float> / 4.0f });
	  obbRenderers_[1]->SetHalfSizes({ 1.0f, 40.0f, 80.0f });
	  obbRenderers_[2]->SetCenter({ 0.0f, 20.0f, -80.0f });
	  obbRenderers_[2]->SetAxis({ -std::numbers::pi_v<float> / 4.0f, 0.0f, 0.0f });
	  obbRenderers_[2]->SetHalfSizes({ 80.0f, 40.0f, 1.0f });
	  obbRenderers_[3]->SetCenter({ 0.0f, 20.0f, 80.0f });
	  obbRenderers_[3]->SetAxis({ std::numbers::pi_v<float> / 4.0f, 0.0f, 0.0f });
	  obbRenderers_[3]->SetHalfSizes({ 80.0f, 40.0f, 1.0f });

	  // OBBのデバッグ描画の初期化
	  for (auto &obbRenderer : obbRenderers_) {
		  obbRenderer->Initialize();
	  }
};

void GameScene::Update() {
    // ここにインゲームの更新処理を書く
     //BGMを鳴らす
    Sound::PlayBGM(Sound::BGM1,-0.5f);
  
#ifdef _DEBUG
	ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
	currentCamera_->EditTransform("CurrentCamera");

	// 平面の編集
	uint32_t planeCount = 0;
	for (auto &planeRenderer : planeRenderers_) {
		planeRenderer->Edit("plane" + std::to_string(planeCount));
		planeCount++;
	}

	// OBBの編集
	uint32_t obbCount = 0;
	for (auto &obbRenderer : obbRenderers_) {
		obbRenderer->Edit("OBB" + std::to_string(obbCount));
		obbCount++;
	}
#endif // _DEBUG

  // 平面の更新処理
  for (auto& planeRenderer : planeRenderers_) {
    planeRenderer->Update();
  }

	// OBBの更新処理
	for (auto &obbRenderer : obbRenderers_) {
		obbRenderer->Update();
	}

	// 地形の更新処理
	stage_->Update();

  // 自キャラの更新処理
  player_->Update();

	// 敵キャラの更新処理
	PopEnemy();

	// 敵が死亡している場合は削除
	enemies_.remove_if([](Enemy *enemy) {
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
	for (Enemy *enemy : enemies_) {
		collisionManager_->AddCollider(enemy);
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();

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
            enemy->OnCollision(player_.get());
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

#pragma region // 敵キャラとOBBの当たり判定
	for (Enemy *enemy : enemies_) {
		if (!enemy)
			continue;
		Sphere sphere = enemy->GetSphere();
		for (auto &obbRenderer : obbRenderers_) {
			OBB &obb = obbRenderer->GetOBB();
			if (IsCollision(obb, sphere)) {
				float e = 0.8f;                                                 // 反発係数
				float distance = Distance(sphere, obb);							// 球の中心から平面までの距離
				float penetration = sphere.radius - distance;                   // 球の半径と距離の差分を貫入量に設定
				RigidBody *rigidBody = enemy->GetRigidBody();                   // フルーツの剛体
				Vector3 velocity = rigidBody->GetVelocity();                    // フルーツの速度
				Vector3 closestPoint = ClosestPoint(sphere.center, obb);		// 球の中心から最も近い点を取得
				Vector3 normal = Normalize(sphere.center - closestPoint);		// 法線ベクトルの計算
				Vector3 reflected = Reflect(velocity, normal);					// 反射ベクトルの計算
				Vector3 projectNormal = Project(reflected, normal);				// 法線方向の投影
				Vector3 movingDirection = reflected - projectNormal;            // 移動方向の計算
				rigidBody->SetVelocity(projectNormal * e + movingDirection);	// 衝突後のフルーツの速度を更新
				enemy->AddTranslate(projectNormal * penetration);               // 貫入量分フルーツの位置を修正
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
	Enemy *newEnemy = new Enemy();
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


    DrawGrid::Draw(*currentCamera_);
    // 天球の描画
    skyDome_->Draw(*currentCamera_);

	  // 地形の描画
	  //stage_->Draw(*currentCamera_);

    // 平面のデバッグ描画
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer->Draw(*currentCamera_);
    }

	  // OBBのデバッグ描画
	  for (auto &obbRenderer : obbRenderers_) {
		  obbRenderer->Draw(*currentCamera_);
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

bool GameScene::GetIsEndScene() { return isEndScene_; }

bool GameScene::GetIsGameOver() { return isGameOver; }
bool GameScene::GetIsGameClear() { return isGameClear; }