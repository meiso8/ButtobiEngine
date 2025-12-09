#include "Nest.h"

#include"Model.h"
#include"Camera.h"
#include"Input.h"
#include"Texture.h"
#include"DebugUI.h"
Nest::Nest() {
    model_ = ModelManager::GetModel(ModelManager::NEST);

    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetLightMode(kLightModeHalfL);
    object3d_->SetMesh(model_);
};

void Nest::Init()
{
    object3d_->Initialize();
    object3d_->worldTransform_.scale_ = { 6.0f,6.0f,6.0f };
    object3d_->worldTransform_.translate_.z = 10.0f;
    //model_->ResetTextureHandle();
}

void Nest::Update()
{
    object3d_->Update();
    DebugUI::CheckObject3d(*object3d_, "Nest");
}

void Nest::Draw(Camera& camera)
{
    object3d_->Draw(camera, kBlendModeNormal);
}
