#define NOMINMAX
#include "TitleScene.h"
#include "Input.h"
#include"Model.h"
#include"ModelManager.h"
#include"Texture.h"
#include"TextureManager.h"
#include"Sprite.h"
#include"Sound.h"
#include"SphereMesh.h"
#include"Random.h"
#include"DebugUI.h"
#include"Quad/Quad.h"
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
    sphereMesh_ = std::make_unique<SphereMesh>();
    quadMesh_->Create(Texture::GetHandle(Texture::UV_CHECKER));
    sphereMesh_->Create({ Texture::GetHandle(Texture::UV_CHECKER) });
    model_ = std::make_unique<Model>();
    model_->Create(ModelManager::WORLD);

    for (int i = 0; i < object3ds_.size(); ++i) {
        object3ds_[i] = std::make_unique<Object3d>();
        object3ds_[i]->Create();
    }

    object3ds_[0]->worldTransform_.Parent(object3ds_[1]->worldTransform_);
    object3ds_[0]->worldTransform_.translate_.x = 2.0f;
    object3ds_[0]->SetMesh(quadMesh_.get());
    object3ds_[1]->SetMesh(sphereMesh_.get());
    object3ds_[2]->SetMesh(model_.get());
}

void TitleScene::Initialize() {

    sceneChange_.Initialize();
    camera_->Initialize(1280.0f, 720.0f);
    camera_->UpdateMatrix();

}

void TitleScene::Update() {

    currentCamera_->UpdateMatrix();

    model_->GetUVTransform().translate.x += std::numbers::pi_v<float> *0.0625f * InverseFPS;
    model_->UpdateUV();

    for (int i = 0; i < object3ds_.size() - 1; ++i) {
        object3ds_[i]->worldTransform_.rotate_.z += std::numbers::pi_v<float> *0.25f * InverseFPS;
    }

    for (int i = 0; i < object3ds_.size(); ++i) {
        object3ds_[i]->Update();
    }
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
    DebugUI::CheckObject3d(*object3ds_[0], "0");
    DebugUI::CheckObject3d(*object3ds_[1], "1");
}

void TitleScene::Draw() {

    DrawGrid::Draw(*currentCamera_);

    for (int i = 0; i < object3ds_.size() - 1; ++i) {
        object3ds_[i]->Draw(*currentCamera_);
    }

    object3ds_[2]->Draw(*currentCamera_, klightModeNone);

}
