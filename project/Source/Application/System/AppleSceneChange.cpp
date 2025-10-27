#include "AppleSceneChange.h"
#include"Sprite.h"
#include"Texture.h"
#include"Easing.h"

void AppleSceneChange::Initialize()
{
    appleSprite_ = std::make_unique<Sprite>();
 
    worldTransform_.Initialize();
    worldTransform_.translate_ = { 640.0f,360.0f };
    appleSprite_->Create(Texture::GetHandle(Texture::APPLE_SCENE_CHANGE), worldTransform_.translate_, worldTransform_.scale_);
    appleSprite_->SetAnchorPoint({ 0.5f,0.5f });
}


void AppleSceneChange::Update(const float& timer)
{
    worldTransform_.scale_ = Easing::EaseInOut(Vector2{ 1.0f,1.0f }, { 3.0f,3.0f }, timer);
    appleSprite_->SetSize(worldTransform_.scale_*size_);
    appleSprite_->Update();

}

void AppleSceneChange::Draw()
{
    appleSprite_->PreDraw(kBlendModeMultiply);
    appleSprite_->Draw();
}
