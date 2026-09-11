#include "EyeCatch.h"
#include"Sprite.h"
#include"TimeManager.h"

EyeCatch::EyeCatch()
{

    const float height = static_cast<float>(Window::GetClientHeight());
    const float whidth = static_cast<float>(Window::GetClientWidth());
    const float halfWidth = whidth * 0.5f;
    const float halfHeight = height * 0.5f;

    std::string directoryName = "Resource/Textures/EyeCatch/eyeCatch_";

    for (int32_t i = 0; i < sprites_.size(); ++i) {
        textureHandles_[i] = Texture::LoadAndGetIndex(directoryName + std::to_string(i) + ".png");
        sprites_[i] = std::make_unique<Sprite>();
        sprites_[i]->Create(TextureFactory::UV_CHECKER,Vector2{ halfWidth, halfHeight });
        sprites_[i]->SetSRVTextureHandle(textureHandles_[i]);
        sprites_[i]->SetAnchorPoint({ 0.5f,0.5f });
        sprites_[i]->AdjustTextureSize();
        sprites_[i]->Update();
    }
}

EyeCatch::~EyeCatch()
{
}

void EyeCatch::Initialize()
{
    animationTimer_ = 0.0f;
    animeCount_ = 0;
    isAnimeEnd_ = false;
}

void EyeCatch::Update()
{
    if (isAnimeEnd_) {
        //アニメ終了時リターンする
        return;
    }
    animationTimer_ += TimeManager::DeltaTime();

    if (animationTimer_ >= 0.0625f) {
        //1秒ごとに
        if (animeCount_ < sprites_.size() - 1) {
            animeCount_++;
            //アニメーションタイマーを初期化する
            animationTimer_ = 0.0f;
        } else {
            isAnimeEnd_ = true;
        }
    }


}

void EyeCatch::Draw()
{
    if (isAnimeEnd_) {
        //アニメ終了時リターンする
        return;
    }
    auto& sprite = sprites_[animeCount_];
    if (sprite) {
        //安全処理
        sprite->Draw();
    }
}
