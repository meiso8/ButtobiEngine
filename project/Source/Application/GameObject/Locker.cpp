#include "Locker.h"
#include"ModelManager.h"
#include"Model.h"

Locker::Locker()
{
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();

}
void Locker::Init()
{
    isSetMesh_ = false;
    object3d_->Initialize();
    object3d_->worldTransform_.rotate_.y = std::numbers::pi_v<float>;
    model_ = ModelManager::GetModel(ModelManager::MEDJED);
    object3d_->SetMesh(model_);

}

void Locker::Draw(Camera& camera)
{
    object3d_->SetLightMode(kLightModeHalfL);
    object3d_->Draw(camera);
}

void Locker::Update()
{

    UpdateSetMesh();
    object3d_->Update();


}

void Locker::UpdateSetMesh()
{
    if (isSetMesh_) {
        model_ = ModelManager::GetModel(ModelManager::MUMMY);
        object3d_->SetMesh(model_);
    } 

}
