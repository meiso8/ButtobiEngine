#include "Ground.h"

#include"Model.h"
#include"Camera.h"
#include"Input.h"
#include"Texture.h"
#include"DebugUI.h"
Ground::Ground() {
    model_ = ModelManager::GetModel(ModelManager::GROUND);
  
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetLightMode(kLightModeHalfL);
    object3d_->SetMesh(model_);
};

void Ground::Init()
{
    object3d_->Initialize();
    object3d_->worldTransform_.translate_.y = { -20.f };
    object3d_->worldTransform_.scale_ = { 5.0f,1.0f,5.0f };
    model_->ResetTextureHandle();
}

void Ground::Update()
{
    object3d_->Update();
    DebugUI::CheckObject3d(*object3d_,"Ground");
}

void Ground::Draw(Camera& camera)
{
    object3d_->Draw(camera, kBlendModeNormal);
}
