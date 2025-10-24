#include"Sound.h"
#include"DirectXCommon.h"
#include <fstream>
#include<assert.h>
#include"StringUtility.h"

#pragma comment(lib, "Mf.lib")  
#pragma comment(lib, "mfplat.lib")  
#pragma comment(lib, "MFreadwrite.lib")  
#pragma comment(lib, "mfuuid.lib")  

#pragma comment(lib, "xaudio2.lib") // xaudio2.libをリンクする。  

using namespace Microsoft::WRL;

ComPtr<IXAudio2> Sound::xAudio2_ = nullptr; // ComオブジェクトなのでComPtrで管理する。  
std::unordered_map<uint32_t, IXAudio2SourceVoice*>Sound::voices_;
IXAudio2MasteringVoice* Sound::masterVoice_ = nullptr;

std::vector<SoundData> Sound::soundDatas;
std::vector<uint32_t> Sound::handles_;

float Sound::bgmVolume_ = 0.0625f;
float Sound::seVolume_ = 0.5f;

void Sound::LoadAllSound()
{
#ifdef _DEBUG

    bgmVolume_ = 0.0f;
    seVolume_ = 0.1f;
#endif // _DEBUG


    handles_.resize(SOUNDS);

    //サウンドの読み込み
    handles_[BGM1] = Load("Resources/Sounds/fruit_mix.mp3");
    handles_[PICO] = Load("Resources/Sounds/pico.mp3");
    handles_[CRACKER] = Load("Resources/Sounds/cracker_short.mp3");
    handles_[CHARGE] = Load("Resources/Sounds/charge.mp3");
    handles_[FRUIT_FALL] = Load("Resources/Sounds/poi.mp3");

    handles_[ATTACK] = Load("Resources/Sounds/externals/attack.mp3");
    handles_[CHREERS] = Load("Resources/Sounds/externals/cheers!.mp3");
    handles_[PLAYER_HIT] = Load("Resources/Sounds/externals/playerHit.mp3");
    handles_[POUR_DRINK] = Load("Resources/Sounds/externals/pour_drink.mp3");
    handles_[YEAH] = Load("Resources/Sounds/yeah.mp3");
    handles_[GAME_OVER] = Load("Resources/Sounds/gameOver.mp3");
    handles_[BOUND] = Load("Resources/Sounds/externals/biyon.mp3");
    handles_[FULL_CHARGE] = Load("Resources/Sounds/externals/up.mp3");
    handles_[ALARM] = Load("Resources/Sounds/externals/alarm.mp3");
}

void Sound::PlayBGM(const uint32_t index, const float& volumeOffset, const bool& loop)
{
    if (!IsPlayingAll()) {
        Play(handles_[index], bgmVolume_ + volumeOffset, loop);
    }
}

void Sound::PlaySE(const uint32_t index, const float& volumeOffset, const bool& loop)
{
    Play(handles_[index], seVolume_ + volumeOffset);
}

void Sound::PlayLoopSE(const uint32_t index, const float& volumeOffset)
{
    if (!IsPlaying(handles_[index])) {
        Play(handles_[index], seVolume_ + volumeOffset, true);
    }
}

void Sound::PlayOriginSE(const uint32_t handleIndex, const float& volumeOffset)
{
    if (!IsPlaying(handles_[handleIndex])) {
        Play(handles_[handleIndex], seVolume_ + volumeOffset, false);
    }
}

void Sound::Pause(const uint32_t handleIndex)
{
    auto it = voices_.find(handles_[handleIndex]);
    if (it != voices_.end() && it->second != nullptr) {
        it->second->Stop(); // バッファは保持されたまま停止
    }
}

void Sound::Resume(const uint32_t handleIndex)
{
    auto it = voices_.find(handles_[handleIndex]);
    if (it != voices_.end() && it->second != nullptr) {
        it->second->Start(); // 停止した位置から再開
    }
}

void Sound::Stop(const uint32_t index)
{
    auto it = voices_.find(handles_[index]);
    if (it != voices_.end() && it->second != nullptr) {
        it->second->Stop(); // バッファは保持されたまま停止
        it->second->Discontinuity();
        it->second->FlushSourceBuffers();
        /*  it->second->DestroyVoice();*/
    }

}

void Sound::StopAllSound()
{
    for (auto& pair : voices_) {
        IXAudio2SourceVoice* voice = pair.second;
        if (voice) {
            voice->Stop();
            voice->Discontinuity();
            voice->FlushSourceBuffers();
            // voice->DestroyVoice();
        }
    }

}


uint32_t Sound::GetSoundByIndex(const std::string& filePath)
{
    //読み込み済みデータを検索
    auto it = std::find_if(
        soundDatas.begin(),
        soundDatas.end(),
        [&](SoundData& soundData) {return soundData.filePath == filePath; }
    );

    if (it != soundDatas.end()) {
        uint32_t soundIndex = static_cast<uint32_t>(std::distance(soundDatas.begin(), it));
        return soundIndex;
    }

    assert(0);
    return 0;
}

void Sound::Unload(SoundData& soundData) {

    // メディアデータの解放
    soundData.mediaData.clear();
    soundData.pWaveFormat = {};
};

