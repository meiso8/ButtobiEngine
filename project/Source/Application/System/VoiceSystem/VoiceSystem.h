#pragma once
#include<string>
class VoiceSystem
{

public:
    enum class VoiceTag {
        kFirstStory,
        kSecondStory
    };

   static void LoadAndStart(const VoiceTag tag);
   static void Update();
   static bool IsEnd();
   static void Stop();
private:
    static  std::string fileName_;
    static float volume_;
    static bool isEnd_;
};

