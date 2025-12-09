#include "FlashEffecter.h"
#include "MatsumotoObj/MY_Utility.h"
void FlashEffecter::Initialize()
{
	flashSprite_ = std::make_unique<Sprite>();
	flashSprite_->Create(Texture::TEXTURE_HANDLE::WHITE_1X1, { 1280.0f,720.0f });
	flashSprite_->SetPosition({ 640.0f,360.0f });
	flashSprite_->SetAnchorPoint({ 0.5f,0.5f });
}

void FlashEffecter::Update()
{
	if (flashTimer_ > 0.0f) {
		flashTimer_ -= 0.016f;
		blinkTimer_ -= 0.016f;
		if (blinkTimer_ <= 0.0f) {
			isVisible_ = !isVisible_;
			blinkTimer_ = flashDuration_; // 点滅間隔でリセット
		}
	}
	else {
		isVisible_ = false;
	}
	flashSprite_->Update();
}

void FlashEffecter::Draw()
{
	if (flashTimer_ <= 0.0f || !isVisible_) return;

	flashSprite_->PreDraw();
	flashSprite_->Draw();
}

void FlashEffecter::StartFlash(const Vector4& color, float time, float duration)
{
	flashTimer_ = time;
	flashDuration_ = duration; // 点滅間隔として使用
	flashSprite_->SetColor(color);
	isVisible_ = true;
	blinkTimer_ = duration; // 初期化
}


void FlashEffecter::Finalize()
{
	flashSprite_.reset();
}
