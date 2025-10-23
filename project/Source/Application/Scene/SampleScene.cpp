#include "SampleScene.h"
#include"MyEngine.h"
#include"Lerp.h"
#include"Easing.h"
#include"DrawGrid.h"
#include"DebugUI.h"
#include<numbers>
#include"MakeMatrix.h"
#include"Sprite.h"
#include"SphereMesh.h"
#include"Random.h"
#include <functional>

SampleScene::SampleScene()
{
    camera_ = std::make_unique<Camera>();
#ifdef _DEBUG
    debugCamera_ = std::make_unique<DebugCamera>();
#endif // _DEBUG

    uint32_t playerTextureHandle = Texture::GetHandle(Texture::PLAYER);
    uint32_t uvCheckerTextureHandle = Texture::GetHandle(Texture::UV_CHECKER);
    uint32_t white1x1TextureHandle = Texture::GetHandle(Texture::WHITE_1X1);
    uint32_t particleTextureHandle = Texture::GetHandle(Texture::PARTICLE);

    for (uint32_t i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            Sprite* sprite = new Sprite();

                sprite->Create(uvCheckerTextureHandle, { i * 8.0f,j*8.0f }, { 8.0f,8.0f }, { 1.0f,1.0f,1.0f,1.0f });
                sprite->Update();
            sprites_.push_back(sprite);
        }
    }

    samplePlayer_ = std::make_unique<SamplePlayer>();

    cube_.resize(2);
    cube_[0].Create(white1x1TextureHandle);
    cube_[1].Create(white1x1TextureHandle);

    sphereMesh_ = std::make_unique<SphereMesh>();
    sphereMesh_->Create(particleTextureHandle);

    quad_.Create(uvCheckerTextureHandle);
}

void SampleScene::Initialize() {

    isEndScene_ = false;

    lightType_ = MaterialResource::LIGHTTYPE::NONE;
    blendMode_ = BlendMode::kBlendModeNormal;

    camera_->Initialize(static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()), Camera::PERSPECTIVE);
    cameraTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,-10.0f } };
    camera_->SetTransform(cameraTransform_);

    currentCamera_ = camera_.get();

#ifdef _DEBUG
    debugCamera_->Initialize(static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()));
    currentCamera_ = debugCamera_.get();
#endif // _DEBUG

    samplePlayer_->Init();

    worldTransformParent_.Initialize();
    WorldTransformUpdate(worldTransformParent_);

    worldTransformChild_.Initialize();
    worldTransformChild_.parent_ = &worldTransformParent_;
    worldTransformChild_.translate_ = { 0.0f,0.0f,1.0f };
    worldTransformChild_.scale_ = { 1.0f,1.0f,1.0f };
    worldTransformChild_.rotate_.y = (std::numbers::pi_v<float> / 4.0f);
    WorldTransformUpdate(worldTransformChild_);

    sphereMesh_->SetVertex({ 4.0f });


    timer_ = 0.0f;
}

void SampleScene::Update()
{
    timer_ += 1.0f / 60.0f;

    if (timer_ > 1.0f) {
        timer_ = 0.0f;
    }


    Sound::PlayBGM(Sound::BGM1, -0.25f);

    if (Input::IsTriggerMouse(0)) {
        Sound::PlaySE(Sound::PICO, 0.0f);
    }

    if (Input::IsTriggerMouse(1)) {
        Sound::PlaySE(Sound::CRACKER, 0.0f);
    }

    worldTransformChild_.parent_ = &worldTransformParent_;
    WorldTransformUpdate(worldTransformChild_);

    currentCamera_->UpdateMatrix();

    samplePlayer_->Update();

    WorldTransformUpdate(worldTransformParent_);

    WorldTransformUpdate(worldTransformChild_);
    quad_.UpdateUV();

    Random::SetMinMax(0.0f,1.0f);

    prePos_ = sphereMesh_->GetBalloonData().expansion;

    sphereMesh_->GetBalloonData().expansion = Easing::EaseInQuart(0.0f, 1.0f, timer_);
    postPos_ = sphereMesh_->GetBalloonData().expansion;


}

void SampleScene::Draw()
{
#ifdef _DEBUG
    DrawGrid::Draw(*currentCamera_);
#endif // _DEBUG

    cube_[0].PreDraw(kBlendModeNormal);
    cube_[0].Draw(*currentCamera_, worldTransformParent_.matWorld_, lightType_);

    quad_.PreDraw();
    quad_.Draw(*currentCamera_, MakeIdentity4x4());

    sphereMesh_->PreDraw(kBlendModeNormal);
    sphereMesh_->Draw(*currentCamera_, worldTransformChild_.matWorld_, lightType_);

    MyEngine::SetBlendMode(blendMode_, kCullModeBack);
    samplePlayer_->Draw(*currentCamera_, lightType_);
    MyEngine::SetBlendMode();

    Sprite::PreDraw(blendMode_);

    for (Sprite* sprite : sprites_) {
        sprite->Draw();
    }

}

void SampleScene::Debug()
{

    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    DebugUI::CheckCamera(*currentCamera_);
    DebugUI::CheckDirectionalLight(lightType_);
    DebugUI::CheckBlendMode(blendMode_);

    samplePlayer_->Debug();

    DebugUI::CheckBalloonData(cube_[0].GetBalloonData());
    DebugUI::CheckWaveData(cube_[0].GetWaveData(0), "waveData0");
    DebugUI::CheckWaveData(cube_[0].GetWaveData(1), "waveData1");
    DebugUI::CheckWorldTransform(worldTransformParent_, "worldTransformParent");
    DebugUI::CheckWorldTransform(worldTransformChild_, "worldTransform");

    DebugUI::CheckSprite(*sprites_[0], "sprite0");
    ImGui::SliderFloat("easing", &sphereMesh_->GetBalloonData().expansion, 0.0f, 1.0f);
    ImGui::Text("velocity : %f", postPos_ - prePos_);

    ImGui::Begin("Quad");

    DebugUI::CheckTransforms(quad_.GetUVScale(), quad_.GetUVRotate(), quad_.GetUVTranslate(), "uvTransform");

    ImGui::End();

}

bool SampleScene::GetIsEndScene()
{
    return isEndScene_;
}

SampleScene::~SampleScene()
{

    for (Sprite* sprite : sprites_) {
        delete sprite;
    }

    sprites_.clear();

}

