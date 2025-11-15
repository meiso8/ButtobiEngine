#pragma once  

#pragma region //MP3  

#include <vector> 
#include <mfapi.h>  
#include <mfidl.h>  
#include <mfreadwrite.h>  
#include <string>

struct SoundData {
    std::string filePath;
    WAVEFORMATEX pWaveFormat;
    std::vector<BYTE> mediaData;
};

#pragma endregion  

#include <xaudio2.h> // wavファイル用  
#include <stdint.h> // int32_t  

#include <wrl.h> // ComPtr(コムポインタ)  
#include<memory>//unique_ptr
#include <unordered_map>

class Sound {
public:

    enum TAG {
        BGM1,
        PICO,
        CRACKER,
        FOOT_STEP,

        SOUNDS
    };
    /// @brief 全てのサウンドを読み込む
    static void LoadAllSound();
    ///
    static void PlayBGM(const TAG& tag, const float& volumeOffset = 0.0f, const bool& loop = true);
    static void PlaySE(const TAG& tag, const float& volumeOffset = 0.0f, const bool& loop = false);
    static void PlayLoopSE(const TAG& tag, const float& volumeOffset = 0.0f);
    static void PlayOriginSE(const TAG& tag, const float& volumeOffset = 0.0f);

    static void Pause(const TAG& tag);  // 一時停止
    static void Resume(const TAG& tag); // 再開
    static void Stop(const TAG& tag);
    static bool IsPlaying(const TAG& tag);

    static void StopAllSound();

    /// @brief 音声データの解放関数  
/// @param soundData 音声データ  
    static void Unload(SoundData& soundData);
    static void Initialize();
    static void Finalize();
    static void SetVol(const float& vol, const TAG& tag);

    static std::vector<float> GetWaveform(const TAG& tag);
    static UINT64 GetSamplesPlayed(const TAG& tag);
public:

    static float bgmVolume_;
    static float seVolume_;

private:
    Sound() = default;
    ~Sound() = default;
    Sound(Sound&) = delete;
    Sound& operator=(Sound&) = delete;

    static void Play(const uint32_t& tag, const float& volume, const bool& isLoop = false);
    static void LoadFile(const std::string& path);
    static uint32_t Load(const std::string& path);
    static bool IsPlayingAll();

    static uint32_t GetSoundByIndex(const std::string& filePath);
    static XAUDIO2_BUFFER GetBuffer(const TAG& tag);
private:

    static  Microsoft::WRL::ComPtr<IXAudio2> xAudio2_; // ComオブジェクトなのでComPtrで管理する。  
    static IXAudio2MasteringVoice* masterVoice_; // ReleaseなしのためComPtrで管理することが出来ない。  
    static std::unordered_map<uint32_t, IXAudio2SourceVoice*> voices_;
    static std::vector<SoundData> soundDatas;
    static std::vector<uint32_t> handles_;
};