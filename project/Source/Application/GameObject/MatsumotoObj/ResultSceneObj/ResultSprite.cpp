#include "ResultSprite.h"
#include"Easing.h"
#include"DebugUI.h"
#include"Input.h"
#include "MatsumotoObj/SceneStaticValue.h"
#include"Window.h"//画面サイズを取得するためにインクルードしています。

ResultSprite::ResultSprite() {
	sprite_ = std::make_unique<Sprite>();
	sprite_->Create(Texture::RESULT_CLEAR, { 640.0f,360.0f });
	//サイズを画面サイズと合わせます。ヨシダ
	sprite_->SetSize({ static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()) });
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
}

void ResultSprite::Initialize() {

	if (SceneStaticValue::isClear) {
		sprite_->SetTexture(Texture::RESULT_CLEAR);
	} else {
		sprite_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		sprite_->SetTexture(Texture::WHITE_1X1);
	}
}

void ResultSprite::Update() {
	

	sprite_->Update();
}

void ResultSprite::Draw() {
	Sprite::PreDraw(kBlendModeNormal);
	sprite_->Draw();
}
