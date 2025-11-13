#include "Player.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include"CircleMesh.h"
#include"CubeMesh.h"

Player::Player() {

    //モデルを取得する
    model_ = ModelManager::GetModel(ModelManager::BOX);

    circleMesh_ = std::make_unique<CircleMesh>();
    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_->Create();
    cubeMesh_->SetMinMax(localAabb_);
    circleMesh_->Create();

    //それぞれのObject3d（WorldTransform）を作る
    eyePos_.Create();
    bodyPos_.Create();
    //モデルやメッシュをセットする
    bodyPos_.SetMesh(cubeMesh_.get());
  
}

void Player::Init()
{
    //体の位置初期化
    bodyPos_.worldTransform_.Initialize();
    //目の位置初期化
    eyePos_.Initialize();
    eyePos_.worldTransform_.Initialize();
    eyePos_.worldTransform_.translate_.y = 1.5f;
    eyePos_.worldTransform_.translate_.z = 0.5f;

    //体の位置を親に設定
    eyePos_.worldTransform_.Parent(bodyPos_.worldTransform_);

    velocity_ = { 0.0f,0.0f,0.0f };
    kSpeed_ = { 0.5f };
    lookBackTime_ = 1.0f;
    isLookBackEnd_ = true;
    circle_.radius = 1.0f;
    localAabb_.min = { -circle_.radius , -circle_.radius ,-circle_.radius };
    localAabb_.max = { circle_.radius , circle_.radius ,circle_.radius };
    characterState_ = { .isHit = false,.isAttack = false,  .hp = 100 };
}

void Player::Draw(Camera& camera, const LightMode& lightType)
{
    model_->SetLightMode(lightType);
    bodyPos_.Draw(camera, kBlendModeNormal);
    eyePos_.Draw(camera, kBlendModeNormal);
}

void Player::Update()
{

    Move();
    LookBack();
    MouseLook();
    bodyPos_.Update();
    eyePos_.Update();
    circle_.center = bodyPos_.worldTransform_.GetWorldPosition();

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

    kSpeed_ = (Input::IsPushKey(DIK_LSHIFT)) ? 0.125f : 0.25f;

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

#ifdef USE_IMGUI
    DebugUI::CheckFlag(isLookBack_, "isLookBack_");
    DebugUI::CheckFlag(isLookBackEnd_, "isEnd_");
    ImGui::SliderFloat3("forward", &GetForward().x, 0.0f, 100.0f);
    DebugUI::CheckCharacterState(characterState_, "player");
#endif // USE_IMGUI

    if (Input::IsTriggerMouse(1)) {
        isLookBack_ = true;

        if (isLookBackEnd_) {
            isLookBackEnd_ = false;
            lookBackTime_ = 0.0f;
            startRotateY = bodyPos_.worldTransform_.rotate_.y;
            endRotateY_ = bodyPos_.worldTransform_.rotate_.y + std::numbers::pi_v<float>;
        }

    }

    if (!isLookBack_) {
        return;
    }

    //マウスを離したら
    if (Input::IsPressMouse(1)) {

        if (!isLookBackEnd_) {
            if (lookBackTime_ < 1.0f) {
                lookBackTime_ += InverseFPS * 2.0f;
            } else {
                lookBackTime_ = 1.0f;
                isLookBackEnd_ = true;
            }
        }
        bodyPos_.worldTransform_.rotate_.y = Easing::EaseOutBack(startRotateY, endRotateY_, lookBackTime_);

    } else {
        if (lookBackTime_ > 0.0f) {
            lookBackTime_ -= InverseFPS * 2.0f;
        } else {
            lookBackTime_ = 0.0f;
            isLookBack_ = false;
        }

        bodyPos_.worldTransform_.rotate_.y = Easing::EaseOutQuad(startRotateY, endRotateY_, lookBackTime_);

    }


}

void Player::MouseLook()
{

    if (isLookBack_) {
        return;
    }

    bodyPos_.worldTransform_.rotate_.y += Input::GetMousePosFiltered().x * InverseFPS * 0.125f;
    eyePos_.worldTransform_.rotate_.x += Input::GetMousePosFiltered().y * InverseFPS * 0.125f;

    eyePos_.worldTransform_.rotate_.x = std::clamp(
        eyePos_.worldTransform_.rotate_.x,
        -std::numbers::pi_v<float> / 2.0f,
        std::numbers::pi_v<float> / 2.0f);

}

AABB Player::GetWorldAABB()
{
    Vector3 pos = bodyPos_.worldTransform_.GetWorldPosition();
    return AABB{
        .min = {pos + localAabb_.min},
        .max = {pos + localAabb_.max}
    };
}

void Player::OnCollision(const Circle& circle)
{
    //中心に向かって移動する
    bodyPos_.worldTransform_.translate_ =
        Easing::EaseInOut(
            bodyPos_.worldTransform_.translate_,
            circle_.center + Normalize(circle.center - circle_.center) * circle_.radius
            , 0.5f);
    //仮に音を鳴らす
    Sound::PlayOriginSE(Sound::CRACKER);
}

void Player::OnCollisionEnemy()
{
    if (characterState_.isHit) {
        return;
    }
    //衝突フラグを真に
    characterState_.isHit = true;

}
