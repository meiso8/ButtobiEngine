#include "SampleScene.h"
#include"MyEngine.h"
#include"Lerp.h"

#include"DrawGrid.h"
#include"DebugUI.h"
#include<numbers>
#include"MakeMatrix.h"
#include"Sprite.h"
#include"SphereMesh.h"
#include <functional>

void SampleScene::Initialize() {

    DrawGrid::Initialize();

    lightType_ = MaterialResource::LIGHTTYPE::NONE;
    blendMode_ = BlendMode::kBlendModeNone;

#pragma region//Camera

    camera_ = std::make_unique<Camera>();
    camera_->Initialize(static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()), Camera::PERSPECTIVE);
    cameraTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,-10.0f } };

    debugCamera_ = std::make_unique<DebugCamera>();
    debugCamera_->Initialize(static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()));

    currentCamera_ = camera_.get();

#pragma endregion

    for (uint32_t i = 0; i < 5; ++i) {
        Sprite* sprite = new Sprite();
        if (i % 2 == 0) {
            sprite->Create(Texture::handle_[Texture::PLAYER], { i * 256.0f,0.0f }, { 128.0f,128.0f }, { 1.0f,1.0f,1.0f,1.0f });
        } else {
            sprite->Create(Texture::handle_[Texture::UV_CHECKER], { i * 256.0f,0.0f }, { 128.0f,128.0f }, { 1.0f,1.0f,1.0f,1.0f });
        }
        sprites_.push_back(sprite);
    }

    samplePlayer_ = std::make_unique<SamplePlayer>();
    samplePlayer_->Init();

    cube_.resize(2);
    cube_[0].Create(Texture::handle_[Texture::WHITE_1X1]);
    cube_[1].Create(Texture::handle_[Texture::WHITE_1X1]);

    worldTransformParent_.Initialize();
    WorldTransformUpdate(worldTransformParent_);


    worldTransformChild_.Initialize();
    worldTransformChild_.parent_ = &worldTransformParent_;



    worldTransformChild_.translate_ = { 0.0f,0.0f,1.0f };
    worldTransformChild_.scale_ = { 1.0f,1.0f,1.0f };
    worldTransformChild_.rotate_.y = (std::numbers::pi_v<float> / 4.0f);
    WorldTransformUpdate(worldTransformChild_);

    particle_.Initialize(Texture::handle_[Texture::UV_CHECKER]);
    sphereMesh_ = std::make_unique<SphereMesh>();
    sphereMesh_->Create(Texture::handle_[Texture::UV_CHECKER]);
    sphereMesh_->SetVertex({ 4.0f });

    quad_.Create(Texture::handle_[Texture::UV_CHECKER]);
    scale_ = { 1.0f,1.0f };
    uvMoveVal_ = { 0.0f,0.0f };
}

void SampleScene::Update()
{

#ifdef _DEBUG

    //if (!SoundManager::IsPlaying()) {
    //    SoundManager::Play(Sound::GetHandle(Sound::BGM1), 0.0625f, true);
    //}

    if (Input::IsTriggerKey(DIK_A)) {
        particle_.Create();
    }

    worldTransformChild_.parent_ = &worldTransformParent_;
    WorldTransformUpdate(worldTransformChild_);

    if (Input::IsTriggerMouse(0)) {
        SoundManager::Play(Sound::GetHandle(Sound::SE1), 0.0625f, false);
    }

    currentCamera_->UpdateMatrix();

    for (Sprite* sprite : sprites_) {
        sprite->UpdateUV();
    }

    samplePlayer_->Update();


    WorldTransformUpdate(worldTransformParent_);

    WorldTransformUpdate(worldTransformChild_);
    quad_.UpdateUV();

    particle_.Update();
#endif
}

void SampleScene::Draw()
{
#ifdef _DEBUG
    DrawGrid::Draw(*currentCamera_);
#endif // _DEBUG

    //cube_[0].PreDraw(kBlendModeNormal);
    //cube_[0].Draw(*currentCamera_, worldTransformParent_.matWorld_, lightType_);
    quad_.PreDraw();
    quad_.Draw(*currentCamera_, MakeIdentity4x4());
 

    

    //sphereMesh_->PreDraw(kBlendModeNormal);
    //sphereMesh_->Draw(*currentCamera_, worldTransformChild_.matWorld_, lightType_);

    MyEngine::SetBlendMode(blendMode_);
    samplePlayer_->Draw(*currentCamera_, lightType_);
    MyEngine::SetBlendMode();

    Sprite::PreDraw(blendMode_);

    for (Sprite* sprite : sprites_) {
        sprite->Draw();
    }

    particle_.Draw(*currentCamera_,kBlendModeAdd);
}

void SampleScene::Debug()
{

    samplePlayer_->Debug();

    DebugUI::CheckBalloonData(cube_[0].GetBalloonData());
    DebugUI::CheckWaveData(cube_[0].GetWaveData(0), "waveData0");
    DebugUI::CheckWaveData(cube_[0].GetWaveData(1), "waveData1");
    DebugUI::CheckDirectionalLight(lightType_);
    DebugUI::CheckBlendMode(blendMode_);
    DebugUI::CheckSprite(*sprites_[0], "sprite0");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    DebugUI::CheckCamera(*currentCamera_);
    DebugUI::CheckWorldTransform(worldTransformParent_, "worldTransformParent");
    DebugUI::CheckWorldTransform(worldTransformChild_, "worldTransform");

    ImGui::Begin("Quad");
    ImGui::SliderFloat2("scale", &scale_.x, -10.0f, 10.0f);
    ImGui::SliderFloat2("uvMoveVol", &uvMoveVal_.x, -10.0f, 10.0f);
    DebugUI::CheckTransforms(quad_.GetUVScale(), quad_.GetUVRotate(), quad_.GetUVTranslate(), "uvTransform");

    ImGui::End();
    DebugUI::CheckFPS();
}

SampleScene::~SampleScene()
{

    for (Sprite* sprite : sprites_) {
        delete sprite;
    }

    sprites_.clear();
}

