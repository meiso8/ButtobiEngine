#include "EnemyShockWave.h"

#include"ModelManager.h"

#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"
#include"Sound.h"

EnemyShockWave::EnemyShockWave() {

    localAABBs_[kHorizontal] = {.min = {-2.0f,-0.5f,-aabbWidth_},.max = {2.0f,0.5f,aabbWidth_}};
    localAABBs_[kVertical] = { .min = {-aabbWidth_,-0.5f,-2.0f},.max = {aabbWidth_,0.5f,2.0f} };

    body_.Create();
    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_.get()->Create(Texture::WHITE_1X1);
    //cubeMesh_->SetMinMax(localAABBs_[kHorizontal]);



    body_.SetColor({ 1.0f,0.5f,0.0f,1.0f });
    body_.SetMesh(cubeMesh_.get());


    SetCollisionAttribute(kCollisionEnemyWave);
    // 弾は「PlayerとPlayerの弾」とだけ衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionPlayerBullet | kCollisionFloor);

    SetType(kAABB);
    SetAABB(localAABBs_[kHorizontal]);

    ////全部と衝突したい時
    //SetCollisionMask(~kCollisionEnemyBullet);
}

EnemyShockWave::~EnemyShockWave() {
}

void EnemyShockWave::Initialize() {
    body_.Initialize();
    speed_ = 0.02f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 6.0f;
    isActive_ = false;
}
void EnemyShockWave::OnCollision(Collider* collider)
{

    if (!isActive_) {
        return;
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayerBullet) {
        //デバック用
        OnCollisionCollider();
    }


    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        //デバック用
        OnCollisionCollider();
    }

    if (collider->GetCollisionAttribute() == kCollisionFloor) {
        //デバック用
        //OnCollisionCollider();
    }


}
Vector3 EnemyShockWave::GetWorldPosition() const
{
    return body_.worldTransform_.GetWorldPosition();
}
void EnemyShockWave::Update() {

    if (!isActive_) {
        return;
    }

    if (lifeTimer_ <= 0.0f) {
        lifeTimer_ = 0.0f;
        isActive_ = false;
        return;
    } else {
        lifeTimer_ -= 0.016f;
    }

    body_.Update();
    ColliderUpdate();

    float color = lifeTimer_ / lifeDuration_;
    body_.SetColor({ color,color,color,1.0f });
    body_.worldTransform_.translate_ = Lerp(body_.worldTransform_.translate_, endPos_, speed_);

}

void EnemyShockWave::Draw(Camera& camera, const LightMode& lightType) {

    if (!isActive_) {
        return;
    }
    body_.SetLightMode(lightType);
    body_.Draw(camera, kBlendModeNormal);

    ColliderDraw(camera);
}


void EnemyShockWave::Shot(const Vector3& startPos, const Vector3& endPos, const AABBType&aabbType) {
    body_.worldTransform_.translate_ = startPos;
    endPos_ = endPos;
    SetAABB(localAABBs_[aabbType]);
    lifeTimer_ = lifeDuration_;
    isActive_ = true;
}