#include "SampleScene.h"
#include"MyEngine.h"
#include"Lerp.h"

#include"DrawGrid.h"
#include"DebugUI.h"
#include<numbers>
#include"MakeIdentity4x4.h"
#include"Sprite.h"
#include"Model.h"

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

    currentCamera = camera_.get();

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

    cubeWorldTransform_.Initialize();
    cubeWorldTransform_.scale_ = { 10.0f,10.0f,10.0f };
    cubeWorldTransform_.SetRotationY(std::numbers::pi_v<float> / 4.0f);
    WorldTransformUpdate(cubeWorldTransform_);

    //particle_.Create(Texture::GetHandle(Texture::UV_CHECKER));

}

void SampleScene::Update()
{

#ifdef _DEBUG

    if (!SoundManager::IsPlaying()) {
        SoundManager::Play(Sound::GetHandle(Sound::BGM1), 0.0625f, true);
    }

    if (Input::IsTriggerKey(DIK_W)) {
        SoundManager::Play(Sound::GetHandle(Sound::SE1), 1.0f, false);
    }

    if (Input::IsTriggerKey(DIK_P)) {
        // スペースキーを押すとデバッグカメラに切り替える
        isDebugCameraActive_ = isDebugCameraActive_ ? false : true;
        currentCamera = (isDebugCameraActive_) ? debugCamera_.get() : camera_.get();
    }

    currentCamera->UpdateMatrix();

    for (Sprite* sprite : sprites_) {
        sprite->UpdateUV();
    }

    samplePlayer_->Update();

#endif
}

void SampleScene::Draw()
{
#ifdef _DEBUG
    DrawGrid::Draw(*currentCamera);
#endif // _DEBUG

    cube_[0].PreDraw(kBlendModeNormal);
    cube_[0].Draw(*currentCamera, MakeIdentity4x4(), lightType_);
    cube_[1].Draw(*currentCamera, cubeWorldTransform_.matWorld_, lightType_);

    MyEngine::SetBlendMode(blendMode_);
    samplePlayer_->Draw(*currentCamera, lightType_);
    MyEngine::SetBlendMode();

    Sprite::PreDraw(blendMode_);

    for (Sprite* sprite : sprites_) {
        sprite->Draw();
    }

    //particle_.Draw(camera_);
}

void SampleScene::Debug()
{

    DebugUI::CheckDirectionalLight(lightType_);

    DebugUI::CheckBlendMode(blendMode_);
    DebugUI::CheckSprite(*sprites_[0], "sprite0");
    DebugUI::CheckCamera(*currentCamera);
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugActive");
    DebugUI::CheckFPS();
}

SampleScene::~SampleScene()
{

    for (Sprite* sprite : sprites_) {
        delete sprite;
    }

    sprites_.clear();
}
