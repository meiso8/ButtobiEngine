#include "SkyDome.h"

#include"Model.h"
#include"Camera.h"
#include"Input.h"
#include"Texture.h"
#include"DebugUI.h"


SkyDome::SkyDome() {

    sphere_ = std::make_unique<SphereMesh>();
    sphere_->Create();
    sphere_->SetTextureHandle(Texture::SKY_DOME);
    
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetLightMode(kLightModeNone);
    object3d_->SetMesh(sphere_.get());
};

void SkyDome::Init()
{
    object3d_->Initialize();
    object3d_->worldTransform_.scale_ = { 200.0f,50.0f,200.0f };
    object3d_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

void SkyDome::Update()
{
    object3d_->GetUVTranslate().x -= InverseFPS* 0.0078125f;
    object3d_->UpdateUV();
    object3d_->Update();
    DebugUI::CheckObject3d(*object3d_, "SkyDome");
}

void SkyDome::Draw(Camera& camera)
{
    object3d_->Draw(camera, kBlendModeNormal,kCullModeFront);
}

void SkyDome::SetColor(const Vector4& color)
{
    object3d_->SetColor(color);
}
