#pragma once
#include"Sprite.h"
#include<memory>
#include<array>

class PauseScreen
{
public:
    static bool isActive_;
    static bool isPause_;
    static bool isBackToTitle;

    PauseScreen();
    void Initialize();
    void Update();
    void Draw();

private:
    enum Layer {
        kBlackScreen,
        kBackToGame,
        kBackToTitle,
        kMaxLayer,
    };

    void TimerUpdate();
    void SelectButton();
    void ScalingButton();

    float pauseTimer_ = 0.0f;
    int selectButtonNum_ = kBackToGame;

    float scaleTheta_ = 0.0f;

    std::array < Vector2, kMaxLayer> pos_;
    std::array<std::unique_ptr<Sprite>, kMaxLayer> sprites_;
};

