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

    Window* windowClass = &MyEngine::GetWC();

    camera_.Initialize(static_cast<float>(windowClass->GetClientWidth()), static_cast<float>(windowClass->GetClientHeight()), false);
    cameraTransform_ = { { 1.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,-10.0f } };

    cameraSprite_.Initialize(static_cast<float>(windowClass->GetClientWidth()), static_cast<float>(windowClass->GetClientHeight()), true);

#pragma endregion

    for (uint32_t i = 0; i < 5; ++i) {
        Sprite* sprite = new Sprite();
        if (i % 2 == 0) {
            sprite->Initialize(Texture::GetHandle(Texture::PLAYER), { 128.0f,128.0f });
        } else {
            sprite->Initialize(Texture::GetHandle(Texture::UV_CHECKER), { 128.0f,128.0f });
        }

        sprite->SetPosition({ i * 256.0f,0.0f });
        sprites_.push_back(sprite);
    }

    samplePlayer_ = std::make_unique<SamplePlayer>();
    samplePlayer_->Init();

    cube_.resize(2);
    cube_[0].Create(Texture::GetHandle(Texture::WHITE_1X1));
    cube_[1].Create(Texture::GetHandle(Texture::WHITE_1X1));

    cubeWorldTransform_.Initialize();
    cubeWorldTransform_.scale_ = { 10.0f,10.0f,10.0f };
    cubeWorldTransform_.SetRotationY(std::numbers::pi_v<float> / 4.0f);
    WorldTransformUpdate(cubeWorldTransform_);

    particle_.Create(Texture::GetHandle(Texture::UV_CHECKER));

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

    if (Input::IsTriggerKey(DIK_O)) {
        camera_.SetOrthographic(camera_.GetIsOrthographic() ? false : true);
    }

    camera_.Update();

    for (Sprite* sprite : sprites_) {
        sprite->UpdateUV();
    }

    samplePlayer_->Update();

#endif
}

void SampleScene::Draw()
{
#ifdef _DEBUG
    DrawGrid::Draw(camera_);
#endif // _DEBUG

    cube_[0].PreDraw(kBlendModeNormal);
    cube_[0].Draw(camera_, MakeIdentity4x4(), lightType_);
    cube_[1].Draw(camera_, cubeWorldTransform_.matWorld_, lightType_);

    MyEngine::SetBlendMode(blendMode_);
    samplePlayer_->Draw(camera_, lightType_);
    MyEngine::SetBlendMode();

    sprites_[0]->PreDraw(blendMode_);

    for (Sprite* sprite : sprites_) {
        sprite->Draw(cameraSprite_, lightType_);
    }

    particle_.Draw(camera_);
}

void SampleScene::Debug()
{

    DebugUI::CheckDirectionalLight(lightType_);
    DebugUI::CheckFPS();
    DebugUI::CheckBlendMode(blendMode_);
    DebugUI::CheckSprite(*sprites_[0]);

    //入力確認
    DebugUI::CheckInput(*Input::GetInstance());

    //視点操作
    DebugUI::CheckCamera(camera_);
    Input::EyeOperation(camera_);
}

SampleScene::~SampleScene()
{

    for (Sprite* sprite : sprites_) {
        delete sprite;
    }

    sprites_.clear();
}
