#include"SoundManager.h"
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

ComPtr<IXAudio2> SoundManager::xAudio2_ = nullptr; // ComオブジェクトなのでComPtrで管理する。  
std::vector<IXAudio2SourceVoice*> SoundManager::voices_ = {};
IXAudio2MasteringVoice* SoundManager::masterVoice_ = nullptr;
bool SoundManager::isStarted_ = false;
bool SoundManager::isPaused_ = false;

std::vector<SoundData> SoundManager::soundDatas;

void SoundManager::LoadFile(const std::string& path) {

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

uint32_t SoundManager::Load(const std::string& path)
{
    LoadFile(path);
    return GetSoundByIndex(path);
}

uint32_t SoundManager::GetSoundByIndex(const std::string& filePath)
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

void SoundManager::Initialize()
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

void SoundManager::Finalize()
{
    xAudio2_.Reset();
    for (uint32_t i = 0; i < soundDatas.size(); ++i) {
        Unload(soundDatas[i]);
    }

    HRESULT result;
    result = MFShutdown();
    assert(SUCCEEDED(result));
}


void SoundManager::Unload(SoundData& soundData) {

    // メディアデータの解放
    soundData.mediaData.clear();
    soundData.pWaveFormat = {};
};

void SoundManager::Play(const uint32_t& tag, const float& volume, bool isLoop) {
    HRESULT result;

    IXAudio2SourceVoice* newVoice = nullptr;

    result = xAudio2_->CreateSourceVoice(&newVoice, &soundDatas[tag].pWaveFormat);
    assert(SUCCEEDED(result));
    newVoice->SetVolume(volume);

    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundDatas[tag].mediaData.data();
    buf.AudioBytes = /*sizeof(BYTE) * */static_cast<UINT32>(soundDatas[tag].mediaData.size());
    buf.Flags = XAUDIO2_END_OF_STREAM;

    if (isLoop) {
        buf.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    result = newVoice->SubmitSourceBuffer(&buf);
    assert(SUCCEEDED(result));
    result = newVoice->Start();//再生開始
    assert(SUCCEEDED(result));
    isStarted_ = true;
    isPaused_ = false;

    voices_.push_back(newVoice);

};

void SoundManager::Stop() {

    for (auto voice : voices_) {
        voice->Stop();
        voice->Discontinuity();
        voice->FlushSourceBuffers();
        voice->DestroyVoice();
    }
    voices_.clear();

    isStarted_ = false;
    isPaused_ = true;

}

void SoundManager::Pause() {
    //if (pSourceVoice_) {
    //    pSourceVoice_->Stop(); // バッファは保持されたまま停止
    //    isPaused_ = true;
    //}
}

void SoundManager::Resume() {
    //if (pSourceVoice_) {
    //    pSourceVoice_->Start(); // 停止した位置から再開
    //}
}

bool SoundManager::IsActuallyPlaying() {
    return isStarted_ && !isPaused_;
}

bool SoundManager::IsPlaying() {
    for (auto voice : voices_) {
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
