#include "FirstStoryOne.h"
#include "ModelManager.h"
#include"Model.h"
#include"Sound.h"
#include"TimeManager.h"
#include"VoiceSystem/VoiceSystem.h"

namespace {
    const float kEndTime_ = 13.0f;
    const float kMiddleTime = 11.0f;
    const float kMiddle0Time = 6.0f;
}

FirstStoryOne::FirstStoryOne()
{
    //モデルを取得する
    Model* model = ModelManager::GetModel("player.gltf");
    aniObject_ = std::make_unique<AnimationObject3d>();
    aniObject_->Create();
    aniObject_->SetMeshAndMaterial(model);
    aniObject_->SetModelAndLoadAnimation(model);
    aniObject_->SetAnimation("InterCome");
}

FirstStoryOne::~FirstStoryOne()
{
    aniObject_->UnRegisterObject();
}

void FirstStoryOne::Initialize()
{

    //便りがない
    VoiceSystem::LoadAndStart(VoiceSystem::VoiceTag::kNoLetter);

    timer_ = 0.0f;
    isHorror_ = false;
    isVoiceStart_ = false;

    for (auto& ringFlag : isInterComeRing_) {
        ringFlag = false;
    }

    //体の位置初期化
    aniObject_->Initialize();
    aniObject_->SetObjectName("FirstStoryPlayer");
    aniObject_->RegisterObject();
    aniObject_->SetTranslate({ -0.477f,0.0f,0.751f });
    aniObject_->SetAnimation("InterCome");
    aniObject_->UpdateAniTimer();

    aniObject_->Update();

    phase_ = kZero;
}

void FirstStoryOne::Update()
{
    
    VoiceSystem::Update();

    if (phase_ == kZero) {
        //Zeroが終了していないとき
        if (VoiceSystem::IsEnd()) {
            //フェーズを変更する
            phase_ = kFirst;
        }
    }

    if (!GetIsStorZeroEnd()) {
        return;
    }

    timer_ += TimeManager::DeltaTime();

    if (timer_ <= kMiddle0Time) {

        phase_ = kFirst;

        if (timer_ >= 4.5f) {
            PlayInterCome(2);
        } else if (timer_ >= 3.5f) {
            PlayInterCome(1);
          
        } else if (timer_ >= 1.0f) {
            PlayInterCome(0);
        } else if (timer_ >= 0.5f) {
            if (!isVoiceStart_) {
                // 次の音声を鳴らす
                VoiceSystem::LoadAndStart(VoiceSystem::VoiceTag::kMissing);
                isVoiceStart_ = true;
            }
        }

    } else if (timer_ <= kMiddleTime) {

        phase_ = kSecond;

        if (!isHorror_) {
            //ホラー音を流す
            Sound::PlaySE(SoundFactory::HORROR2);
            isHorror_ = true;
        }

    } else if (timer_ <= kEndTime_) {

        phase_ = kThird;

    } else {
        phase_ = kEnd;
    }

    aniObject_->Update();
    //アニメーションタイマーのアップデート
    aniObject_->UpdateAniTimer();
}

void FirstStoryOne::Draw()
{
    if (phase_ == kFirst) {
        //中間時間いないの時
        aniObject_->Draw();
    }
}

void FirstStoryOne::PlayInterCome(const int32_t index)
{
    if (static_cast<size_t>(index) < isInterComeRing_.size()) {
        if (!isInterComeRing_[index]) {
            //いちどどめてから
            Sound::PlaySE(SoundFactory::INTERCOM);
            isInterComeRing_[index] = true;
        }
    }
}

const float FirstStoryOne::GetLocalTimer()
{
    float time = 0.0f;

    if (phase_ == kFirst) {
        time = TimeManager::GetLocalTimer(timer_, 0.0f, kMiddle0Time);
    } else  if (phase_ == kSecond) {
        time = TimeManager::GetLocalTimer(timer_, kMiddle0Time, kMiddleTime);
    } else if (phase_ == kThird) {
        time = TimeManager::GetLocalTimer(timer_, kMiddleTime, kEndTime_);
    }
    return time;
}

void FirstStoryOne::Unload()
{
    VoiceSystem::Unload();
    phase_ = kEnd;
    timer_ = kEndTime_;
}
