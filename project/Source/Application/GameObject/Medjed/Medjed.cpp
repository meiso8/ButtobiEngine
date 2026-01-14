#include "Medjed.h"
#include"ModelManager.h"
#include"MakeMatrix.h"
#include"SkinningModel.h"
#include"CollisionConfig.h"
#include"Sound.h"
#include"TransformAni/TransformAni.h"
#include"TimeManager.h"
#include<algorithm>
#include"Easing.h"
void Medjed::OnCollision(Collider* collider)
{

    if (collider->GetCollisionAttribute() == kCollisionPlayer) {

        if (!isHit_) {
            isHit_ = true;
            if (!isFind_) {
                Sound::PlayOriginSE(Sound::VOICE_Asobimasyo, 0.5f);
            }
        }

    }


    if (collider->GetCollisionAttribute() == kCollisionWall || collider->GetCollisionAttribute() == kCollisionDummyMedjed) {
        velocity_ *= -0.8f;
        ResolveCollision(aniObj_->worldTransform_.translate_, velocity_, GetCollisionInfo());
    }

    OnCollisionCollider();
}

Vector3 Medjed::GetWorldPosition() const
{
    return aniObj_->worldTransform_.GetWorldPosition();
}

void Medjed::GoToTarget(const Vector3& target)
{
    aniObj_->worldTransform_.translate_ = Lerp(aniObj_->worldTransform_.translate_, target, 0.05f);
}


void Medjed::MoveStart()
{
    aniObj_->InitTime();
    aniObj_->SetAnimation(ModelManager::GetModel("medjedAnimation.gltf"));
}

//
//void Medjed::MoveAround(const Vector3& target, const Vector3& dir)
//{
//    // プレイヤーの後ろを目指す
//    Vector3 newDir = { dir.x, 0.0f, dir.z };
//    Vector3 desiredPos = target - Normalize(newDir) * 5.0f;
//
//    // 現在位置
//    Vector3& currentPos = aniObj_->worldTransform_.translate_;
//
//    // 目標方向を計算
//    Vector3 toTarget = desiredPos - currentPos;
//    toTarget.y = 0.0f;
//
//    // 距離が小さければ止まる
//    float distance = Length(toTarget);
//    if (distance < 2.0f) {
//        velocity_ = { 0.0f, 0.0f, 0.0f };
//        return;
//    }
//
//    // 加速度的に追いかける
//    Vector3 direction = Normalize(toTarget);
//    float acceleration = 0.01f;
//    float maxSpeed = 0.01f;
//
//    velocity_ += direction * acceleration;
//    velocity_ = ClampLength(velocity_, maxSpeed); // 速度制限
//    // 移動
//    currentPos += velocity_;
//}
//
//Vector3 ClampLength(const Vector3& v, float maxLength) { 
//    float len = Length(v);
//    if (len > maxLength) { 
//        return Normalize(v) * maxLength; }
//    return v;
//}

Medjed::Medjed() {

    model_ = ModelManager::GetModel("normalMed.gltf");

    aniObj_ = std::make_unique<AnimationObject3d>();
    aniObj_->Create();

    skinningModel = std::make_unique<SkinningModel>();
    skinningModel->CreateDatas(model_, model_);
    aniObj_->SetMeshAndData(skinningModel.get());

    SetType(kAABB);
    SetAABB(localAABB_);
    SetCollisionAttribute(kCollisionMedjed);
    SetCollisionMask(kCollisionPlayer | kCollisionWall | kCollisionDummyMedjed);

}

void Medjed::Look(const Vector3& target)
{
    Vector3 direction = target - GetWorldPosition();
    aniObj_->worldTransform_.rotate_.y = std::atan2(direction.x, direction.z); // Y軸回転（ラジアン）
}

void Medjed::Update()
{

    if (IsHide()) {
        return;
    }

    isHit_ = false;
    if (isFind_) {
        aniTimer_ += kInverseFPS * 0.25f;
        aniTimer_ = std::clamp(aniTimer_, 0.0f, 1.0f);
        aniObj_->SetColor({ 1.0f,1.0f,1.0f,Easing::EaseInOut(0.0f,1.0f,aniTimer_) });
    } else {

        SetColor({ 1.0f,1.0f,1.0f,0.0f });
//#ifdef _DEBUG
//
//        SetColor({ 1.0f,0.0f,0.0f,1.0f });
//#endif // _DEBUG

    }

    aniObj_->UpdateAniTimer();
    aniObj_->Update();
    ColliderUpdate();
}
void Medjed::Init()
{

    velocity_ = { 1.0f,0.0f,1.0f };
    hideTimer_ = 0.0f;
    aniTimer_ = 0.0f;
    isHit_ = false;
    isFind_ = false;
    aniObj_->Initialize();
    aniObj_->SetAnimation(model_);
    aniObj_->SetColor({ 1.0f,1.0f,1.0f,0.0f });
}

void Medjed::Draw(Camera& camera)
{
    if (IsHide()) {
        return;
    }
    /*   ColliderDraw(camera);*/
    aniObj_->Draw(camera);


}
