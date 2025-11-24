#pragma once
#include"Sprite.h"
#include<memory>
#include<array>
class PauseScreen
{
public:
    static bool isPause_;
    float pauseTimer_ = 0.0f;
    PauseScreen();
    void Update();
    void Draw();
    enum Layer {
        kBlackScreen,
        kPauseBG,
        kPausing,
        kBackToGame,
        kReset,
        kBackToTitle,
        kConfirm,
        kButton,
        kMaxLayer,
    };
private:
    Vector2 startPos_ = { 0.0f,0.0f };
    Vector2 endPos_ = { 0.0f,0.0f };
    std::array<std::unique_ptr<Sprite>, kMaxLayer> sprites_;
};

