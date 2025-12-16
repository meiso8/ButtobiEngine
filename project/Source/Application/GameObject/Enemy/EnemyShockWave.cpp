#include "EnemyShockWave.h"

#include"ModelManager.h"

#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"
#include"Sound.h"
#include"VibrateManager.h"
EnemyShockWave::EnemyShockWave() {

    localAABBs_[kHorizontal] = { .min = {-2.0f,-0.5f,-kAabbWidth_},.max = {2.0f,0.5f,kAabbWidth_} };
    localAABBs_[kVertical] = { .min = {-kAabbWidth_,-0.5f,-2.0f},.max = {kAabbWidth_,0.5f,2.0f} };

    body_.Create();
    //for (int i = 0; i < kMaxAABB; ++i) {
    //    cubeMesh_[i] = std::make_unique<CubeMesh>();
    //    cubeMesh_[i].get()->Create(Texture::WHITE_1X1);
    //    cubeMesh_[i]->SetMinMax(localAABBs_[i]);
    //}

    body_.SetColor({ 1.0f,0.0f,0.0f,0.9f });
    //body_.SetMesh(cubeMesh_[kHorizontal].get());


    SetCollisionAttribute(kCollisionEnemyWave);
    // 弾はPlayerとだけ衝突したい
    SetCollisionMask(kCollisionPlayer);

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
    lifeDuration_ = 7.0f;
    isActive_ = false;
    aabbType_ = kHorizontal;
    isEmit_ = false;
}
void EnemyShockWave::OnCollision(Collider* collider)
{

    if (!isActive_) {
        return;
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        //デバック用
        OnCollisionCollider();
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

    isEmit_ = false;

    body_.Update();
    //ColliderUpdate();


    if (lifeTimer_ <= 0.0f) {

        lifeTimer_ = 0.0f;
        isActive_ = false;
        return;
    } else {
        lifeTimer_ -= 0.016f;
    }

    if (lifeTimer_ > 3.5f) {
        isEmit_ = true;
    }

    if (lifeTimer_ <= kMoveTime_) {
        if (!isSound_) {
            isSound_ = true;
            Sound::PlaySE(Sound::kWindAttackShot);
            VibrateManager::SetTime(1.0f,1000, 1000);
        }
        body_.worldTransform_.translate_ = Lerp(body_.worldTransform_.translate_, endPos_, speed_);
    } else {
        float lifeTime = lifeDuration_ - lifeTimer_;
        lifeTime =1.0f- std::clamp(lifeTime, 0.0f, 1.0f);
       
        body_.SetColor({ 1.0f,0.0f,0.0f,lifeTime*0.9f });

    }

}

void EnemyShockWave::Draw(Camera& camera) {

    if (!isActive_) {
        return;
    }

    body_.Draw(camera);

    //ColliderDraw(camera);
}


void EnemyShockWave::Shot(const Vector3& startPos, const Vector3& endPos, const AABBType& aabbType) {
    body_.worldTransform_.translate_ = startPos;
    endPos_ = endPos;
    SetAABB(localAABBs_[aabbType]);
    //body_.SetMesh(cubeMesh_[aabbType].get());
    body_.SetColor({ 1.0f,0.0f,0.0f,0.9f });
    aabbType_ = aabbType;
    lifeTimer_ = lifeDuration_;
    isActive_ = true;
    body_.Update();

    isSound_ = false;
}