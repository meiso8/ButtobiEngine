#include"Texture.h"
#include"TextureManager.h"
#include"ModelManager.h"

std::vector<uint32_t>Texture::handle_;

void Texture::LoadAllTexture()
{
    handle_.resize(TEXTURES);

    handle_[WHITE_1X1] = TextureManager::Load("Resources/white1x1.png");
    handle_[UV_CHECKER] = TextureManager::Load("Resources/uvChecker.png");
    handle_[NUMBERS] = TextureManager::Load("Resources/numbers.png");
    handle_[PLAYER] = TextureManager::Load("Resources/player.png");

    handle_[LIFE] = TextureManager::Load("Resources/UI/life.png");
    handle_[SCORE] = TextureManager::Load("Resources/UI/score.png");
    handle_[COMBO] = TextureManager::Load("Resources/UI/combo.png");
    handle_[SPEED_BONUS] = TextureManager::Load("Resources/UI/speedBonus.png");
    handle_[WASD] = TextureManager::Load("Resources/UI/WASD.png");
    handle_[SPACE] = TextureManager::Load("Resources/UI/space.png");
    handle_[TIMER] = TextureManager::Load("Resources/UI/timer.png");
    handle_[JUICE] = TextureManager::Load("Resources/UI/juice.png");
    handle_[PARTICLE] = TextureManager::Load("Resources/particle.png");
}
