#include "VoiceSystem.h"
#include"Sound.h"

std::string VoiceSystem::fileName_ = {""};
float VoiceSystem::volume_ = 1.0f;
bool VoiceSystem::isEnd_ = false;



void VoiceSystem::Update()
{

    if (Sound::IsPlaying(fileName_)) {
        return;
    }

    //プレイ終了時、アンロードする
    Unload();
}

bool VoiceSystem::IsEnd()
{
    return isEnd_;
}

void VoiceSystem::Stop()
{
    Sound::Stop(fileName_);
}

void VoiceSystem::Unload()
{   
    Sound::Stop(fileName_);
    //プレイ終了時、アンロードする
    Sound::Unload(fileName_);
    isEnd_ = true;
}

void VoiceSystem::Play()
{
    Sound::Load(fileName_);
    Sound::Play(fileName_, volume_);
    isEnd_ = false;
}
