#include "SoundManager.h"
#include"Sound.h"
void SoundManager::PlayCorrectSE()
{
    Sound::PlaySE(Sound::CORRECT);
}

void SoundManager::PlayCancelSE()
{
    Sound::PlaySE(Sound::PICO);
}
