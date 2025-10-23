#include"Texture.h"
#include"TextureManager.h"
#include"ModelManager.h"

std::vector<uint32_t>Texture::handle_;

void Texture::LoadAllTexture()
{
    handle_.resize(TEXTURES);

    handle_[WHITE_1X1] = TextureManager::Load("Resources/Textures/white1x1.png");
    handle_[UV_CHECKER] = TextureManager::Load("Resources/Textures/uvChecker.png");
    handle_[NUMBERS] = TextureManager::Load("Resources/Textures/numbers.png");

    handle_[LIFE] = TextureManager::Load("Resources/Textures/UI/life.png");
    handle_[SCORE] = TextureManager::Load("Resources/Textures/UI/score.png");
    handle_[COMBO] = TextureManager::Load("Resources/Textures/UI/combo.png");
	handle_[COMBONUMBERS] = TextureManager::Load("Resources/Textures/UI/ComboNumber.png");
    handle_[SPEED_BONUS] = TextureManager::Load("Resources/Textures/UI/speedBonus.png");
	handle_[SPEED_BONUSNUMBERS] = TextureManager::Load("Resources/Textures/UI/SpeedNumber.png");
    handle_[WASD] = TextureManager::Load("Resources/Textures/UI/WASD.png");
    handle_[SPACE] = TextureManager::Load("Resources/Textures/UI/space.png");
    handle_[TIMER] = TextureManager::Load("Resources/Textures/UI/timer.png");
    handle_[TIMERNUMBERS] = TextureManager::Load("Resources/Textures/UI/timerNumbers.png");
    handle_[JUICE] = TextureManager::Load("Resources/Textures/UI/juice.png");
    handle_[PARTICLE] = TextureManager::Load("Resources/Textures/particle.png");
}

uint32_t Texture::AddTextureHandle(const std::string& filePath)
{
    uint32_t handle = TextureManager::Load(filePath);

    auto it = std::find_if(
        handle_.begin(),
        handle_.end(),
        [&](uint32_t existingHandle) { return existingHandle == handle; }
    );

    if (it != handle_.end()) {
        return static_cast<uint32_t>(std::distance(handle_.begin(), it));
    }

    handle_.push_back(handle);
    return static_cast<uint32_t>(Texture::handle_.size() - 1);;

}

uint32_t Texture::AddHandleForSRV(const uint32_t& textureHandle)
{
    //タミーハンドルを追加
    handle_.push_back(textureHandle);
    return static_cast<uint32_t>(Texture::handle_.size() - 1);;

}
