#define NOMINMAX
#include "TitleScene.h"
#include "Input.h"
#include"Model.h"
#include"ModelManager.h"
#include"Texture.h"
#include"TextureManager.h"
#include"Sprite.h"
#include"Sound.h"
#include"Random.h"
#include"DebugUI.h"
#include"Quad.h"
#include"MakeMatrix.h"
#include"DrawGrid.h"

TitleScene::TitleScene()
{

    camera_ = std::make_unique<Camera>();

#ifdef _DEBUG
    // デバッグカメラの初期化
    debugCamera_ = std::make_unique<DebugCamera>();
    debugCamera_->Initialize(1280, 720);
#endif // _DEBUG

    // 現在のカメラを設定
    currentCamera_ = camera_.get();
    //矩形を描画
    quadMesh_ = std::make_unique<QuadMesh>();
}

void TitleScene::Initialize() {

    sceneChange_.Initialize();
    camera_->Initialize(1280.0f, 720.0f);
    camera_->UpdateMatrix();
    quadMesh_->Create(Texture::GetHandle(Texture::UV_CHECKER));
}

void TitleScene::Update() {

    currentCamera_->UpdateMatrix();
}


TitleScene::~TitleScene()
{
    camera_ = nullptr;

}

void TitleScene::Debug()
{
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
}

void TitleScene::Draw() {
    DrawGrid::Draw(*currentCamera_);
    quadMesh_->PreDraw();
    quadMesh_->Draw(*currentCamera_, MakeIdentity4x4());
}
