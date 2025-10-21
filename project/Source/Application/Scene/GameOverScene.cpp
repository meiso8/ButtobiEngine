#include "GameOverScene.h"
#include "DebugUI.h"
#include "Enemy.h"
#include "../Stage.h"
#include "Collision.h"
#include "Random.h"
#include <array>

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG

constexpr int winWidth = 1280;
constexpr int winHeight = 720;

void GameOverScene::Initialize() {
	isEndScene_ = false;

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

	// 敵の初期化
	enemy_ = std::make_unique<Enemy>();
	Random::SetMinMax(0.0f, 40.0f);
	std::array<Vector3, 2> enemyPositions = {
		Vector3{ -80.0f, 40.0f, Random::Get() },
		Vector3{ 80.0f, 40.0f, Random::Get() }
	};
	Random::SetMinMax(0.0f, 1.0f);
	enemy_->Initialize(enemyPositions[static_cast<uint32_t>(Random::Get())]);

	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
}

void GameOverScene::Update() {
	isEndScene_ = true;

#ifdef _DEBUG
	ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
	currentCamera_->EditTransform("CurrentCamera");
	DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
	std::function<void()> func = [this]() { SwitchCamera(); };
	DebugUI::Button("ChangeCamera", func);
	ImGui::Text("Score: %u", score_);
	enemy_->Edit("Enemy");
#endif // _DEBUG

	currentCamera_->UpdateMatrix();

	enemy_->Update();
	stage_->Update();

	CheckAllCollisions();
}

void GameOverScene::Draw() {
	enemy_->Draw(*currentCamera_);
	stage_->Draw(*currentCamera_);
}

bool GameOverScene::GetIsEndScene() {
	return isEndScene_;
}

void GameOverScene::CheckAllCollisions() {
#pragma region // 敵キャラと平面の当たり判定
	Sphere sphere = enemy_->GetSphere();
	for (uint32_t i = 0; i < Stage::kMaxPlane; i++) {
		if (IsCollision(sphere, stage_->GetPlane(i))) {
			enemy_->OnCollision(stage_->GetPlane(i));
		}
	}
#pragma endregion

#pragma region // 敵キャラとOBBの当たり判定
	sphere = enemy_->GetSphere();
	for (uint32_t i = 0; i < Stage::kMaxOBB; i++) {
		if (IsCollision(stage_->GetOBB(i), sphere)) {
			enemy_->OnCollision(stage_->GetOBB(i));
		}
	}
#pragma endregion
}