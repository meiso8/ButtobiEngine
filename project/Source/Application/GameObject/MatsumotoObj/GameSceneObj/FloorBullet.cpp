#include "FloorBullet.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"


FloorBullet::FloorBullet() {
    body_.Create();
    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_.get()->Create(Texture::WHITE_1X1);
    cubeMesh_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
    body_.SetMesh(cubeMesh_.get());

    SetRadius(0.5f);
    SetCollisionAttribute(kCollisionPlayerBullet);
    // 弾は「敵と敵の弾」とだけ衝突したい
    SetCollisionMask(kCollisionEnemy | kCollisionEnemyBullet);
}

FloorBullet::~FloorBullet() {
}

void FloorBullet::Initialize() {
    body_.Initialize();
    moveDir_ = { 0.0f,0.0f,1.0f };
    moveSpeed_ = 0.2f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 2.0f;
    isActive_ = false;
    isHit_ = false;
    flashingTimer_ = kFlashingTime_;
    size_ = 1.0f;
}
void FloorBullet::InitFlagAndPosAndTimer()
{
    lifeTimer_ = 0.0f;
    flashingTimer_ = 0.0f;

    body_.worldTransform_.translate_ = { 0.0f,-10.0f,0.0f };




    isHit_ = false;
    isActive_ = false;
}
void FloorBullet::OnCollision(Collider* collider)
{
    if (!isActive_) {
        return;
    }

    if (collider->GetCollisionAttribute() == kCollisionEnemy) {
        if (!isHit_&& flashingTimer_ == kFlashingTime_) {
            isHit_ = true;
        }
    }

    //デバック用
    OnCollisionCollider();

}
Vector3 FloorBullet::GetWorldPosition() const
{
    return body_.worldTransform_.GetWorldPosition();
}
void FloorBullet::Update() {

#ifdef USE_IMGUI
    DebugUI::CheckFlag(isActive_, "isActive");
    DebugUI::CheckFlag(isHit_, "isHit_");
    ImGui::Text(" flashingTimer_ %f", flashingTimer_);
    ImGui::Text(" lifeTimer_ %f", lifeTimer_);
    DebugUI::CheckWorldTransform(body_.worldTransform_, "BulletTransform");
#endif

    if (!isActive_) {
        return;
    }

    if (isHit_) {
        Flashing();
    } else {
        Move();
    }

    body_.Update();

    ColliderUpdate();

}

void FloorBullet::Draw(Camera& camera, const LightMode& lightType) {



    if (!isActive_) {
        return;
    }

    body_.SetLightMode(lightType);
    body_.Draw(camera, kBlendModeNormal);
    ColliderDraw(camera);

}

void FloorBullet::Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size) {
    body_.worldTransform_.translate_ = position;
    moveDir_ = Normalize(direction);
    moveSpeed_ = speed;
    size_ = size;
    body_.worldTransform_.rotate_ = moveDir_;
    body_.worldTransform_.scale_ = { size_,size_,size_ };
    lifeTimer_ = lifeDuration_;
    flashingTimer_ = kFlashingTime_;
    isActive_ = true;
    isHit_ = false;
}

void FloorBullet::Flashing()
{
    flashingTimer_ -= InverseFPS;

    if (flashingTimer_ <= 0.0f) {
        InitFlagAndPosAndTimer();
    }

    float t = flashingTimer_ / kFlashingTime_ * 100.0f;

    if (static_cast<int>(t) % 2 == 0) {
        body_.SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
    } else {
        body_.SetColor({ 1.0f, 0.0f, 0.0f, 0.5f });
    }

}

void FloorBullet::Move()
{
    if (lifeTimer_ <= 0.0f) {
        InitFlagAndPosAndTimer();
        return;
    } else {
        lifeTimer_ -= 0.016f;
    }

    body_.worldTransform_.rotate_ = moveDir_;
    body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;

}
