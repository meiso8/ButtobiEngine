#include "RaySprite.h"

RaySprite::RaySprite()
{
    sprite_ = std::make_unique<Sprite>();
    sprite_->Create(Texture::EYE, { 0.0f,0.0f });
    sprite_->SetAnchorPoint({ 0.5f,0.5f });

    const float width = float(Window::GetClientWidth());
    const float height = float(Window::GetClientHeight());
    sprite_->SetPosition({ width * 0.5f,height * 0.5f });
}

void RaySprite::Draw()
{
    Sprite::PreDraw();
    sprite_->Draw();
}

bool RaySprite::IntersectsAABB(const AABB& aabb)
{
    if (RayIntersectsAABB(ray_, aabb, tMin_, tMax_)) {
        sprite_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
        return true;
    }
    //当たってないときα値を下げる
    sprite_->SetColor({ 1.0f,1.0f,1.0f,0.5f });
    return false;
}
