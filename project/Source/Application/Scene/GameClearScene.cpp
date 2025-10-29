#include "GameClearScene.h"
#include "DebugUI.h"
#include "Enemy.h"
#include "../Stage.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "Random.h"
#include <array>
#include"Input.h"
#include"Sprite.h"
#include"Texture.h"
#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG
#include"Sound.h"
#include"Shutter.h"

constexpr int winWidth = 1280;
constexpr int winHeight = 720;

void GameClearScene::Initialize() {

	sceneChange_.Initialize();
	isStartSceneChange_ = false;
	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize(winWidth, winHeight, Camera::PERSPECTIVE);
	camera_->translate_ = { 0.0f, 10.0f, -100.0f };

#ifdef _DEBUG
	// デバッグカメラの初期化
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize(winWidth, winHeight);
#endif // _DEBUG

	// 現在のカメラを設定
	currentCamera_ = camera_.get();

	stage_ = std::make_unique<Stage>();
	stage_->Initialize();

	isRenderSprite_ = false;
	clearSprite_ = std::make_unique<Sprite>();
	clearSprite_->Create(Texture::GetHandle(Texture::CLEAR), { 640.0f - 480.0f,96.0f }, { 960.0f,192.0f });


	collisionManager_ = std::make_unique<CollisionManager>();

	//エネミーをクリアする
	enemies_.clear();
	subtractWaitToPopTimer_ = 60;

	score_->SetScorePos();
}
void GameClearScene::Update() {

	if (sceneChange_.isSceneStart_) {

		if (isStartSceneChange_) {
			//カウントアップする
			sceneChange_.UpdateEnd(120);
		}

		if (Input::IsTriggerKey(DIK_SPACE)) {
			//省略する
			isRenderSprite_ = true;
			isStartSceneChange_ = true;

			if (sceneChange_.endTimer_ < 60) {
				sceneChange_.endTimer_ = 60;
			}
		}

		EnemyUpdate();

		//減少時間が1になったら
		if (subtractWaitToPopTimer_ == 1) {
			isRenderSprite_ = true;
			Sound::PlayOriginSE(Sound::YEAH);
			isStartSceneChange_ = true;
		}

	} else {

		sceneChange_.UpdateStart(60);
		if (shutter_) {
			shutter_->Open(sceneChange_.startTimer_ * InverseFPS);
		}
	}

	currentCamera_->UpdateMatrix();
	stage_->Update();

}

void GameClearScene::EnemyUpdate() {
	// 敵が死亡している場合は削除
	enemies_.remove_if([](const std::unique_ptr<Enemy> &enemy) { return enemy->IsDead(); });

	PopEnemy();

	for (auto &enemy : enemies_) {
		enemy->Update();
	}

	CheckAllCollisions();

}

void GameClearScene::Draw() {
	for (auto &enemy : enemies_) {
		enemy->Draw(*currentCamera_);
	}
	stage_->Draw(*currentCamera_);

	clearSprite_->PreDraw();

	if (isRenderSprite_) {
		clearSprite_->Draw();
		score_->Draw();
	}

	if (shutter_) {
		shutter_->Draw();
	}

}

void GameClearScene::Debug() {
#ifdef _DEBUG
	ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
	currentCamera_->EditTransform("CurrentCamera");
	DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
	std::function<void()> func = [this]() { SwitchCamera(); };
	DebugUI::Button("ChangeCamera", func);
	ImGui::Text("Score: %u", score_);
	uint32_t enemyCount = 0;
	for (auto &enemy : enemies_) {
		enemy->Edit("Enemy" + std::to_string(enemyCount));
		enemyCount++;
	}
#endif // _DEBUG
}

void GameClearScene::CheckAllCollisions() {
	// 衝突マネージャのコライダーをクリア
	collisionManager_->ClearColliders();

	// コライダーをリストに登録
	for (auto &enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();

#pragma region // 敵キャラと平面の当たり判定
	for (auto &enemy : enemies_) {
		if (!enemy) continue;
		Capsule capsule = enemy->GetCapsule();
		for (uint32_t i = 0; i < Stage::kMaxPlane; i++) {
			Plane plane = stage_->GetPlane(i);
			if (IsCollision(capsule, plane)) {
				enemy->OnCollision(plane);
			}
		}
	}
#pragma endregion

#pragma region // 敵キャラとOBBの当たり判定
	for (auto &enemy : enemies_) {
		if (!enemy) continue;
		Capsule capsule = enemy->GetCapsule();
		for (uint32_t i = 0; i < Stage::kMaxOBB; i++) {
			OBB obb = stage_->GetOBB(i);
			if (IsCollision(capsule, obb)) {
				enemy->OnCollision(obb);
			}
		}
	}
#pragma endregion

	for (auto &enemy : enemies_) {
		if (!enemy) continue;
		enemy->CollisionUpdate();
	}
}

void GameClearScene::PopEnemy() {
	// 待機処理
	if (isWaitingToPop_) {
		waitToPopTimer_ -= subtractWaitToPopTimer_;
		if (waitToPopTimer_ <= 0) {
			isWaitingToPop_ = false;	// 待機完了
			//音を鳴らす
			Sound::PlaySE(Sound::CRACKER);
		}
		return;
	}

	// 敵の出現処理
	if (subtractWaitToPopTimer_ > 0) {
		subtractWaitToPopTimer_ -= 1;
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		Random::SetMinMax(-40.0f, 40.0f);
		std::array<Vector3, 4> enemyPositions = {
			Vector3{ -80.0f, 40.0f, Random::Get() },
			Vector3{ 80.0f, 40.0f, Random::Get() },
			Vector3{ Random::Get(), 40.0f, -80.0f },
			Vector3{ Random::Get(), 40.0f, 80.0f }
		};
		Random::SetMinMax(0.0f, 4.0f);
		newEnemy->Initialize(enemyPositions[static_cast<uint32_t>(Random::Get())]);
		enemies_.emplace_back(std::move(newEnemy));
		isWaitingToPop_ = true;
		waitToPopTimer_ = 60;
	}

}

GameClearScene::GameClearScene() = default;
GameClearScene::~GameClearScene() = default;