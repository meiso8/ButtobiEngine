#pragma once
#include"Sprite.h"
#include<memory>
#include<array>
class PauseScreen
{
public:
    static bool isActive_;
    static bool isPause_;
    static bool isRetry;
    static bool isBackToTitle;

    PauseScreen();
    void Initialize();
    void Update();
    void Draw();

private:
    enum Layer {
        kBlackScreen,
        kPauseBG,
        kPausing,
        kBackToGame,
        kReTry,
        kBackToTitle,
        kButton,
        kConfirm,
        kMaxLayer,
    };
    enum Button {
        kBackToGameButton,
        kReTryButton,
        kBackToTitleButton,
        kButtonMax,
    };

    void TimerUpdate();
    void TimerDown();
    void SelectButton();
    void ScalingButton();

	float selectTimer_ = 0.0f;
    float pauseTimer_ = 0.0f;
    int selectButtonNum_ = kBackToGameButton;

    float scaleTheta_ = 0.0f;

    std::array < Vector2, kMaxLayer> startPos_;
    std::array < Vector2, kMaxLayer> endPos_;
    std::array<std::unique_ptr<Sprite>, kMaxLayer> sprites_;

    Vector2 menuUiPos_;
    std::unique_ptr<Sprite> menuSprite_ = nullptr;

};

