#include "ResultScene.h"
#include "MatsumotoObj/SceneStaticValue.h" 
#include "MatsumotoObj/KeyBindConfig.h"
#include"MatsumotoObj/GameSceneObj/HealItemSpawner.h"


#define NOMINMAX

//Debug用のImGui表示セット
#include"DebugUI.h"
//グリッド表示
#include"DrawGrid.h"
//音を鳴らすのに必要なもの
#include"Sound.h"

#include"Input.h"



ResultScene::ResultScene() {
	// 現在のカメラを設定
	currentCamera_ = camera_.get();

    SceneStaticValue::isClear = true;

#pragma region // オブジェクト生成
	resultSprite_ = std::make_unique<ResultSprite>();
	tree_ = std::make_unique<Tree>();
	nest_ = std::make_unique<Nest>();
	bossDummy_ = std::make_unique<BossDummy>();
	playerDummy_ = std::make_unique<PlayerDummy>();
	skyDome_ = std::make_unique<SkyDome>();
#pragma endregion

	resultSceneEmitterManager_ = std::make_unique<ResultSceneEmitterManager>();
	resultSceneEmitterManager_->Create();
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
	skyDome_->Init();
	skyDome_->SetColor(skyDome_->skyColor_);
	tree_->Initialize();
    nest_->Init();
	nest_->SetPosition({ 0.0f,-1.0f,10.0f });
	bossDummy_->Initialize();
	playerDummy_->Initialize();
#pragma endregion

	camera_->translate_ = { 12.0f, 3.0f, 0.0f };
	camera_->rotate_ = { 0.2f, -1.3f, 0.0f };
	
	//ヒールアイテムをクリアする　ここでタイトルシーンのバグを解消できるはず…
	HealItemSpawner::Instance().Initialize();

	resultSceneEmitterManager_->Initialize();
}

void ResultScene::Update() {
	timer_ += 0.016f;

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
	skyDome_->Draw(*currentCamera_);
	tree_->Draw(*currentCamera_);
	nest_->Draw(*currentCamera_);
	bossDummy_->Draw(*currentCamera_, LightMode::kLightModeHalfL);
	playerDummy_->Draw(*currentCamera_);
	
	//エミッター
	resultSceneEmitterManager_->Draw();
	
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
	ImGui::Checkbox("isClear",&SceneStaticValue::isClear);
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);

	resultSceneEmitterManager_->Debug();
#endif // !USE_IMGUI
}

void ResultScene::UpdateCamera() {
    if (isDebugCameraActive_) {
        debugCamera_->UpdateMatrix();
    } else {
		camera_->rotate_ = { 0.2f + sinf(timer_) * 0.01f, -1.3f + cosf(timer_) * 0.01f, 0.0f};

		camera_->UpdateMatrix();
    }
}

void ResultScene::UpdateGameObject() {
	resultSprite_->Update();
	skyDome_->Update();
	tree_->Update();
	nest_->Update();
	bossDummy_->SorryUpdate();
	playerDummy_->Update();
}

void ResultScene::UpdateEmitter() {
	//エミッター
	resultSceneEmitterManager_->Update(*currentCamera_);
}

void ResultScene::CheckAllCollision() {
}
