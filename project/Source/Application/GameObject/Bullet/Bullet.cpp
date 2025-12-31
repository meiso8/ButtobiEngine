#include "Bullet.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"

Bullet::Bullet() {
    model_ = ModelManager::GetModel(ModelManager::PEOPLE);
    body_.Create();
    body_.SetMesh(model_);
    body_.SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });

    SetType(kAABB);
    SetAABB({ {-1.0f,-1.0f,-1.0f} ,{1.0f,1.0f,1.0f} });
    SetBulletType(kEnemy);
}

Bullet::~Bullet() {
}

void Bullet::Initialize() {
    body_.Initialize();
    body_.SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
    moveDir_ = { 0.0f,0.0f,1.0f };
    moveSpeed_ = 0.2f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 2.0f;
    isActive_ = false;
    size_ = 3.0f;
}
void Bullet::OnCollision(Collider* collider)
{
    if (!isActive_) {
        return;
    }
    //デバック用
    OnCollisionCollider();
}
Vector3 Bullet::GetWorldPosition() const
{
    return body_.worldTransform_.GetWorldPosition();
}
void Bullet::Update() {

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

void Bullet::Draw(Camera& camera) {

    if (!isActive_) {
        return;
    }

    body_.Draw(camera, kBlendModeNormal);
    ColliderDraw(camera);
}

void Bullet::SetBulletType(const BulletType& type)
{
    if (type == kPlayer) {
        SetCollisionAttribute(kCollisionPlayerBullet);
        SetCollisionMask(kCollisionEnemy | kCollisionMedjed);
    } else {
        SetCollisionAttribute(kCollisionEnemyBullet);
        // 弾は「Player」とだけ衝突したい
        SetCollisionMask(kCollisionPlayer);
    }
}

void Bullet::Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size, const Bullet::BulletType& type) {
    body_.SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
    type_ = type;
    SetBulletType(type);
    body_.worldTransform_.translate_ = position;
    moveDir_ = Normalize(direction);
    moveSpeed_ = speed;
    size_ = size;
    body_.worldTransform_.rotate_ = moveDir_;
    body_.worldTransform_.scale_ = { size_,size_,size_ };
    lifeTimer_ = lifeDuration_;
    isActive_ = true;
}

void Bullet::SetColor(const Vector4& color)
{
    body_.SetColor(color);
}