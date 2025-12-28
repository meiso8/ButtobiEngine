#define NOMINMAX
#include<numbers>
#include "Player.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include"CircleMesh.h"
#include"CubeMesh.h"
#include"JsonFile.h"
#include"LightingManager.h"
#include"MakeMatrix.h"
#include"CoordinateTransform.h"
#include"Sprite.h"
#include"CollisionConfig.h"
void Player::OnCollision(Collider* collider)
{

    if (collider->GetCollisionAttribute() == kCollisionEnemy || collider->GetCollisionAttribute() == kCollisionEnemyBullet || collider->GetCollisionAttribute() == kCollisionMedjed) {
        OnCollisionEnemy();
    }

    if (collider->GetCollisionAttribute() == kCollisionWall) {
        OnCollisionWall();
    }

    OnCollisionCollider();

}
Vector3 Player::GetWorldPosition() const
{
    return bodyPos_.worldTransform_.GetWorldPosition();
}
Player::Player() {

    //モデルを取得する
    model_ = ModelManager::GetModel(ModelManager::PLAYER_BODY);

    circle_.radius = 0.25f;
    localAabb_.min = { -circle_.radius , 0.0f ,-circle_.radius };
    localAabb_.max = { circle_.radius , 1.5f ,circle_.radius };

    SetType(ColliderType::kAABB);
    SetAABB(localAabb_);
    SetCollisionAttribute(kCollisionPlayer);
    SetCollisionMask(kCollisionEnemy | kCollisionEnemyBullet | kCollisionMedjed);

    //それぞれのObject3d（WorldTransform）を作る
    eyePos_.Create();
    bodyPos_.Create();
    //モデルやメッシュをセットする
    bodyPos_.SetMesh(model_);

    raySprite_ = std::make_unique<RaySprite>();

}

void Player::Init()
{

    isPressSpace_ = false;
    zoomTimer_ = 0.0f;

    //体の位置初期化
    bodyPos_.Initialize();
    bodyPos_.worldTransform_.translate_.z = -15.0f;
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

    Json file = JsonFile::GetJsonFiles("config");

    characterState_.hps.hp = file["CharacterState"]["hp"];
    characterState_.hps.maxHp = file["CharacterState"]["hp"];
    characterState_.isAttack = file["CharacterState"]["isAttack"];
    characterState_.isHit = file["CharacterState"]["isHit"];

}

void Player::UpdateRay()
{
    raySprite_->UpdateRay(Ray{ .origin = eyePos_.worldTransform_.GetWorldPosition(), .diff = GetForward() });
}

void Player::Draw(Camera& camera, const LightMode& lightType)
{
    bodyPos_.SetLightMode(lightType);
    bodyPos_.Draw(camera, kBlendModeNormal);
    eyePos_.Draw(camera, kBlendModeNormal);

    raySprite_->Draw();

    ColliderDraw(camera);
}

void Player::Update()
{

    isWallHit = false;

    if (characterState_.isHit) {
        if (hitTimer_ > 0.0f) {
            hitTimer_ -= InverseFPS;
        } else {
            hitTimer_ = 0.0f;
            characterState_.isHit = false;
        }
    }

    Move();
    Zoom();
    PointLightSwitch();
    LookBack();
    MouseLook();
    UpdateRay();

    bodyPos_.Update();
    eyePos_.Update();

    ColliderUpdate();

    circle_.center = GetWorldPosition();

}

