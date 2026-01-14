#include"Sound.h"
#include"DirectXCommon.h"
#include <fstream>
#include<assert.h>
#include"StringUtility.h"
#include<algorithm>

#pragma comment(lib, "Mf.lib")  
#pragma comment(lib, "mfplat.lib")  
#pragma comment(lib, "MFreadwrite.lib")  
#pragma comment(lib, "mfuuid.lib")  

#pragma comment(lib, "xaudio2.lib") // xaudio2.libをリンクする。  

using namespace Microsoft::WRL;

ComPtr<IXAudio2> Sound::xAudio2_ = nullptr; // ComオブジェクトなのでComPtrで管理する。  
std::unordered_map<SoundFactory::TAG, IXAudio2SourceVoice*>Sound::voices_;
IXAudio2MasteringVoice* Sound::masterVoice_ = nullptr;

std::unordered_map<SoundFactory::TAG,SoundData> Sound::soundDatas_;

float Sound::bgmVolume_ = 0.25f;
float Sound::seVolume_ = 0.75f;

void Sound::PlayBGM(const SoundFactory::TAG& tag, const float& volumeOffset, const bool& loop)
{
    SetVol(bgmVolume_ + volumeOffset, tag);

    if (!IsPlaying(tag)) {
        Play(tag, bgmVolume_ + volumeOffset, loop);
    }
}

void Sound::PlaySE(const SoundFactory::TAG& tag, const float& volumeOffset, const bool& loop)
{
    Play(tag, seVolume_ + volumeOffset);
}

void Sound::PlayLoopSE(const SoundFactory::TAG& tag, const float& volumeOffset)
{
    if (!IsPlaying(tag)) {
        Play(tag, seVolume_ + volumeOffset, true);
    }
}

void Sound::PlayOriginSE(const SoundFactory::TAG& tag, const float& volumeOffset)
{
    if (!IsPlaying(tag)) {
        Play(tag, seVolume_ + volumeOffset, false);
    }
}

void Sound::Pause(const SoundFactory::TAG& tag)
{
    auto it = voices_.find(tag);
    if (it != voices_.end() && it->second != nullptr) {
        it->second->Stop(); // バッファは保持されたまま停止
    }
}

void Sound::Resume(const SoundFactory::TAG& tag)
{
    auto it = voices_.find(tag);
    if (it != voices_.end() && it->second != nullptr) {
        it->second->Start(); // 停止した位置から再開
    }
}

void Sound::Stop(const SoundFactory::TAG& tag)
{
    auto it = voices_.find(tag);
    if (it != voices_.end() && it->second != nullptr) {
        it->second->Stop(); // バッファは保持されたまま停止
        it->second->Discontinuity();
        it->second->FlushSourceBuffers();
    }

}

bool Sound::IsPlaying(const SoundFactory::TAG& tag) {

    auto it = voices_.find(tag);
    if (it != voices_.end() && it->second != nullptr) {
        XAUDIO2_VOICE_STATE state{};
        it->second->GetState(&state);
        if (state.BuffersQueued > 0) {
            return true; // 少なくとも1つの音声が再生中
        }
    }

    return false; // 音声が停止している
}


void Sound::StopAllSound()
{
    for (auto& pair : voices_) {
        IXAudio2SourceVoice* voice = pair.second;
        if (voice) {
            voice->Stop();
            voice->Discontinuity();
            voice->FlushSourceBuffers();
        }
    }

}


XAUDIO2_BUFFER Sound::GetBuffer(const SoundFactory::TAG& tag)
{
    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundDatas_[tag].mediaData.data();
    buf.AudioBytes = static_cast<UINT32>(soundDatas_[tag].mediaData.size());
    buf.Flags = XAUDIO2_END_OF_STREAM;

    return buf;
}

