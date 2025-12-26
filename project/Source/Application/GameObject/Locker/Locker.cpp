#include "Locker.h"
#include"ModelManager.h"
#include"Model.h"
#include"CollisionConfig.h"
Locker::Locker()
{
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();

    SetType(kAABB);
    AABB AABB = { .min = {-0.5f,0.0f,-0.5f},.max = {0.5f,2.0f,0.5f} };

    SetAABB(AABB);
    SetCollisionAttribute(kCollisionLocker);
    SetCollisionMask(kCollisionPlayer);
}
void Locker::Init()
{
    isSetMesh_ = false;
    object3d_->Initialize();
    object3d_->worldTransform_.rotate_.y = std::numbers::pi_v<float>;
    model_ = ModelManager::GetModel(ModelManager::normalMedjed_GLTF);
    object3d_->SetMesh(model_);

}

void Locker::Draw(Camera& camera)
{
    object3d_->SetLightMode(kLightModeHalfL);
    object3d_->Draw(camera);

    ColliderDraw(camera);
}

void Locker::Update()
{

    UpdateSetMesh();
    object3d_->Update();
    ColliderUpdate();

}

void Locker::UpdateSetMesh()
{
    if (isSetMesh_) {
        model_ = ModelManager::GetModel(ModelManager::MUMMY);
        object3d_->SetMesh(model_);
    }

}

void Locker::OnCollision(Collider* collider)
{
    OnCollisionCollider();
}

Vector3 Locker::GetWorldPosition() const
{
    return object3d_->worldTransform_.GetWorldPosition();
}
