#include"Texture.h"
#include"TextureManager.h"
#include"ModelManager.h"

std::vector<uint32_t>Texture::textureHandle_;
Texture* Texture::instance_ = nullptr;

Texture* Texture::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new Texture();
    }
    return instance_;
}

void Texture::Load()
{
    TextureManager::GetInstance()->Initialize();
    textureHandle_.resize(TEXTURES);

    textureHandle_[WHITE_1X1] = TextureManager::Load("resources/white1x1.png");
    textureHandle_[UV_CHECKER] = TextureManager::Load("resources/uvChecker.png");
    textureHandle_[NUMBERS] = TextureManager::Load("resources/numbers.png");
    textureHandle_[PLAYER] = TextureManager::Load("resources/player.png");

    textureHandle_[LIFE] = TextureManager::Load("resources/UI/life.png");
    textureHandle_[SCORE] = TextureManager::Load("resources/UI/score.png");
    textureHandle_[COMBO] = TextureManager::Load("resources/UI/combo.png");
    textureHandle_[SPEED_BONUS] = TextureManager::Load("resources/UI/speedBonus.png");
    textureHandle_[WASD] = TextureManager::Load("resources/UI/WASD.png");
    textureHandle_[SPACE] = TextureManager::Load("resources/UI/space.png");
    textureHandle_[TIMER] = TextureManager::Load("resources/UI/timer.png");
    textureHandle_[JUICE] = TextureManager::Load("resources/UI/juice.png");

}