// ====================================================================================================

uint32_t Sound::Load(const std::string& path)
{
    LoadFile(path);
    //サウンドハンドルを返す
    return (UINT)soundDatas.size() - 1;
}

void Sound::Play(const uint32_t& handle, const float& volume, const bool& isLoop) {
    HRESULT result;

    IXAudio2SourceVoice* newVoice = nullptr;

    result = xAudio2_->CreateSourceVoice(&newVoice, &soundDatas[handle].pWaveFormat);
    assert(SUCCEEDED(result));
    newVoice->SetVolume(volume);

    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundDatas[handle].mediaData.data();
    buf.AudioBytes = /*sizeof(BYTE) * */static_cast<UINT32>(soundDatas[handle].mediaData.size());
    buf.Flags = XAUDIO2_END_OF_STREAM;

    if (isLoop) {
        buf.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    result = newVoice->SubmitSourceBuffer(&buf);
    assert(SUCCEEDED(result));
    result = newVoice->Start();//再生開始
    assert(SUCCEEDED(result));

    voices_[handle] = newVoice;
};


bool Sound::IsPlayingAll() {

    for (const auto& pair : voices_) {
        IXAudio2SourceVoice* voice = pair.second;
        if (voice) {
            XAUDIO2_VOICE_STATE state{};
            voice->GetState(&state);
            if (state.BuffersQueued > 0) {
                return true; // 少なくとも1つの音声が再生中
            }
        }
    }
    return false; // すべての音声が停止している
}


bool Sound::IsPlaying(const uint32_t& index) {

    auto it = voices_.find(handles_[index]);
    if (it != voices_.end() && it->second != nullptr) {
        XAUDIO2_VOICE_STATE state{};
        it->second->GetState(&state);
        if (state.BuffersQueued > 0) {
            return true; // 少なくとも1つの音声が再生中
        }
    }

    return false; // 音声が停止している
}

void Sound::LoadFile(const std::string& path) {

    //読み込み済みテクスチャを検索
    auto it = std::find_if(
        soundDatas.begin(),
        soundDatas.end(),
        [&](SoundData& soundData) {return soundData.filePath == path; }
    );

    //テクスチャ枚数上限チェック
    assert(soundDatas.size() < DirectXCommon::kMaxSoundCount);

    if (it != soundDatas.end()) {
        return;
    }

    std::wstring filePathW = StringUtility::ConvertString(path);

    //ソースリーダーの作成
    ComPtr<IMFSourceReader> pMFSourceReader = { nullptr };
    HRESULT result = MFCreateSourceReaderFromURL(filePathW.c_str(), NULL, &pMFSourceReader);
    assert(SUCCEEDED(result));

    //メディアタイプの取得
    ComPtr<IMFMediaType> pMFMediaType{ nullptr };
    MFCreateMediaType(&pMFMediaType);
    pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    pMFSourceReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType.Get());
    assert(SUCCEEDED(result));

    ComPtr<IMFMediaType> pOutType;
    pMFSourceReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pOutType);

    //オーディオデータ形式の作成　メディアタイプからWaveForMatexを生成
    WAVEFORMATEX* waveFormat{ nullptr };
    MFCreateWaveFormatExFromMFMediaType(pOutType.Get(), &waveFormat, nullptr);


    //テクスチャデータを追加
    soundDatas.resize(soundDatas.size() + 1);
    //追加したテクスチャデータの参照を取得する
    SoundData& soundData = soundDatas.back();
    soundData.pWaveFormat = *waveFormat;
    soundData.filePath = path;

    while (true) {
        ComPtr<IMFSample> pMFSample{ nullptr };
        DWORD streamIndex = 0, flags = 0;
        LONGLONG llTimeStamp = 0;
        //サンプルを読み込む
        result = pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &streamIndex, &flags, &llTimeStamp, &pMFSample);

        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)break;

        if (pMFSample) {
            ComPtr<IMFMediaBuffer> pMFMediaBuffer{ nullptr };
            pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

            BYTE* pBuffer{ nullptr };
            DWORD maxLength = 0, cbCurrentLength = 0;
            pMFMediaBuffer->Lock(&pBuffer, &maxLength, &cbCurrentLength);
            soundData.mediaData.insert(soundData.mediaData.end(), pBuffer, pBuffer + cbCurrentLength);
            pMFMediaBuffer->Unlock();
        }

    }


}



void Sound::Initialize()
{
    HRESULT result;
    result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    assert(SUCCEEDED(result));

    //マスターボイスの生成
    result = xAudio2_->CreateMasteringVoice(&masterVoice_);//masterVoiceはxAudio2の解放と同時に無効になるため自分でdeleteしない
    assert(SUCCEEDED(result));

    result = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
    assert(SUCCEEDED(result));

    soundDatas.reserve(DirectXCommon::kMaxSoundCount);
}

void Sound::Finalize()
{
    xAudio2_.Reset();
    for (uint32_t i = 0; i < soundDatas.size(); ++i) {
        Unload(soundDatas[i]);
    }

    HRESULT result;
    result = MFShutdown();
    assert(SUCCEEDED(result));
}