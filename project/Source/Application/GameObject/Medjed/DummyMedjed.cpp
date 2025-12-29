#include "DummyMedjed.h"
#include"ModelManager.h"
#include"Model.h"
#include"CollisionConfig.h"
#include"Random.h"
#include"Easing.h"
#include"Window.h"
#include<algorithm>
void DummyMedjed::LookTarget(const Vector3& target)
{

    aniTimer_ += InverseFPS *0.5f;
    aniTimer_ = std::clamp(aniTimer_, 0.0f, 1.0f);

    Vector3 direction = target - GetWorldPosition();
    object3d_->worldTransform_.rotate_.y = Easing::EaseInOutBounce(startRotateY_, std::atan2(direction.x, direction.z), aniTimer_);
}

DummyMedjed::DummyMedjed()
{
    model_ = ModelManager::GetModel(ModelManager::normalMedjed_GLTF);

    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetMesh(model_);

    SetType(kAABB);
    SetAABB(localAABB_);
    SetCollisionAttribute(kCollisionDummyMedjed);
    SetCollisionMask(kCollisionPlayer);

}
void DummyMedjed::Init()
{

    aniTimer_ = 0.0f;
    object3d_->Initialize();
    Random::SetMinMax(0.0f, rotateRange_);
    startRotateY_ = Random::Get();
    object3d_->worldTransform_.rotate_.y = startRotateY_;

}

void DummyMedjed::Draw(Camera& camera)
{
    object3d_->SetLightMode(kLightModeHalfL);
    object3d_->Draw(camera);

    ColliderDraw(camera);
}

void DummyMedjed::Update()
{
    object3d_->Update();
    ColliderUpdate();
}


void DummyMedjed::OnCollision(Collider* collider)
{
    OnCollisionCollider();
}

Vector3 DummyMedjed::GetWorldPosition() const
{
    return object3d_->worldTransform_.GetWorldPosition();
}
