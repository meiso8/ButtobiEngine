#include "Locker.h"
#include"ModelManager.h"
#include"Model.h"

Locker::Locker()
{
    model_ = ModelManager::GetModel(ModelManager::LOCKER);

    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();

    object3d_->SetMesh(model_);
}



void Locker::Init()
{
    object3d_->Initialize();
    object3d_->worldTransform_.translate_.y = 1.0f;
}

void Locker::Draw(Camera& camera)
{

    object3d_->Draw(camera);



}

void Locker::Update()
{


    object3d_->Update();


}