void Player::Move()
{
    velocity_ = { 0.0f,0.0f,0.0f };

    bodyPos_.worldTransform_.translate_.y = Lerp(0.0f, bodyPos_.worldTransform_.translate_.y, 0.5f);

    Vector2 controllerPos = { velocity_.x ,velocity_.z };
    if (Input::IsControllerStickPosMove(BUTTON_LEFT, 0, &controllerPos)) {
        velocity_.x = controllerPos.x;
        velocity_.z = controllerPos.y;
    }

    if (Input::IsPushKey(DIK_A)) { velocity_.x = -1.0f; }
    if (Input::IsPushKey(DIK_D)) { velocity_.x = 1.0f; }
    if (Input::IsPushKey(DIK_W)) { velocity_.z = 1.0f; }
    if (Input::IsPushKey(DIK_S)) { velocity_.z = -1.0f; }

    kSpeed_ = (Input::IsPushKey(DIK_LSHIFT) || Input::IsControllerPressButton(XINPUT_GAMEPAD_X, 0)) ? 0.125f : 0.25f;

    if (fabs(velocity_.x) > 0.0f || fabs(velocity_.z) > 0.0f) {
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

void Player::Zoom()
{
    if (Input::IsPushKey(DIK_SPACE)) {

        isZoom_ = true;
        if (zoomTimer_ < 1.0f) {
            zoomTimer_ += InverseFPS * 2.0f;

        } else {
            zoomTimer_ = 1.0f;
            isPressSpace_ = true;
        }

    } else {
        if (zoomTimer_ > 0.0f) {
            zoomTimer_ -= InverseFPS * 2.0f;
        } else {
            zoomTimer_ = 0.0f;
            isZoom_ = false;
        }
    }

}

void Player::PointLightSwitch()
{
    if (Input::IsTriggerMouse(0)) {
        Sound::PlaySE(Sound::SWITCH_ON);
        LightingManager::isPointLightOn_ = LightingManager::isPointLightOn_ ? false : true;
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
    if (Input::IsTriggerMouse(1) || Input::IsControllerTriggerLTRT(BUTTON_RIGHT, 0)) {
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

    if (Input::IsPressMouse(1) || Input::IsControllerPressLTRT(BUTTON_RIGHT, 0)) {

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

    Vector2 controllerPos = { cameraRotateY_ ,cameraRotateX_ };

    if (Input::IsControllerStickPosMove(BUTTON_RIGHT, 0, &controllerPos)) {
        cameraRotateY_ += controllerPos.x * InverseFPS * cameraSpeed_;
        cameraRotateX_ -= controllerPos.y * InverseFPS * cameraSpeed_;
    }

    cameraRotateY_ += Input::GetMousePosFiltered().x * InverseFPS / cameraSpeed_;
    cameraRotateX_ += Input::GetMousePosFiltered().y * InverseFPS / cameraSpeed_;

    bodyPos_.worldTransform_.rotate_.y = Lerp(bodyPos_.worldTransform_.rotate_.y, cameraRotateY_, 0.5f);
    eyePos_.worldTransform_.rotate_.x = Lerp(eyePos_.worldTransform_.rotate_.y, cameraRotateX_, 0.5f);


    eyePos_.worldTransform_.rotate_.x = std::clamp(
        eyePos_.worldTransform_.rotate_.x,
        -std::numbers::pi_v<float> / 2.0f,
        std::numbers::pi_v<float> / 2.0f);

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

void Player::ResolveCollision(const AABB& wallAABB, const AABB& playerAABB) {
    Vector3& pos = bodyPos_.worldTransform_.translate_;

    // 各軸のオーバーラップ量を計算
    float overlapX1 = wallAABB.max.x - playerAABB.min.x;
    float overlapX2 = playerAABB.max.x - wallAABB.min.x;
    //float overlapY1 = wallAABB.max.y - playerAABB.min.y;
    //float overlapY2 = playerAABB.max.y - wallAABB.min.y;
    float overlapZ1 = wallAABB.max.z - playerAABB.min.z;
    float overlapZ2 = playerAABB.max.z - wallAABB.min.z;

    // 最小のオーバーラップ方向を探す
    float minOverlap = std::numeric_limits<float>::max();
    Vector3 push{};

    if (overlapX1 > 0.0f && overlapX1 < minOverlap) {
        minOverlap = overlapX1;
        push = { overlapX1, 0.0f, 0.0f };
    }
    if (overlapX2 > 0.0f && overlapX2 < minOverlap) {
        minOverlap = overlapX2;
        push = { -overlapX2, 0.0f, 0.0f };
    }
    //if (overlapY1 > 0.0f && overlapY1 < minOverlap) {
    //    minOverlap = overlapY1;
    //    push = { 0.0f, overlapY1, 0.0f };
    //}
    //if (overlapY2 > 0.0f && overlapY2 < minOverlap) {
    //    minOverlap = overlapY2;
    //    push = { 0.0f, -overlapY2, 0.0f };
    //}
    if (overlapZ1 > 0.0f && overlapZ1 < minOverlap) {
        minOverlap = overlapZ1;
        push = { 0.0f, 0.0f, overlapZ1 };
    }
    if (overlapZ2 > 0.0f && overlapZ2 < minOverlap) {
        minOverlap = overlapZ2;
        push = { 0.0f, 0.0f, -overlapZ2 };
    }

    // 押し戻し！
    pos += push;
}

void Player::OnCollisionWall()
{
    if (isWallHit) {
        return;
    }
    isWallHit = true;

}

void Player::OnCollisionEnemy()
{
    if (characterState_.isHit) {
        return;
    }

    Sound::PlaySE(Sound::CRACKER);
    //衝突フラグを真に
    characterState_.isHit = true;
    characterState_.hps.hp -= 10;
    hitTimer_ = 1.0f;

}
