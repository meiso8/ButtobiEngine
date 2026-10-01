#include "Skip.h"
#include"Sprite.h"

Skip::Skip()
{
    skipSprite_ = std::make_unique<Sprite>();
    skipSprite_->Create(TextureFactory::Handle::SKIP, { 1280.0f - 128.0f - 64.0f, 720.0f - 64.0f });
    skipSprite_->SetAnchorPoint({ 0.5f, 0.5f });
}

Skip::~Skip()
{
}

void Skip::Initialize()
{
    isSkipDraw_ = false;
}

void Skip::Draw()
{
    if (isSkipDraw_) {
        skipSprite_->Draw();
    }
}
