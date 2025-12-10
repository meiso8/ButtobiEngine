#include<algorithm>
#include "HPIcon.h"
#include"Easing.h"
#include"DebugUI.h"
#include"Input.h"

HPIcon::HPIcon() {

	shake = std::make_unique<Shake>();
}

void HPIcon::Setting(const Vector2& size, const Vector2& pos) {

	for (int i = 0; i < maxIcon_; ++i) {
		sprites_[SpriteTypes::layer1].emplace_back(std::make_unique<Sprite>());
		sprites_[SpriteTypes::MaxHp].emplace_back(std::make_unique<Sprite>());
	}

	for (auto& [type, spriteList] : sprites_) {
		for (auto& sprite : spriteList) {
			sprite->Create(textureHandle_, { 64.0f,128.0f }, { 0.0f,0.0f,0.0f,0.5f });
		}
	}

	for (auto& [type, spriteList] : sprites_) {
		for (size_t i = 0; i < spriteList.size(); ++i) {

			const float spacing = 16.0f; // スプライト同士の間隔

			Vector2 offsetPos = {
				pos.x + i * (size.x + spacing),
				pos.y
			};
			spriteList[i]->SetSize(size);
			spriteList[i]->SetPosition(offsetPos);

			position_.push_back(offsetPos);
			spriteList[i]->SetTexture(textureHandle_);

		}
	}
}

void HPIcon::Initialize() {
	timer_ = 0.0f;
	preHP = hps_->hp;

	for (auto& sprite : sprites_[layer1]) {
		sprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	drawHpNum_ = maxIcon_;
	shake->Reset();
}

void HPIcon::Update() {


	if (hps_->hp > 0) {

		if (hps_->hp != preHP) {
			timer_ = maxTime_;
		}

		preHP = hps_->hp;

		if (timer_ > 0.0f) {
			timer_ -= InverseFPS;
			theta_ += std::numbers::phi_v<float>*InverseFPS * 12.0f;
			
		} else {
			timer_ = 0.0f;
			theta_ = 0.0f;
			drawHpNum_ = static_cast<size_t>(hps_->hp / (hps_->maxHp / static_cast<float>(maxIcon_)) + 0.9f);
			drawHpNum_ = static_cast<size_t>(std::clamp(static_cast<float>(drawHpNum_), 0.0f, static_cast<float>(maxIcon_)));
		}

		float alpha = cosf(theta_) * 0.5f + 1.0f;
		sprites_[layer1][(drawHpNum_ - 1) % maxIcon_]->SetColor({ 1.0f,1.0f,1.0f,alpha });

		HitAction();

	} else {

		drawHpNum_ = 0;

	}




	int num = (int)drawHpNum_;
	DebugUI::CheckFloat(timer_, "ICONTimer");
	DebugUI::CheckInt(num, "num");


}

void HPIcon::Draw() {

	Sprite::PreDraw();

	for (auto& sprite : sprites_[MaxHp]) {
		sprite->Draw();
	}


	for (size_t i = 0; i < drawHpNum_; ++i) {
		sprites_[layer1][i]->Draw();
	}


}

void HPIcon::HitAction()
{
	if (isHitPtr_&& *isHitPtr_) {
		shake->Start(40.0f, 15);
	}
	
	shake->Update();

	for (size_t i = 0; i < drawHpNum_; ++i) {
		sprites_[MaxHp][i]->SetPosition(position_[i] + shake->GetOffset());
		sprites_[layer1][i]->SetPosition(position_[i] + shake->GetOffset());
	}

}

HPIcon::~HPIcon()
{
	position_.clear();
}