std::vector<float> Sound::GetWaveform(const SoundFactory::TAG& tag)
{
    XAUDIO2_BUFFER buf = Sound::GetBuffer(tag);
    std::vector<float> waveform;
    const int16_t* pcm = reinterpret_cast<const int16_t*>(buf.pAudioData);
    size_t sampleCount = buf.AudioBytes / sizeof(int16_t);

    // オーディオ入力コールバック側（例）
    waveform.resize(sampleCount);
    for (size_t i = 0; i < sampleCount; ++i) {
        waveform[i] = pcm[i] / 32768.0f; // -32768〜32767 → -1.0〜1.0
    }
    return waveform;
}

UINT64 Sound::GetSamplesPlayed(const SoundFactory::TAG& tag)
{
    XAUDIO2_VOICE_STATE state{};
    auto it = voices_.find(tag);
    if (it != voices_.end() && it->second != nullptr) {
        it->second->GetState(&state);
    }

    return state.SamplesPlayed;
}

void Sound::Unload(SoundData& soundData) {

    // メディアデータの解放
    soundData.mediaData.clear();
    soundData.pWaveFormat = {};
};

// ====================================================================================================

void Sound::Play(const SoundFactory::TAG& tag, const float& volume, const bool& isLoop) {
    HRESULT result;

    IXAudio2SourceVoice* newVoice = nullptr;

    result = xAudio2_->CreateSourceVoice(&newVoice, &soundDatas_[tag].pWaveFormat);
    assert(SUCCEEDED(result));
    float newVolume = volume;
    //最大値と最小値を入れる
    newVolume = std::clamp(newVolume, 0.0f, 10.0f);

    newVoice->SetVolume(newVolume);

    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundDatas_[tag].mediaData.data();
    buf.AudioBytes = static_cast<UINT32>(soundDatas_[tag].mediaData.size());
    buf.Flags = XAUDIO2_END_OF_STREAM;

    if (isLoop) {
        buf.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    result = newVoice->SubmitSourceBuffer(&buf);
    assert(SUCCEEDED(result));
    result = newVoice->Start();//再生開始
    assert(SUCCEEDED(result));

    voices_[tag] = newVoice;
};


void Sound::SetVol(const float& vol, const SoundFactory::TAG& tag)
{
    auto it = voices_.find(tag);
    if (it != voices_.end() && it->second != nullptr) {
        float newVolume = vol;
        //最大値と最小値を入れる
        newVolume = std::clamp(newVolume, 0.0f, 10.0f);
        it->second->SetVolume(newVolume);
    }
}

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


void Sound::Load(const std::string& path, const SoundFactory::TAG& tag) {

    //読み込み済みテクスチャを検索
    auto it = std::find_if(
        soundDatas_.begin(),
        soundDatas_.end(),
        [&](const auto& pair) {
            return pair.second.filePath == path;
        }
    );

    if (soundDatas_.contains(tag)) {
        return;
    }

    //テクスチャ枚数上限チェック
    assert(soundDatas_.size() < DirectXCommon::kMaxSoundCount);

    if (it != soundDatas_.end()) {
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

    soundDatas_[tag].pWaveFormat = *waveFormat;
    soundDatas_[tag].filePath = path;

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
            soundDatas_[tag].mediaData.insert(soundDatas_[tag].mediaData.end(), pBuffer, pBuffer + cbCurrentLength);
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

    soundDatas_.reserve(DirectXCommon::kMaxSoundCount);
}

void Sound::Finalize()
{

    for (auto& pair : voices_) {
        IXAudio2SourceVoice* voice = pair.second;
        if (voice) {
            voice->Stop();
            voice->Discontinuity();
            voice->FlushSourceBuffers();
            voice->DestroyVoice();
        }
    }

    for (auto& [tag, data] : soundDatas_) {
        Unload(data);
    }

    soundDatas_.clear();
    xAudio2_.Reset();

    HRESULT result;
    result = MFShutdown();
    assert(SUCCEEDED(result));
}
