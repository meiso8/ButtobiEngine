#include "SamplePlayer.h"

#include"Input.h"
#include"Model.h"
#include"ModelManager.h"
#include"Camera/Camera.h"
#include"DebugUI.h"

SamplePlayer::SamplePlayer() {

    kSpeed_ = 0.0f;
    velocity_ = { 0.0f };
    model_ = new Model();
    model_->Create(ModelManager::PLAYER);
}

SamplePlayer::~SamplePlayer()
{
    delete model_;
}

void SamplePlayer::Init()
{
    worldTransform_.Initialize();
    velocity_ = { 0.0f,0.0f,0.0f };
    kSpeed_ = { 0.5f };
}

void SamplePlayer::Draw(Camera& camera, uint32_t lightType)
{
    model_->Draw( camera, worldTransform_.matWorld_, lightType);
}

void SamplePlayer::Update()
{

    Input* input = Input::GetInstance();

    velocity_ = { 0.0f,0.0f,0.0f };

    if (input->GetJoyStickPos(&velocity_.x, &velocity_.z, Input::BUTTON_LEFT)) {
        //JoyStick
        Vector2 rotate = { velocity_.x,velocity_.z };
        worldTransform_.rotate_.y = std::atan2(-rotate.x, -rotate.y);
    }

    if (input->IsPushKey(DIK_A)) {
        velocity_.x = -1.0f;
    }

    if (input->IsPushKey(DIK_D)) {
        velocity_.x = 1.0f;
    }

    if (input->IsPushKey(DIK_W)) {
        velocity_.z += 1.0f;
    }

    if (input->IsPushKey(DIK_S)) {
        velocity_.z -= 1.0f;
    }

    kSpeed_ = (input->IsPushKey(DIK_LSHIFT)) ? 0.25f : 0.5f;


    if (std::fabsf(velocity_.x) + std::fabsf(velocity_.y) + std::fabsf(velocity_.z) > 1.0f) {
        velocity_ = Normalize(velocity_);
    }

    worldTransform_.translate_ += velocity_ * kSpeed_;

    WorldTransformUpdate(worldTransform_);

}

void SamplePlayer::Debug()
{

    DebugUI::CheckModel(*model_,"playerModel");
}
