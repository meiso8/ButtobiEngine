#include "SkyDome.h"

#include"Model.h"
#include"Camera.h"
#include"Input.h"
#include"Texture.h"
#include"DebugUI.h"

SkyDome::SkyDome() {
    model_ = ModelManager::GetModel(ModelManager::WORLD);

    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetLightMode(kLightModeNone);
    object3d_->SetMesh(model_);
};

void SkyDome::Init()
{
    object3d_->Initialize();
}

void SkyDome::Update()
{
    object3d_->UpdateUV();
    object3d_->GetUVTranslate().x += InverseFPS;
    object3d_->Update();
    DebugUI::CheckObject3d(*object3d_, "SkyDome");
}

void SkyDome::Draw(Camera& camera)
{
    object3d_->Draw(camera, kBlendModeNormal);
}
