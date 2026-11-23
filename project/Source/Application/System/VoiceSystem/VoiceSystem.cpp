#include "VoiceSystem.h"
#include"Sound.h"

std::string VoiceSystem::fileName_ = {""};
float VoiceSystem::volume_ = 1.0f;
bool VoiceSystem::isEnd_ = false;
void VoiceSystem::LoadAndStart(const VoiceTag tag)
{
   
    fileName_ = "Resource/Sounds/Voice/Story/" + std::to_string(static_cast<int>(tag))+".mp3";
    Sound::Load(fileName_);
    Sound::Play(fileName_, volume_);
    isEnd_ = false;
}

void VoiceSystem::Update()
{

    if (Sound::IsPlaying(fileName_)) {
        return;
    }

    //プレイ終了時、アンロードする
    Sound::Unload(fileName_);
    isEnd_ = true;
}

bool VoiceSystem::IsEnd()
{
    return isEnd_;
}

void VoiceSystem::Stop()
{
    Sound::Stop(fileName_);
}
