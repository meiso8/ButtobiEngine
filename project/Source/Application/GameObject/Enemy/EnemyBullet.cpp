#include "EnemyBullet.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"

EnemyBullet::EnemyBullet() {
    model_ = ModelManager::GetModel(ModelManager::PEOPLE);
    body_.Create();
    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_.get()->Create(Texture::WHITE_1X1);
    cubeMesh_->SetColor({ 0.0f,1.0f,0.0f,1.0f });
    body_.SetMesh(model_);

    SetRadius(1.5f);
    SetCollisionAttribute(kCollisionEnemyBullet);
    // 弾は「Player」とだけ衝突したい
    SetCollisionMask(kCollisionPlayer);
}

EnemyBullet::~EnemyBullet() {
}

void EnemyBullet::Initialize() {
    body_.Initialize();
    moveDir_ = { 0.0f,0.0f,1.0f };
    moveSpeed_ = 0.2f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 2.0f;
    isActive_ = false;
    size_ = 3.0f;
}
void EnemyBullet::OnCollision(Collider* collider)
{
    if (!isActive_) {
        return;
    }
    //デバック用
    OnCollisionCollider();
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
        isActive_ = false;
        return;
    } else {
        lifeTimer_ -= 0.016f;
    }

    body_.worldTransform_.rotate_ = moveDir_;
    body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;
    body_.Update();

    ColliderUpdate();

}

void EnemyBullet::Draw(Camera& camera, const LightMode& lightType) {


    if (!isActive_) {
        return;
    }
    body_.SetLightMode(lightType);
    body_.Draw(camera, kBlendModeNormal);
    ColliderDraw(camera);


}

void EnemyBullet::Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size) {
    body_.worldTransform_.translate_ = position;
    moveDir_ = Normalize(direction);
    moveSpeed_ = speed;
    size_ = size;
    body_.worldTransform_.rotate_ = moveDir_;
    body_.worldTransform_.scale_ = { size_,size_,size_ };
    lifeTimer_ = lifeDuration_;
    isActive_ = true;
}