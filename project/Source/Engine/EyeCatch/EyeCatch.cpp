#include "EyeCatch.h"
#include"Sprite.h"
#include"TimeManager.h"

EyeCatch::EyeCatch()
{

    const float height = static_cast<float>(Window::GetClientHeight());
    const float width = static_cast<float>(Window::GetClientWidth());
    const float halfWidth = width * 0.5f;
    const float halfHeight = height * 0.5f;
    Texture::LoadAndMapHandle("Resource/Textures/white1x1.png", TextureFactory:: WHITE_1X1);
    std::string directoryName = "Resource/Textures/EyeCatch/eyeCatch_";

    for (int32_t i = 0; i < sprites_.size(); ++i) {
        textureHandles_[i] = Texture::LoadAndGetIndex(directoryName + std::to_string(i) + ".png");
        sprites_[i] = std::make_unique<Sprite>();
        sprites_[i]->Create(TextureFactory::WHITE_1X1,Vector2{ halfWidth, halfHeight });
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

void EyeCatch::Update(bool isLoop)
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
            if (isLoop) {
                Initialize();
            } else {
                isAnimeEnd_ = true;
            }
  
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
