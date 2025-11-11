#include "Player.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

Player::Player() {

    model_ = ModelManager::GetModel(ModelManager::PLAYER);
    eyePos_.Create();
    //eyePos_.SetMesh(model_);
    bodyPos_.Create();
    bodyPos_.SetMesh(model_);
}

void Player::Init()
{
    bodyPos_.worldTransform_.Initialize();

    eyePos_.Initialize();
    eyePos_.worldTransform_.Initialize();
    eyePos_.worldTransform_.translate_.y = 1.5f;
    eyePos_.worldTransform_.translate_.z= 0.5f;

    //体の位置が親
    eyePos_.worldTransform_.Parent(bodyPos_.worldTransform_);

    velocity_ = { 0.0f,0.0f,0.0f };
    kSpeed_ = { 0.5f };
    time_ = 1.0f;
    isEnd_ = true;
}

void Player::Draw(Camera& camera, const LightMode& lightType)
{
    bodyPos_.Draw(camera,lightType);
    eyePos_.Draw(camera, lightType);
}

void Player::Update()
{

    Move();
    LookBack();
    MouseLook();   
    bodyPos_.Update();
    eyePos_.Update();


}

void Player::Move()
{
    velocity_ = { 0.0f,0.0f,0.0f };

    bodyPos_.worldTransform_.translate_.y = Lerp(0.0f, bodyPos_.worldTransform_.translate_.y, 0.5f);

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
            soundTimer_ += kSpeed_;
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
        bodyPos_.worldTransform_.translate_.y = sinf(walkingTheta_) * 0.25f;

        //速度を正規化しそれぞれ足す
        velocity_ = Normalize(velocity_);
        bodyPos_.worldTransform_.translate_ += forward * velocity_.z * kSpeed_;
        bodyPos_.worldTransform_.translate_ += right * velocity_.x * kSpeed_;
    }


}

Vector3& Player::GetForward()
{
    static Vector3 forward;
    forward = Normalize(Vector3{ eyePos_.worldTransform_.matWorld_.m[2][0],
        eyePos_.worldTransform_.matWorld_.m[2][1],  eyePos_.worldTransform_.matWorld_.m[2][2] });
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
            startRotateY = bodyPos_.worldTransform_.rotate_.y;
            endRotateY_ = bodyPos_.worldTransform_.rotate_.y + std::numbers::pi_v<float>;
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
        bodyPos_.worldTransform_.rotate_.y = Easing::EaseOutBack(startRotateY, endRotateY_, time_);

    } else {
        if (time_ > 0.0f) {
            time_ -= InverseFPS * 2.0f;
        } else {
            time_ = 0.0f;
            isLookBack_ = false;
        }

        bodyPos_.worldTransform_.rotate_.y = Easing::EaseOutQuad(startRotateY, endRotateY_, time_);

    }


}

void Player::MouseLook()
{

    if (isLookBack_) {
        return;
    }

    bodyPos_.worldTransform_.rotate_.y += Input::GetMousePosFiltered().x * InverseFPS * 0.25f;
   

    
    
    eyePos_.worldTransform_.rotate_.x += Input::GetMousePosFiltered().y * InverseFPS * 0.25f;

    eyePos_.worldTransform_.rotate_.x = std::clamp(
        eyePos_.worldTransform_.rotate_.x,
        -std::numbers::pi_v<float> / 2.0f,
        std::numbers::pi_v<float> / 2.0f);


}
