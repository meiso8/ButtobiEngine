#include "EnemyBomb.h"
#include"ModelManager.h"

#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"
#include"Sound.h"


EnemyBomb::EnemyBomb() {
    body_.Create();
    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_.get()->Create(Texture::WHITE_1X1);
    body_.SetColor({ 0.0f,0.0f,1.0f,1.0f });
    body_.SetMesh(cubeMesh_.get());

    SetRadius(kRadius_);
    SetCollisionAttribute(kCollisionNone);
    // 弾は「PlayerとPlayerの弾」とだけ衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionPlayerBullet| kCollisionFloor);
    ////全部と衝突したい時
    //SetCollisionMask(~kCollisionEnemyBullet);
}

EnemyBomb::~EnemyBomb() {
}

void EnemyBomb::Initialize() {
    body_.Initialize();
    moveDir_ = { 0.0f,0.0f,1.0f };
    speed_ = 0.1f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 4.5f;
    isActive_ = false;
	isGroundHit_ = false;
    size_ = 1.0f;
    lifeDelay_ = 0.0f;

}
void EnemyBomb::OnCollision(Collider* collider)
{

    if (!isActive_) {
        return;
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayerBullet) {
        //デバック用
        OnCollisionCollider();
    }

    if (collider->GetCollisionAttribute() == kCollisionFloor) {
        if (lifeTimer_ <= 3.8f) {
            isActive_ = false;
            isGroundHit_ = false;
        }
    }
}
Vector3 EnemyBomb::GetWorldPosition() const
{
    return body_.worldTransform_.GetWorldPosition();
}
void EnemyBomb::Update() {

    if (!isActive_) {
        return;
    }

    if (lifeTimer_ <= 0.0f) {

     lifeTimer_ = 0.0f;
     
    } else {
        lifeTimer_ -= 0.016f;
    }

    body_.worldTransform_.rotate_ = moveDir_;

    body_.Update();
    ColliderUpdate();

    float color = lifeTimer_ / lifeDuration_ + lifeDelay_;
    body_.SetColor({ color,color,color,1.0f });

    if (lifeTimer_ <= 3.8f) {
        SetCollisionAttribute(kCollisionEnemyBomb);
        body_.worldTransform_.translate_ = Lerp(body_.worldTransform_.translate_, endPos_, speed_);
    } else {
        SetCollisionAttribute(kCollisionNone);
        Vector3 endPos = endPos_;
        endPos.y += 2.0f;
        body_.worldTransform_.translate_ = Lerp(body_.worldTransform_.translate_, endPos, speed_);
    }

}

void EnemyBomb::Draw(Camera& camera) {

    if (!isActive_) {
        return;
    }

    body_.Draw(camera, kBlendModeNormal);

    ColliderDraw(camera);
}

void EnemyBomb::Shot(const Vector3& startPos, const Vector3& endPos, const float& size) {
    body_.worldTransform_.translate_ = startPos;
    endPos_ = endPos;
    size_ = size;
    body_.worldTransform_.scale_ = { size_,size_,size_ };
	lifeDelay_ = std::fabsf(Length(endPos - startPos) / 5.0f);
    lifeTimer_ = lifeDuration_ + lifeDelay_;
    isActive_ = true;
	isGroundHit_ = false;
	body_.Update();
	ColliderUpdate();
}