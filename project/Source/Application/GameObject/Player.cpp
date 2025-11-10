#include "Player.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

Player::Player() {

    model_ = ModelManager::GetModel(ModelManager::ARM_L);
    object3d_.Create();
    object3d_.SetMesh(model_);

}

void Player::Init()
{
    object3d_.Initialize();
    object3d_.worldTransform_.Initialize();
    velocity_ = { 0.0f,0.0f,0.0f };
    kSpeed_ = { 0.5f };
    time_ = 1.0f;
    isEnd_ = true;
}

void Player::Draw(Camera& camera, const LightMode& lightType)
{

    object3d_.Draw(camera, lightType);
}

void Player::Update()
{

    Move();
    LookBack();
    MouseLook();
    object3d_.Update();

}

void Player::Move()
{
    velocity_ = { 0.0f,0.0f,0.0f };

    object3d_.worldTransform_.translate_.y = Lerp(0.0f, object3d_.worldTransform_.translate_.y, 0.5f);

    if (Input::IsPushKey(DIK_A)) {
        velocity_.x = -1.0f;
    }

    if (Input::IsPushKey(DIK_D)) {
        velocity_.x = 1.0f;
    }

    if (Input::IsPushKey(DIK_W)) {
        velocity_.z = 1.0f;
    }

    if (Input::IsPushKey(DIK_S)) {
        velocity_.z = -1.0f;
    }

    kSpeed_ = (Input::IsPushKey(DIK_LSHIFT)) ? 0.25f : 0.5f;

    if (Input::IsPushKey(DIK_A) || Input::IsPushKey(DIK_D) || Input::IsPushKey(DIK_W) || Input::IsPushKey(DIK_S)) {


        if (soundTimer_ == 0.0f) {
            Sound::PlaySE(Sound::FOOT_STEP);
        }

        if (soundTimer_ < 7.5f) {
            soundTimer_ +=  kSpeed_;
        } else {
            soundTimer_ = 0.0f;
        }

        //前の方向を取得
        Vector3 forward = GetForward();
        forward.y = 0.0f;

        // forwardに垂直な右方向ベクトルを計算
        Vector3 right = Cross(Vector3(0, 1, 0), forward);
        right = Normalize(right);

        //移動時の縦揺れを再現　速さによって揺れの周期を変更
        walkingTheta_ += std::numbers::pi_v<float>*InverseFPS * 15.0f * kSpeed_;
        object3d_.worldTransform_.translate_.y = sinf(walkingTheta_) * 0.25f;

        //速度を正規化しそれぞれ足す
        velocity_ = Normalize(velocity_);
        object3d_.worldTransform_.translate_ += forward * velocity_.z * kSpeed_;
        object3d_.worldTransform_.translate_ += right * velocity_.x * kSpeed_;
    }


}

Vector3& Player::GetForward()
{
    static Vector3 forward;
    forward = Normalize(Vector3{ object3d_.worldTransform_.matWorld_.m[2][0],
        object3d_.worldTransform_.matWorld_.m[2][1],  object3d_.worldTransform_.matWorld_.m[2][2] });
    return forward;
    ;
}

void Player::LookBack()
{


    DebugUI::CheckFlag(isLookBack_, "isLookBack_");
    DebugUI::CheckFlag(isEnd_, "isEnd_");
    ImGui::SliderFloat3("forward", &GetForward().x, 0.0f, 100.0f);

    if (Input::IsTriggerMouse(1)) {
        isLookBack_ = true;

        if (isEnd_) {
            isEnd_ = false;
            time_ = 0.0f;
            startRotateY = object3d_.worldTransform_.rotate_.y;
            endRotateY_ = object3d_.worldTransform_.rotate_.y + std::numbers::pi_v<float>;
        }

    }

    if (!isLookBack_) {
        return;
    }

    //マウスを離したら
    if (Input::IsPressMouse(1)) {

        if (!isEnd_) {
            if (time_ < 1.0f) {
                time_ += InverseFPS * 2.0f;
            } else {
                time_ = 1.0f;
                isEnd_ = true;
            }
        }
        object3d_.worldTransform_.rotate_.y = Easing::EaseOutBack(startRotateY, endRotateY_, time_);

    } else {
        if (time_ > 0.0f) {
            time_ -= InverseFPS * 2.0f;
        } else {
            time_ = 0.0f;
            isLookBack_ = false;
        }

        object3d_.worldTransform_.rotate_.y = Easing::EaseOutQuad(startRotateY, endRotateY_, time_);

    }


}

void Player::MouseLook()
{

    if (isLookBack_) {
        return;
    }

    object3d_.worldTransform_.rotate_.y += Input::GetMousePosFiltered().x * InverseFPS * 0.25f;
    object3d_.worldTransform_.rotate_.x += Input::GetMousePosFiltered().y * InverseFPS * 0.25f;

    object3d_.worldTransform_.rotate_.x = std::clamp(
        object3d_.worldTransform_.rotate_.x,
        -std::numbers::pi_v<float> / 2.0f,
        std::numbers::pi_v<float> / 2.0f);

}
