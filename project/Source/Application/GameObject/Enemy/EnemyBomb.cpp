#include "EnemyBomb.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"
#include"Sound.h"
#include"Random.h"

EnemyBomb::EnemyBomb() {
    body_.Create();

    body_.SetColor({ 0.0f,0.0f,1.0f,1.0f });
    model_ = ModelManager::GetModel(ModelManager::BOMB);
    body_.SetMesh(model_);

    SetRadius(kRadius_);
    SetCollisionAttribute(kCollisionNone);
    // 弾は「PlayerとPlayerの弾」とだけ衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionFloor | kCollisionStrongFloor);
    ////全部と衝突したい時
    //SetCollisionMask(~kCollisionEnemyBullet);
}

EnemyBomb::~EnemyBomb() {
}

void EnemyBomb::Initialize() {
    body_.Initialize();
    body_.worldTransform_.rotate_.y = std::numbers::pi_v<float>;
    body_.worldTransform_.rotate_.x = -1.0f;
    speed_ = 0.1f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 4.5f;
    isActive_ = false;
    size_ = 1.0f;
    lifeDelay_ = 0.0f;
    isBound_ = false;
    velocity_ = { 0.0f };
}
void EnemyBomb::OnCollision(Collider* collider)
{

    if (!isActive_) {
        return;
    }

    if (collider->GetCollisionAttribute() == kCollisionFloor) {
        if (lifeTimer_ <= 3.8f) {
            isActive_ = false;
        }
    }

    if (collider->GetCollisionAttribute() == kCollisionStrongFloor) {

        if (isBound_) {
            velocity_.y = 0.5f;
            return;
        }

        isBound_ = true;
        Random::SetMinMax(-0.5f, 0.5f);

        velocity_ = { Random::Get(),-0.1f,Random::Get() };
        Sound::PlaySE(Sound::kFloorRespawn);
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

    if (lifeTimer_ <= 2.5f) {

        lifeTimer_ = 0.0f;
        isActive_ = false;
    } else {
        lifeTimer_ -= 0.016f;
    }


    body_.Update();
    ColliderUpdate();

    float color = lifeTimer_ / lifeDuration_ + lifeDelay_;
    body_.SetColor({ color,color,color,1.0f });


    if (isBound_) {
        body_.worldTransform_.translate_ += velocity_;
        body_.worldTransform_.rotate_.y += std::numbers::pi_v<float>*0.25f;
    } else {

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


}

void EnemyBomb::Draw(Camera& camera) {

    if (!isActive_) {
        return;
    }

    body_.SetLightMode(kLightModeLReflectance);
    body_.Draw(camera, kBlendModeNormal);

    ColliderDraw(camera);
}

void EnemyBomb::Shot(const Vector3& startPos, const Vector3& endPos, const float& size) {
    body_.worldTransform_.translate_ = startPos;
    endPos_ = endPos;
    size_ = size;
    body_.worldTransform_.scale_ = { size_,size_,size_ };
    body_.worldTransform_.rotate_.y = std::numbers::pi_v<float>;
    body_.worldTransform_.rotate_.x = -1.0f;


    lifeDelay_ = std::fabsf(Length(endPos - startPos) / 5.0f);
    lifeTimer_ = lifeDuration_ + lifeDelay_;
    isActive_ = true;
    isBound_ = false;
    body_.Update();
    ColliderUpdate();
}