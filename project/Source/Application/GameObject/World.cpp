#include "World.h"
#include"Model.h"
#include"Camera.h"
#include"Input.h"
#include"Texture.h"

World::World() {
    model_ = ModelManager::GetModel(ModelManager::WORLD);

    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetLightMode(kLightModeNone);
    object3d_->SetMesh(model_);
};

void World::Init()
{
    object3d_->SetColor({0.0f,0.0f,0.0f,1.0f});
    object3d_->Initialize();
    model_->ResetTextureHandle();
    timer_ = 0.0f;
}

void World::Update()
{
    object3d_->GetUVTransform().translate.x += std::numbers::pi_v<float> *0.0625f*0.5f * InverseFPS;
    object3d_->UpdateUV();

    //if (Input::IsTriggerKey(DIK_SPACE)) {
    //    texture_++;
    //    texture_ %= Texture::TEXTURES;
    //    SetTexture(texture_);
    //}

}

void World::Draw(Camera& camera)
{
    object3d_->Draw(camera,kBlendModeNormal);
}

void World::SetTexture(const uint32_t& texture)
{
    if (texture == NORMAL) {
        model_->ResetTextureHandle();
    } else {
        model_->SetTextureHandle(Texture::TEXTURE_HANDLE(texture));
    }
}

void World::UpdateColor()
{
    if (timer_ == 1.0f) {
        return;
    }

    if (timer_ < 1.0f) {
        timer_+=InverseFPS;
    } else {
        timer_ = 1.0f;
    }

    object3d_->SetColor({ timer_,timer_,timer_,1.0f });

}
