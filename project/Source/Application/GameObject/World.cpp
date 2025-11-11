#include "World.h"
#include"Model.h"
#include"Camera.h"
#include"Input.h"
#include"Texture.h"

World::World() {
    model_ = ModelManager::GetModel(ModelManager::WORLD);
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetMesh(model_);
};

void World::Init()
{
    object3d_->Initialize();
    model_->ResetTextureHandle();
}

void World::Update()
{
 /*   model_->GetUVTransform().translate.x += std::numbers::pi_v<float> *0.0625f * InverseFPS;
    model_->UpdateUV();*/

    if (Input::IsTriggerKey(DIK_SPACE)) {
        texture_++;
        texture_ %= Texture::TEXTURES;
        SetTexture(texture_);
    }

}

void World::Draw(Camera& camera)
{
    object3d_->Draw(camera, klightModeNone);
}

void World::SetTexture(const uint32_t& texture)
{
    if (texture == NORMAL) {
        model_->ResetTextureHandle();
    } else {
        model_->SetTextureHandle(Texture::GetHandle(texture));
    }
  
}
