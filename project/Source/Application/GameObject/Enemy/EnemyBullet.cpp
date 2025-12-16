#include "EnemyBullet.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"

EnemyBullet::EnemyBullet() {

    body_.Create();
    model_ = ModelManager::GetModel(ModelManager::NATURE_OBJECT);
    body_.SetMesh(model_);

    SetRadius(0.5f);
    SetCollisionAttribute(kCollisionEnemyBullet);
    //// 弾は「PlayerとPlayerの弾」とだけ衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionPlayerBullet);
    ////全部と衝突したい時
    //SetCollisionMask(~kCollisionEnemyBullet);
}

EnemyBullet::~EnemyBullet() {
}


void EnemyBullet::InitFlagAndPosAndTimer()
{
    lifeTimer_ = 0.0f;
    body_.worldTransform_.translate_ = { 0.0f,-10.0f,0.0f };
    isActive_ = false;
}

void EnemyBullet::Initialize() {

    body_.Initialize();
    body_.worldTransform_.rotate_.x = 1.0f;
    moveDir_ = { 0.0f,0.0f,1.0f };
    moveSpeed_ = 0.1f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 2.0f;
    isActive_ = false;
    isBulletHit_ = false;
    size_ = 1.0f;
}
void EnemyBullet::OnCollision(Collider* collider)
{

    if (!isActive_) {
        return;
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayerBullet) {
        //弾が衝突
        isBulletHit_ = true;
        //葉っぱの音を鳴らすよ
        Sound::PlaySE(Sound::kPauseButton);
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayer || collider->GetCollisionAttribute() == kCollisionPlayerBullet) {
        //デバック用
        InitFlagAndPosAndTimer();
        OnCollisionCollider();
    }


}
Vector3 EnemyBullet::GetWorldPosition() const
{
    return body_.worldTransform_.GetWorldPosition();
}
void EnemyBullet::Update() {

    if (!isActive_) {
        return;
    }

    if (lifeTimer_ <= 0.0f) {
        InitFlagAndPosAndTimer();
        return;
    } else {
        lifeTimer_ -= 0.016f;
    }

    body_.worldTransform_.rotate_.y += std::numbers::phi_v<float>*InverseFPS * 3.0f;
    body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;
    body_.Update();

    ColliderUpdate();



}

void EnemyBullet::Draw(Camera& camera) {

    if (!isActive_) {
        return;
    }

    body_.Draw(camera, kBlendModeNormal);

    //ColliderDraw(camera);
}

void EnemyBullet::Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size) {
    body_.worldTransform_.translate_ = position;
    moveDir_ = Normalize(direction);
    moveSpeed_ = speed;
    size_ = size;
    body_.worldTransform_.scale_ = { size_,size_,size_ };
    lifeTimer_ = lifeDuration_;
    isActive_ = true;
    body_.Update();

    ColliderUpdate();
}