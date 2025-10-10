#include "Sound.h"
#include"SoundManager.h"

std::vector<uint32_t>Sound::soundHandle_;

void Sound::LoadAllSound()
{
    soundHandle_.resize(SOUNDS);

    //サウンドの読み込み
    soundHandle_[BGM1] = SoundManager::Load(L"resources/Sounds/dreamcore.mp3");
    soundHandle_[BGM2] = SoundManager::Load(L"resources/Sounds/kiritan.mp3");
    soundHandle_[SE1] = SoundManager::Load(L"resources/Sounds/poppo.mp3");
    soundHandle_[SE2] = SoundManager::Load(L"resources/Sounds/broken.mp3");
}
