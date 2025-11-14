#pragma once
#include<cstdint>
#include<memory>
#include"Sprite.h"
#include<unordered_map>
#include<functional>
class SceneChange;

typedef void (SceneChange::* PFunc)();

class SceneChange
{
public:

    enum State {
        kUnKnown,
        kFadeIn,
        kFadeOut,
        kWipeIn,
        kWipeOut,
        kSceneStart,
        kSceneEnd
    };
    SceneChange();
    ~SceneChange();
    void SetState(State state, uint32_t endTime);
    void Initialize();
    void Update();
    void Draw();
    bool IsStartScene() { return(state_ == kSceneStart) ? true : false; }
    bool IsEndScene() { return(state_ == kSceneEnd) ? true : false; }
    static uint32_t GetTimer() { return timer_; }
private:
    static uint32_t timer_;
    static uint32_t endTime_;

    std::unique_ptr<Sprite> sprite_ = nullptr;
    State state_ = SceneChange::State::kUnKnown;
    //メンバ関数ポインタテーブル
    static std::unordered_map<State, PFunc>StatesUpdate_;
    void FadeOut();
    void FadeIn();
    void WipeOut();
    void WipeIn();

    void TimerUpdate();
    void SwitchScene();

};


