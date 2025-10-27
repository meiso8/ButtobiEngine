#include "Shutter.h"
#include"Sprite.h"
#include"Texture.h"
#include"Easing.h"
#include"Sound.h"

void Shutter::Initialize()
{
    shutterWorldTransform_.Initialize();
    shutterWorldTransform_.scale_ = { 1280.0f,720.0f };
    closeWordWorldTransform_.Initialize();
    closeWordWorldTransform_.parent_ = &shutterWorldTransform_;
    closeWordWorldTransform_.scale_ = { 1.0f,1.0f };
    closeWordWorldTransform_.translate_ = { 640,360.0f };
    isAnnounce_ = false;
}

void Shutter::Create()
{
    shutterSprite_ = std::make_unique<Sprite>();
    closeWordSprite_ = std::make_unique<Sprite>();

    Initialize();

    shutterSprite_->Create(
        Texture::GetHandle(Texture::SHUTTER),
        shutterWorldTransform_.translate_,
        shutterWorldTransform_.scale_
    );

    closeWordSprite_->Create(
        Texture::GetHandle(Texture::CLOSED),
        closeWordWorldTransform_.translate_,
        closeWordWorldTransform_.scale_
    );

    closeWordSprite_->SetAnchorPoint({ 0.5f,0.5f });
    closeWordSprite_->Update();
}

void Shutter::Draw()
{
    shutterSprite_->PreDraw();
    shutterSprite_->Draw();
    if (isAnnounce_) {
        closeWordSprite_->Draw();
    }

}

void Shutter::Close(const float& timer)
{
    EasingTransformY(-720.0f, 0.0f, timer);
    if (timer <= 1.0f) {
        closeWordWorldTransform_.scale_ =
            Easing::EaseInBack(Vector2{1.0f,1.0f}, {1.5f,1.5f}, timer);
        closeWordSprite_->SetSize(wordSize_ * closeWordWorldTransform_.scale_);
        closeWordSprite_->Update();
    }
    if (!isAnnounce_) {
        if (timer > 0.5f) {
            Sound::PlayOriginSE(Sound::ANNOUNCE_CLOSE);
            isAnnounce_ = true;
        }
    }

}

void Shutter::Open(const float& timer)
{
    EasingTransformY(0.0f, -720.0f, timer);
    if (isAnnounce_) {
        if (timer > 0.2f) {
            isAnnounce_ = false;
        }
    }
}

void Shutter::EasingTransformY(const float& start, const float& end, const float& timer)
{
    if (timer > 1.0f) {
        return;
    }
    shutterWorldTransform_.translate_.y =
        Easing::EaseOutSine(start, end, timer);
    shutterSprite_->SetPosition(shutterWorldTransform_.translate_);
}

