#define NOMINMAX
#include "TitleScene.h"
#include "Input.h"
#include"Model.h"
#include"ModelManager.h"

#include"Texture.h"
#include"Sprite.h"
#include"Sound.h"
#include"SphereMesh.h"
#include"Random.h"
#include"DebugUI.h"
#include"Quad/Quad.h"
#include"MakeMatrix.h"
#include"DrawGrid.h"
#include "Particle/ParticleEmitter.h"


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
    quadMesh_ = std::make_unique<QuadMesh>();
    sphereMesh_ = std::make_unique<SphereMesh>();
    quadMesh_->Create(Texture::GetHandle(Texture::UV_CHECKER));
    sphereMesh_->Create({ Texture::GetHandle(Texture::UV_CHECKER) });
    //モデルを借りる

    models_[0] = ModelManager::GetModel(ModelManager::BUILDING);

    for (int i = 0; i < object3ds_.size(); ++i) {
        object3ds_[i] = std::make_unique<Object3d>();
        object3ds_[i]->Create();
    }

    object3ds_[0]->worldTransform_.Parent(object3ds_[1]->worldTransform_);
    object3ds_[0]->worldTransform_.translate_.x = 2.0f;
    object3ds_[0]->worldTransform_.rotate_.x = std::numbers::pi_v<float>*0.5f;
    object3ds_[0]->SetMesh(sphereMesh_.get());
    object3ds_[1]->SetMesh(models_[0]);


    sprite_ = std::make_unique<Sprite>();
    sprite_->Create(Texture::GetHandle(Texture::TEST), { 0.0f,0.0f });

    particleManager_ = std::make_unique<ParticleManager>();
    particleManager_->Create();
    particleManager_->CreateParticleGroup("uvChecker", Texture::GetHandle(Texture::UV_CHECKER));
    particleManager_->CreateParticleGroup("numbers", Texture::GetHandle(Texture::NUMBERS));
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

    if (Input::IsPushKey(DIK_A)) {
        particleEmitter_->emitter_.transform.translate.x -= InverseFPS * 2.0f;
    }
    if (Input::IsPushKey(DIK_D)) {
        particleEmitter_->emitter_.transform.translate.x += InverseFPS * 2.0f;
    }

    if (Input::IsPushKey(DIK_Q)) {
        currentCamera_->translate_.x -= InverseFPS * 2.0f;
    }
    if (Input::IsPushKey(DIK_E)) {
        currentCamera_->translate_.x += InverseFPS * 2.0f;
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
    DebugUI::CheckMaterial(*sphereMesh_->GetMaterial(),"sphereMesh");

}

void TitleScene::Draw() {

#ifdef _DEBUG

    DrawGrid::Draw(*currentCamera_);

#endif

    world_->Draw(*currentCamera_);
    object3ds_[1]->Draw(*currentCamera_, kLightModeHalfL);
    object3ds_[0]->Draw(*currentCamera_, kLightModeLReflectance, kBlendModeNormal, kCullModeNone);

    for (int i = 0; i < lockers_.size(); ++i) {
        lockers_[i]->Draw(*currentCamera_);
    }

    medjed_->Draw(*currentCamera_);

    player_->Draw(*currentCamera_, kLightModeHalfL);

    particleManager_->Draw();

    sprite_->PreDraw();
    sprite_->Draw();

}
