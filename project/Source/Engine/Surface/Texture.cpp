#include"Texture.h"
#include"TextureManager.h"
#include"ModelManager.h"

std::vector<uint32_t>Texture::handle_;

void Texture::LoadAllTexture()
{
    handle_.resize(TEXTURES);

    handle_[WHITE_1X1] = TextureManager::Load("resources/white1x1.png");
    handle_[UV_CHECKER] = TextureManager::Load("resources/uvChecker.png");
    handle_[NUMBERS] = TextureManager::Load("resources/numbers.png");
    handle_[PLAYER] = TextureManager::Load("resources/player.png");

    handle_[LIFE] = TextureManager::Load("resources/UI/life.png");
    handle_[SCORE] = TextureManager::Load("resources/UI/score.png");
    handle_[COMBO] = TextureManager::Load("resources/UI/combo.png");
    handle_[SPEED_BONUS] = TextureManager::Load("resources/UI/speedBonus.png");
    handle_[WASD] = TextureManager::Load("resources/UI/WASD.png");
    handle_[SPACE] = TextureManager::Load("resources/UI/space.png");
    handle_[TIMER] = TextureManager::Load("resources/UI/timer.png");
    handle_[JUICE] = TextureManager::Load("resources/UI/juice.png");
    handle_[PARTICLE] = TextureManager::Load("resources/particle.png");
}
