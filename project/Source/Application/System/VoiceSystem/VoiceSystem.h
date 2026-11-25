#pragma once
#include<string>
#include <type_traits>

class VoiceSystem
{

public:
    enum class VoiceTag {
        kNoLetter,
        kMissing,
        kMuseum,
        kEnter,
    };

    enum class HintTag {
        kPuzzle,
    };
   template <typename T>
   static void LoadAndStart(const T tag);
   static void Update();
   static bool IsEnd();
   static void Stop();
   static void Unload();
private:
    static void Play();
    static  std::string fileName_;
    static float volume_;
    static bool isEnd_;
};

template<typename T>
inline void VoiceSystem::LoadAndStart(const T tag)
{
    std::string dirPath;

    if constexpr (std::is_same_v<T, HintTag>) {
        dirPath = "Resource/Sounds/Voice/Hint/";
    } else if constexpr (std::is_same_v<T, VoiceTag>) {
        dirPath = "Resource/Sounds/Voice/Story/";
    } else {
        static_assert(false, "Unsupported tag type for LoadAndStart");
    }

    fileName_ = dirPath + std::to_string(static_cast<int>(tag)) + ".mp3";
    Play();
}
