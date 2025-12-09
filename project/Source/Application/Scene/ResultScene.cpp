#include "ResultScene.h"
#include "MatsumotoObj/SceneStaticValue.h" 
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
#include "MatsumotoObj/KeyBindConfig.h"

#include "MatsumotoObj/SceneStaticValue.h"

ResultScene::ResultScene() {
	// 現在のカメラを設定
	currentCamera_ = camera_.get();

#pragma region // オブジェクト生成
	resultSprite_ = std::make_unique<ResultSprite>();
#pragma endregion

}

ResultScene::~ResultScene() {
    camera_ = nullptr;
}

void ResultScene::Initialize() {

    Sound::StopAllSound();
	sceneChange_->Initialize();
	sceneChange_->SetState(SceneChange::kFadeOut, 60);

#pragma region // オブジェクト初期化
	resultSprite_->Initialize();
#pragma endregion
}

void ResultScene::Update() {

    if (SceneStaticValue::isClear) {
        Sound::PlayBGM(Sound::resultBGM);
    } else {
        sceneChange_->SetState(SceneChange::kFadeIn, 60);
    }

    if (KeyBindConfig::Instance().IsTrigger("Shot")) {
        sceneChange_->SetState(SceneChange::kFadeIn, 60);
    }

	UpdateCamera();
	UpdateGameObject();
	CheckAllCollision();
	UpdateEmitter();
}

void ResultScene::Draw() {

#ifdef _DEBUG
    DrawGrid::Draw(*currentCamera_);
#endif

#pragma region // オブジェクト描画
	resultSprite_->Draw();
#pragma endregion

    //シーン遷移を描画する
    sceneChange_->Draw();
}

void ResultScene::Debug() {
    if (Input::IsTriggerKey(DIK_Q)) {
        SwitchCamera();
    }

#ifdef USE_IMGUI //ImGuiを使用する際はこれで囲んでください
    ImGui::Text("SwitchCamera : Q key");
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
#endif // !USE_IMGUI
}

void ResultScene::UpdateCamera() {
    if (isDebugCameraActive_) {
        debugCamera_->UpdateMatrix();
    } else {
		camera_->UpdateMatrix();
    }
}

void ResultScene::UpdateGameObject() {
	resultSprite_->Update();
}

void ResultScene::UpdateEmitter() {
}

void ResultScene::CheckAllCollision() {
}
