#pragma once
#include"AnimationObject3d.h"
#include<memory>
class FirstStoryOne
{
public:
    enum PHASE {
        kZero,
        kFirst,
        kSecond,
        kThird,
        kEnd,
    };
    FirstStoryOne();
    ~FirstStoryOne();
    //アニメーション付きオブジェクト
    void Initialize();
    void Update();
    void Draw();
    void PlayInterCome(const int32_t index);
    const PHASE GetPhase() { return phase_; }
    const float GetLocalTimer();
    const bool GetIsStorZeroEnd() { return phase_!= kZero; }
    void Unload();
    const bool GetIsStoryOneEnd() { return phase_ == kEnd; };
private:
    std::unique_ptr<AnimationObject3d> aniObject_ = nullptr;
    std::array<bool, 3> isInterComeRing_;
    float timer_ = 0.0f;
    bool isHorror_ = false;
    PHASE phase_ = kZero;
    bool isVoiceStart_ = false;
};

