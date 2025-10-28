#pragma once

#include "Sprite.h"
#include <array>

class Score;

class UIManager {


private:
	//<数値系>

	//--ライフ
	// 最大ライフ
	int MaxLife_ = 3;
	// 現在のライフ
	int Life_ = MaxLife_;

	//スコアクラス
	std::unique_ptr<Score> scoreClass_ = nullptr;


	//--コンボ
	// 現在のコンボ数
	uint32_t Combo_ = 0;
	// 最大コンボ数
	uint32_t MaxCombo_ = 15;
	// 　コンボボーナス倍率
	float ComboBonus_ = 1.0f;
	// コンボタイマー
	float ComboTimer_ = 0.0f;
	// コンボタイマーのカウント
	bool isUpdateComboTimer_ = false;

	//--スピードボーナス
	// スピードボーナス倍率
	float speedBonus_ = 1.0f;
	uint32_t speed_ = 0;

	//--ジュースボーナス
	float JuiceBonus_ = 1.0f;
	uint32_t JuiceMeter = 0;
	uint32_t JuiceCount = 0;

	//--ゲーム時間
	// 現在のゲーム時間
	float GameTime_ = 0.0f;
	// 最大ゲーム時間
	float MaxGameTime_ = 90.0f;

	float spakleAlpha[3]{};
	bool isSparkleAlive[3] = {false,false,false};

	//<サイズ系>

	Vector2 LifeSize_ = {90, 90};


	//Vector2 NumberSize_ = {50, 50};
	Vector2 ComboSize_ = {256*0.5f, 76*0.5f};
	Vector2 ComboCountSize_ = {40.0f, 40.0f};
	Vector2 ComboNumberSize_ = {25, 25};
	Vector2 SpeedBonusSize_ = {256*0.7f, 90*0.7f};
	Vector2 SpeedBonusNumberSize_ = {40, 40};
	Vector2 WASDSize_ = {32, 32};
	Vector2 SpaceSize_ = {128, 128};
	Vector2 TimerSize_ = {160, 160};
	Vector2 JuiceSize_ = {255, 340};
	Vector2 JuiceNumberSize = {40 ,40};
	Vector2 JuiceStringSize = {96*2.5f, 31*2.5f};
	Vector2 JuiceCountSize = {140, 60};
	Vector2 TimerNumbersSize = {30, 40};
	
	Vector2 SparkleMaxSize = {200, 200};

	//<位置系>

	Vector2 LifeFirstPosition_ = {10, 0};
	float LifePositionInterval_ = 20.0f + LifeSize_.x;

	Vector2 ComboPosition_ = {980, 190};
	Vector2 ComboCountPosition_ = {1120, 185};
	float ComboCountPositionInterval = 40.0f;
	Vector2 ComboNumberPosition = {1180, 170};
	float ComboNumberPositionInteval = 20.0f;

	Vector2 speedBonusPosition_ = {940, 230};
	Vector2 speedBonusNumberPosition = {1120, 240};
	float speedBonusNumberInterval = 30.0f;

	Vector2 WASDPosition_[4] = {
		{20+WASDSize_.x, 640},
		{20,640+WASDSize_.y},
		{(WASDSize_.x)+20,640+WASDSize_.y},
        {(WASDSize_.x*2)+20, 640 + WASDSize_.y}
    };

	Vector2 SpacePosition_ = {WASDPosition_[3].x + WASDSize_.x + 20, WASDPosition_[0].y-WASDSize_.y};

	Vector2 TimerPosition_ = {20, 80};
	Vector2 timerNumbersFirstPos = {40, 135};
	float timerNumbersPosInterval = 30.0f;
	Vector2 JuicePosition_ = {1000, 360};
	Vector2 JuiceNumberPosition_ = {1120, 320};
	float JuiceNummerIntervalPosition_ = 30.0f;
	Vector2 JuiceStringPosition_ = {920, 300};
	Vector2 JuiceCountPosition_ = {1120, 370};
	std::array<Vector2, 3> sparklesPosirion;

	//<テクスチャ系>
	uint32_t LifeTextureHandle_ = 0;


	uint32_t comboTextureHandle_ = 0;
	uint32_t speedBonusTextureHandle_ = 0;
	uint32_t WASDTextureHandle_ = 0;
	uint32_t SpaceTextureHandle_ = 0;
	uint32_t TimerTextureHandle_ = 0;
	uint32_t JuiceTextureHandle_ = 0;
	uint32_t JuiceNumberTextureHandle_ = 0;
	uint32_t JuiceStringTextureHandle_ = 0;
	uint32_t JuiceCountTextureHandle_ = 0;
	uint32_t sparkleTextureHandle_ = 0;
	/*uint32_t NumbersTextureHandle_ = 0;*/
	uint32_t timerNumbersTexturHandle = 0;
	uint32_t ComboNumberTextureHandle = 0;
	uint32_t SpeedNumberTextureHandle = 0;

	//<スプライト系>
	std::array<Sprite, 3> lifeSprites;


	std::unique_ptr<Sprite> comboSprite;
	std::unique_ptr<Sprite> speedBonusSprite;
	std::array<Sprite,4> WASDSprite;
	std::unique_ptr<Sprite> SpaceSprite;
	std::unique_ptr<Sprite> TimerSprite;
	std::unique_ptr<Sprite> JuiceSprite;
	std::array<Sprite, 4> JuiceNumberSprite;
	std::unique_ptr<Sprite> JuiceStringSprite;
	std::unique_ptr<Sprite> JuiceCountSprite;

	std::array<Sprite, 4> timerNumbersSprites;
	std::array<Sprite, 3> sparkleSprites;

	std::array<Sprite, 3> ComboNumberSprites;
	std::array<Sprite, 2> ComboCountSprites;
	std::array<Sprite, 3> SpeedNumberSprites;

public:
	UIManager();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	void Debug();

	void UpdateCombo();

	void SetLife(int life);
	bool* GetSparkleIsAlive(int index) { return &isSparkleAlive[index]; };
	uint32_t* GetJuiceMeter() { return &JuiceMeter; };
	uint32_t* GetSpeedPointer() { return &speed_; };
	int GetMaxLife() { return MaxLife_; };
	uint32_t* GetComboPointer() { return &Combo_; }
	float* GetComboTimerPtr() { return &ComboTimer_; }
	Score* GetScorePtr() { return scoreClass_.get(); };
	float GetTimer() { return GameTime_; };
	void SetIsUpdateComboTimer(const bool& isUpdateComboTimer) { isUpdateComboTimer_ = isUpdateComboTimer; };
};
