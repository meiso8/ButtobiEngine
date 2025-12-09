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

    //TAGを宣言する
    enum TAG {
  
        inGameBGM01,
        inGameBGM02,
        playerHP1BGM,
        gameOverBGM,
        resultBGM,
        titleBGM,

        kBossDamage01,
        kBossDamage02,
        kBossFireBall,
        kBossHeal,
        kBossTackle,
        kDamage,
        kDecision,
        kDefeatBoss,
        kExplosion,
        kFloorUpdate,
        kFloorRespawn,
        kMeltFloor,



        kMoveCursor,
        kPauseButton,
        kPlayerDamage,
        kPlayerHeal,
        kPlayerWalk,
        kStripObj,
        kStun,
        kTrhowObj,
        kWindAttackCharge,
        kWindAttackShot,

        zushaa,
        
        SOUNDS
    };

    /// @brief 全てのサウンドを読み込む
    static void LoadAllSound();

    /// @brief BGＭを再生する
    /// @param tag タグ
    /// @param volumeOffset offset
    /// @param loop ループフラグ　デフォルト true
    static void PlayBGM(const TAG& tag, const float& volumeOffset = 0.0f, const bool& loop = true);
    /// @brief SEを再生する
    /// @param tag タグ
    /// @param volumeOffset  offset
    /// @param loop ループフラグ デフォルト false
    static void PlaySE(const TAG& tag, const float& volumeOffset = 0.0f, const bool& loop = false);
    /// @brief LoopSEを再生する
    /// @param tag タグ
    /// @param volumeOffset  offset
    static void PlayLoopSE(const TAG& tag, const float& volumeOffset = 0.0f);
    /// @brief 重複を省いたSEを再生する
    /// @param tag タグ
    /// @param volumeOffset offset
    static void PlayOriginSE(const TAG& tag, const float& volumeOffset = 0.0f);
    /// @brief 一時停止
    /// @param tag 
    static void Pause(const TAG& tag);
    /// @brief 再開
    /// @param tag タグ
    static void Resume(const TAG& tag);
    /// @brief 停止
    /// @param tag タグ 
    static void Stop(const TAG& tag);
    /// @brief 全ての音を停止する
    static void StopAllSound();
    /// @brief プレイ中かどうかを検証する
    /// @param tag タグ 
    /// @return 判定結果
    static bool IsPlaying(const TAG& tag);


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