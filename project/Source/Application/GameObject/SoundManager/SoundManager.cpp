#include "SoundManager.h"
#include"Sound.h"
void SoundManager::PlayCorrectSE()
{
    Sound::PlaySE(Sound::CRACKER);
}

void SoundManager::PlayCancelSE()
{
    Sound::PlaySE(Sound::PICO);
}
