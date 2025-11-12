#define NOMINMAX
#include "TitleScene.h"
//入力処理に必要なもの
#include "Input.h"
//Debug用のImGui表示セット
#include"DebugUI.h"
//ImGuiだけ使用したかったら以下をインクルードすること
//#include"ImGuiClass.h"

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

#include "ParticleEmitter.h"
#include"Particle.h"

#include"Random.h"
#include"MakeMatrix.h"

TitleScene::TitleScene()
{

    camera_ = std::make_unique<Camera>();

#ifdef _DEBUG
    // デバッグカメラの初期化
    debugCamera_ = std::make_unique<DebugCamera>();
#endif // _DEBUG

    // 現在のカメラを設定
    currentCamera_ = camera_.get();
    //矩形を描画
    planeMesh_ = std::make_unique<PlaneMesh>();
    sphereMesh_ = std::make_unique<SphereMesh>();
    cube_ = std::make_unique<Cube>();
    planeMesh_->Create(Texture::UV_CHECKER);
    sphereMesh_->Create(Texture::UV_CHECKER);
    cube_->Create(Texture::UV_CHECKER);

    //モデルを借りる

    models_[0] = ModelManager::GetModel(ModelManager::BUILDING);

    for (int i = 0; i < object3ds_.size(); ++i) {
        object3ds_[i] = std::make_unique<Object3d>();
        object3ds_[i]->Create();
    }

    object3ds_[0]->worldTransform_.Parent(object3ds_[1]->worldTransform_);
    object3ds_[0]->worldTransform_.translate_.x = 2.0f;
    object3ds_[0]->worldTransform_.rotate_.x = std::numbers::pi_v<float>*0.5f;
    object3ds_[0]->SetMesh(cube_.get());
    object3ds_[1]->SetMesh(models_[0]);


    sprite_ = std::make_unique<Sprite>();
    sprite_->Create(Texture::TEST, { 0.0f,0.0f });

    particleManager_ = std::make_unique<ParticleManager>();
    particleManager_->Create();
    particleManager_->CreateParticleGroup("uvChecker", Texture::UV_CHECKER);
    particleManager_->CreateParticleGroup("numbers", Texture::NUMBERS);
    particleEmitter_ = std::make_unique<ParticleEmitter>();
    particleEmitter_->SetName("uvChecker");

    player_ = std::make_unique<Player>();
    world_ = std::make_unique<World>();
    medjed_ = std::make_unique<Medjed>();
    medjed_->SetTargetMatrix(&player_->GetBodyPos());
    for (int i = 0; i < lockers_.size(); ++i) {
        lockers_[i] = std::make_unique<Locker>();
    }
}

void TitleScene::Initialize() {

    sceneChange_.Initialize();
    camera_->Initialize();
    camera_->UpdateMatrix();
    particleEmitter_->Initialize();
    player_->Init();
    world_->Init();
    for (int i = 0; i < lockers_.size(); ++i) {
        lockers_[i]->Init();
        lockers_[i]->SetPosX(i * 1.0f);
    }
    medjed_->Init();
}

void TitleScene::Update() {

    if (isDebugCameraActive_) {
        currentCamera_->UpdateMatrix();
    } else {
        camera_->worldMat_ = player_->GetEyePos();
        if (Input::IsPushKey(DIK_SPACE)) {
            Vector3 translate = player_->GetForward();
            camera_->worldMat_.m[3][0] += translate.x * 3.0f;
            camera_->worldMat_.m[3][1] += translate.y * 3.0f;
            camera_->worldMat_.m[3][2] += translate.z * 3.0f;
        }

        camera_->UpdateViewProjectionMatrix();

    }

    if (Input::IsTriggerKey(DIK_W)) {
        particleEmitter_->SetName("uvChecker");
    }
    if (Input::IsTriggerKey(DIK_S)) {
        particleEmitter_->SetName("numbers");
    }

    player_->Update();

    medjed_->Update();

    particleEmitter_->Update();
    particleManager_->Update(*currentCamera_);

    object3ds_[0]->worldTransform_.rotate_.z += std::numbers::pi_v<float> *0.25f * InverseFPS;

    for (int i = 0; i < object3ds_.size(); ++i) {
        object3ds_[i]->Update();
    }
    for (int i = 0; i < lockers_.size(); ++i) {
        lockers_[i]->Update();
    }

    world_->Update();

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
    DebugUI::CheckParticle(*particleManager_, *particleEmitter_);
    DebugUI::CheckMaterial(sphereMesh_->GetMaterial(), "sphereMesh");
    DebugUI::CheckPointLightData(sphereMesh_->GetPointLightData(), "sphereMesh");
}

void TitleScene::Draw() {

#ifdef _DEBUG

    DrawGrid::Draw(*currentCamera_);

#endif

    world_->Draw(*currentCamera_);
    object3ds_[1]->Draw(*currentCamera_);
    object3ds_[0]->Draw(*currentCamera_);

    for (int i = 0; i < lockers_.size(); ++i) {
        lockers_[i]->Draw(*currentCamera_);
    }

    medjed_->Draw(*currentCamera_);

    player_->Draw(*currentCamera_, kLightModeHalfL);

    particleManager_->Draw();

    sprite_->PreDraw();
    sprite_->Draw();

}
