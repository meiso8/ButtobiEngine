#pragma once
#include<string>
class VoiceSystem
{

public:
    enum class VoiceTag {
        kNoLetter,
        kMissing,
        kMuseum,
        KEnter,
    };

   static void LoadAndStart(const VoiceTag tag);
   static void Update();
   static bool IsEnd();
   static void Stop();
   static void Unload();
private:
    static  std::string fileName_;
    static float volume_;
    static bool isEnd_;
};

