#include "EnemyBomb.h"
#include"ModelManager.h"

#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"
#include"Sound.h"

bool EnemyBomb::isPlayerHit_ = false;

EnemyBomb::EnemyBomb() {
    body_.Create();
    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_.get()->Create(Texture::WHITE_1X1);
    body_.SetColor({ 0.0f,0.0f,1.0f,1.0f });
    body_.SetMesh(cubeMesh_.get());

    SetRadius(kRadius_);
    SetCollisionAttribute(kCollisionEnemyBomb);
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
    lifeDuration_ = 6.0f;
    isActive_ = false;
    size_ = 1.0f;
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


    if (collider->GetCollisionAttribute() == kCollisionPlayer&&isExplosion_) {
        //デバック用
        isPlayerHit_ = true;
        OnCollisionCollider();
    }

    if (collider->GetCollisionAttribute() == kCollisionFloor) {
        //デバック用
        //OnCollisionCollider();
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
        if (!isExplosion_) {
            Sound::PlaySE(Sound::PICO);
            isExplosion_ = true;
            explosionFrame_ = 0;
            lifeTimer_ = 0.0f;
        }
    } else {
        lifeTimer_ -= 0.016f;
    }

    body_.worldTransform_.rotate_ = moveDir_;

    body_.Update();
    ColliderUpdate();

    if (isExplosion_) {
        explosionFrame_++;
       
        if (explosionFrame_ >= 3) {
            isExplosion_ = false;
            isActive_ = false;
            return;
        }
    }

    float color = lifeTimer_ / lifeDuration_;
    body_.SetColor({ color,color,color,1.0f });

    if (lifeTimer_ <= 3.8f) {
        body_.worldTransform_.translate_ = Lerp(body_.worldTransform_.translate_, endPos_, speed_);
    } else {
        Vector3 endPos = endPos_;
        endPos.y += 2.0f;
        body_.worldTransform_.translate_ = Lerp(body_.worldTransform_.translate_, endPos, speed_);
    }

}

void EnemyBomb::Draw(Camera& camera, const LightMode& lightType) {

    if (!isActive_) {
        return;
    }
    body_.SetLightMode(lightType);
    body_.Draw(camera, kBlendModeNormal);

    ColliderDraw(camera);
}


void EnemyBomb::Shot(const Vector3& startPos, const Vector3& endPos, const float& size) {
    body_.worldTransform_.translate_ = startPos;
    endPos_ = endPos;
    size_ = size;
    body_.worldTransform_.scale_ = { size_,size_,size_ };
    lifeTimer_ = lifeDuration_;
    isActive_ = true;
    isExplosion_ = false;
}