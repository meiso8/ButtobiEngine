#include "Medjed.h"
#include"ModelManager.h"
#include"Model.h"
#include"MakeMatrix.h"
Medjed::Medjed() {

    model_ = ModelManager::GetModel(ModelManager::MEDJED);
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetMesh(model_);


    aniObj_ = std::make_unique<AnimationObject3d>();
    aniObj_->Create();
    aniObj_->SetMeshAndData(ModelManager::GetModel(ModelManager::Ani_GLTF));

}


void Medjed::LookTarget()
{

    Vector3 direction = *targetPos_- GetWorldPos();
    object3d_->worldTransform_.rotate_.y = std::atan2(direction.x, direction.z); // Y軸回転（ラジアン）
    aniObj_->worldTransform_.rotate_.y=   object3d_->worldTransform_.rotate_.y;
}


void Medjed::Update()
{
    LookTarget();
    object3d_->Update();

    aniObj_->Update();

}
void Medjed::Init()
{
    object3d_->Initialize();
}

void Medjed::Draw(Camera& camera)
{
    //object3d_->SetLightMode(kLightModeHalfL);
    //object3d_->Draw(camera);

    aniObj_->Draw(camera);
}
