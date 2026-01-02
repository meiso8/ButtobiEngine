#include "SoundManager.h"
#include"Sound.h"
#include"Window.h"

void SoundManager::InitMedjedScene()
{
    Sound::bgmVolume_ = 0.1f;
    Sound::StopAllSound();
}

void SoundManager::PlayCorrectSE()
{
    Sound::PlayOriginSE(Sound::CORRECT);
}


void SoundManager::PlayGOGOGOSE()
{
    Sound::PlayOriginSE(Sound::GOGOGO);
}


void SoundManager::PlayCancelSE()
{
    Sound::PlaySE(Sound::BUZZER);
}

void SoundManager::ApperMedjedUpdate()
{
    if (Sound::bgmVolume_ < 0.5f) {
        Sound::bgmVolume_ += InverseFPS * 0.25f;
    }
    Sound::Stop(Sound::BGM_Sea);
    Sound::PlayBGM(Sound::BGM_ArabRuins);
}

void SoundManager::NotFindMedjedUpdate()
{
    Sound::Stop(Sound::BGM_ArabRuins);
    Sound::PlayBGM(Sound::BGM_Sea);
}
