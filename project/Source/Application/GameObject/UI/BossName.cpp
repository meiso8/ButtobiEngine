#include "BossName.h"

#include"DebugUI.h"
BossName::BossName()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->Create(Texture::KARAMATSU, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
	sprite_->SetScale(Vector2{ 0.75f, 0.75f});
}

void BossName::Draw()
{
	Sprite::PreDraw();
	sprite_->Draw();
}

void BossName::SetPosition(const Vector2& pos)
{
	sprite_->SetPosition(pos);
}

