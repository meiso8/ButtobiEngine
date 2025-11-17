#define NOMINMAX
#include "SampleScene.h"
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
#include"Plane/PlaneMesh.h"

#include "ParticleEmitter.h"
#include"Particle.h"

#include"Random.h"
#include"MakeMatrix.h"

#include"Collision.h"
#include"Circle/CircleMesh.h"
#include"Circle.h"

SampleScene::SampleScene()
{
    // 現在のカメラを設定
    currentCamera_ = camera_.get();
    //メッシュの生成
    planeMesh_ = std::make_unique<PlaneMesh>();
    planeMesh_->Create();
    sphereMesh_ = std::make_unique<SphereMesh>();
    sphereMesh_->Create();
    cube_ = std::make_unique<CubeMesh>();
    cube_->Create();

    //モデルを借りる

    models_[0] = ModelManager::GetModel(ModelManager::FLOOR);

    for (int i = 0; i < object3ds_.size(); ++i) {
        object3ds_[i] = std::make_unique<Object3d>();
        object3ds_[i]->Create();
    }

    object3ds_[0]->worldTransform_.Parent(object3ds_[1]->worldTransform_);
    object3ds_[0]->worldTransform_.translate_.x = 2.0f;
    object3ds_[0]->worldTransform_.translate_.y = 1.0f;
    object3ds_[0]->worldTransform_.translate_.z = -2.0f;
    object3ds_[0]->worldTransform_.rotate_.x = std::numbers::pi_v<float>*0.5f;
    object3ds_[0]->SetMesh(cube_.get());

    object3ds_[1]->SetMesh(models_[0]);


    sprite_ = std::make_unique<Sprite>();
    sprite_->Create(Texture::TEST, { 0.0f,0.0f });


    particleManager_ = ParticleManager::GetInstance();

    particleManager_->CreateParticleGroup("uvChecker", Texture::UV_CHECKER);
    particleManager_->CreateParticleGroup("white", Texture::WHITE_1X1);
    particleEmitter_ = std::make_unique<ParticleEmitter>();
    particleEmitter_->SetName("uvChecker");

    player_ = std::make_unique<Player>();
    world_ = std::make_unique<World>();
    filed_ = std::make_unique<Field>();

}

void SampleScene::Initialize() {

    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kWipeOut, 60);
    camera_->Initialize();
    camera_->UpdateMatrix();
    particleEmitter_->Initialize();
    player_->Init();
    world_->Init();
    filed_->Init();
}

void SampleScene::Update() {


    if (isDebugCameraActive_) {
        currentCamera_->UpdateMatrix();
    } else {
        camera_->worldMat_ = player_->GetEyeMatrix();
        //if (Input::IsPushKey(DIK_SPACE)) {
        //    Vector3 translate = player_->GetForward();
        //    camera_->worldMat_.m[3][0] += translate.x * 3.0f;
        //    camera_->worldMat_.m[3][1] += translate.y * 3.0f;
        //    camera_->worldMat_.m[3][2] += translate.z * 3.0f;
        //}

        camera_->UpdateViewProjectionMatrix();

    }

    if (Input::IsTriggerKey(DIK_W)) {
        particleEmitter_->SetName("uvChecker");
    }

    if (Input::IsTriggerKey(DIK_S)) {
        particleEmitter_->SetName("white");
    }

    player_->Update();
    filed_->Update();

    particleEmitter_->Update();
    particleManager_->Update(*currentCamera_);

    object3ds_[0]->worldTransform_.rotate_.z += std::numbers::pi_v<float> *0.25f * InverseFPS;

    for (int i = 0; i < object3ds_.size(); ++i) {
        object3ds_[i]->Update();
    }

    world_->Update();

    CheckAllCollision();
}


SampleScene::~SampleScene()
{
    camera_ = nullptr;
}

void SampleScene::Debug()
{

    if (Input::IsTriggerKey(DIK_Q)) {
        SwitchCamera();
    }

#ifdef USE_IMGUI

    ImGui::Text("SwitchCamera : Q key");
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    DebugUI::CheckObject3d(*object3ds_[0], "0");
    DebugUI::CheckObject3d(*object3ds_[1], "1");
    DebugUI::CheckParticle(*particleEmitter_);
    DebugUI::CheckMesh(*sphereMesh_, "sphereMesh");
    DebugUI::CheckMesh(*cube_, "cube_");
    DebugUI::CheckModel(*models_[0], "model0");
    DebugUI::CheckSprite(*sprite_, "sprite0");

#endif // !USE_IMGUI
}

void SampleScene::CheckAllCollision()
{

    //2つの急の中心点間距離を求める 
    if (Distance(player_->GetCircle(), filed_->circle_) >= player_->GetCircle().radius - filed_->circle_.radius) {

    }

    if (IsCollisionInCircleLine(player_->GetCircle(), filed_->circle_)) {

        player_->OnCollision(filed_->circle_);
        Sound::PlayOriginSE(Sound::CRACKER);
    };
}



void SampleScene::Draw() {

#ifdef _DEBUG

    DrawGrid::Draw(*currentCamera_);

#endif

    world_->Draw(*currentCamera_);
    filed_->Draw(*currentCamera_);

    //object3ds_[1]->Draw(*currentCamera_);
    //object3ds_[0]->Draw(*currentCamera_);

    player_->Draw(*currentCamera_, kLightModeHalfL);

    particleManager_->Draw();

    sprite_->PreDraw();
    sprite_->Draw();

    sceneChange_->Draw();
}
