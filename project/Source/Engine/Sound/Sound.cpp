#include "Sound.h"
#include"SoundManager.h"

std::vector<uint32_t>Sound::handle_;

float Sound::bgmVolume_ = 0.5f;
float Sound::seVolume_ = 0.5f;

void Sound::LoadAllSound()
{
    handle_.resize(SOUNDS);

    //サウンドの読み込み
    handle_[BGM1] = SoundManager::Load("resources/Sounds/fruit_mix.mp3");
    handle_[BGM2] = SoundManager::Load("resources/Sounds/dreamcore.mp3");
    handle_[SE1] = SoundManager::Load("resources/Sounds/pico.mp3");
    handle_[SE2] = SoundManager::Load("resources/Sounds/cracker.mp3");
}

void Sound::PlayBGM(const uint32_t handle, const float& volumeOffset)
{
    if (!SoundManager::IsPlaying()) {
        SoundManager::Play(handle_[handle], bgmVolume_ + volumeOffset, true);
    }
}

void Sound::PlaySE(const uint32_t handle, const float& volumeOffset)
{
    SoundManager::Play(handle_[handle], seVolume_ + volumeOffset, false);
}

