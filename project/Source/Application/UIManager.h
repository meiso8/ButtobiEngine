#pragma once

#include"Sprite.h"
#include<array>

class UIManager {


private:

    //<数値系>

    //--ライフ
    // 最大ライフ
    int MaxLife_ = 3;
    // 現在のライフ
    int Life_ = MaxLife_;

    //--スコア
    // 現在のスコア
    int Score_ = 0;
    // ハイスコア
    int HighScore_ = 0;

    // スコア加算値
    int AddBaseScore_ = 100;
    // スコアの最終加算値
    int AddFinalScore_ = 0;

    //--コンボ
    // 現在のコンボ数
    int Combo_ = 0;
    // 最大コンボ数
    int MaxCombo_ = 15;
    //　コンボボーナス倍率
    float ComboBonus_ = 1.0f;
    // コンボタイマー
    float ComboTimer_ = 0.0f;

    //--スピードボーナス
    // スピードボーナス倍率
    float speedBonus_ = 1.0f;

    //--ゲーム時間
    // 現在のゲーム時間
    float GameTime_ = 0.0f;
    // 最大ゲーム時間
    float MaxGameTime_ = 60.0f * 60;



    //<サイズ系>

    Vector2 LifeSize_ = { 90, 90 };
    Vector2 ScoreSize_ = { 256, 64 };
    Vector2 ComboSize_ = { 256, 64 };
    Vector2 SpeedBonusSize_ = { 256, 64 };
    Vector2 WASDSize_ = { 128, 128 };
    Vector2 SpaceSize_ = { 128, 128 };
    Vector2 TimerSize_ = { 64, 64 };
    Vector2 JuiceSize_ = { 255, 340 };

    //<位置系>

    Vector2 LifeFirstPosition_ = { 0, 0 };
    float LifePositionInterval_ = 20.0f + LifeSize_.x;

    Vector2 ScorePosition_ = { 1000, 20 };

    Vector2 ComboPosition_ = { 1000, 80 };

    Vector2 speedBonusPosition_ = { 1000, 140 };

    Vector2 WASDPosition_ = { 20, 600 };

    Vector2 SpacePosition_ = { WASDPosition_.x + WASDSize_.x + 20, WASDPosition_.y };

    Vector2 TimerPosition_ = { 50, 20 };
    Vector2 JuicePosition_ = { 1000, 360 };

    //<テクスチャ系>
    uint32_t LifeTextureHandle_ = 0;
    uint32_t scoreTextureHandle_ = 0;
    uint32_t comboTextureHandle_ = 0;
    uint32_t speedBonusTextureHandle_ = 0;
    uint32_t WASDTextureHandle_ = 0;
    uint32_t SpaceTextureHandle_ = 0;
    uint32_t TimerTextureHandle_ = 0;
    uint32_t JuiceTextureHandle_ = 0;
	uint32_t NumbersTextureHandle_ = 0;

    //<スプライト系>
    std::array<Sprite,3> lifeSprites;
    std::unique_ptr<Sprite> scoreSprite;
    std::unique_ptr<Sprite> comboSprite;
    std::unique_ptr<Sprite> speedBonusSprite;
    std::unique_ptr<Sprite> WASDSprite;
    std::unique_ptr<Sprite> SpaceSprite;
    std::unique_ptr<Sprite> TimerSprite;
    std::unique_ptr<Sprite> JuiceSprite;
	std::unique_ptr<Sprite> numbersSprite;


public:
    UIManager();
    //初期化
    void Initialize();
    // 更新
    void Update();
    // 描画	
    void Draw();
    void Debug();

    //フルーツのスピードを取得する
    void SetFruitSpeed(const float& speedA, const float& speedB);

    void SetLife(int life) {
        Life_ = life;
        if (Life_ < 0) {
            Life_ = 0;
        }
        if (Life_ > MaxLife_) {
            Life_ = MaxLife_;
        }
    };

    int GetMaxLife() { return MaxLife_; };

};
