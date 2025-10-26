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


	//<サイズ系>

	Vector2 LifeSize_ = {90, 90};


	//Vector2 NumberSize_ = {50, 50};
	Vector2 ComboSize_ = {256*0.8f, 64*0.8f};
	Vector2 ComboNumberSize_ = {50, 50};
	Vector2 SpeedBonusSize_ = {256*0.85f, 64*0.85f};
	Vector2 SpeedBonusNumberSize_ = {50, 50};
	Vector2 WASDSize_ = {128, 128};
	Vector2 SpaceSize_ = {128, 128};
	Vector2 TimerSize_ = {160, 160};
	Vector2 JuiceSize_ = {255, 340};
	Vector2 JuiceNumberSize = {50 ,50};
	Vector2 JuiceStringSize = {96*2.5f, 31*2.5f};
	Vector2 JuiceCountSize = {140, 60};
	Vector2 TimerNumbersSize = {30, 40};

	//<位置系>

	Vector2 LifeFirstPosition_ = {10, 0};
	float LifePositionInterval_ = 20.0f + LifeSize_.x;



	Vector2 ComboPosition_ = {900, 180};
	Vector2 ComboNumberPosition = {1080, 180};
	float ComboNumberPositionInteval = 50.0f;

	Vector2 speedBonusPosition_ = {900, 240};
	Vector2 speedBonusNumberPosition = {1080, 240};
	float speedBonusNumberInterval = 50.0f;

	Vector2 WASDPosition_ = {20, 600};

	Vector2 SpacePosition_ = {WASDPosition_.x + WASDSize_.x + 20, WASDPosition_.y};

	Vector2 TimerPosition_ = {20, 80};
	Vector2 timerNumbersFirstPos = {40, 135};
	float timerNumbersPosInterval = 30.0f;
	Vector2 JuicePosition_ = {1000, 360};
	Vector2 JuiceNumberPosition_ = {1080, 300};
	float JuiceNummerIntervalPosition_ = 50.0f;
	Vector2 JuiceStringPosition_ = {900, 300};
	Vector2 JuiceCountPosition_ = {1120, 370};

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
	/*uint32_t NumbersTextureHandle_ = 0;*/
	uint32_t timerNumbersTexturHandle = 0;
	uint32_t ComboNumberTextureHandle = 0;
	uint32_t SpeedNumberTextureHandle = 0;

	//<スプライト系>
	std::array<Sprite, 3> lifeSprites;


	std::unique_ptr<Sprite> comboSprite;
	std::unique_ptr<Sprite> speedBonusSprite;
	std::unique_ptr<Sprite> WASDSprite;
	std::unique_ptr<Sprite> SpaceSprite;
	std::unique_ptr<Sprite> TimerSprite;
	std::unique_ptr<Sprite> JuiceSprite;
	std::array<Sprite, 4> JuiceNumberSprite;
	std::unique_ptr<Sprite> JuiceStringSprite;
	std::unique_ptr<Sprite> JuiceCountSprite;

	std::array<Sprite, 4> timerNumbersSprites;


	std::array<Sprite, 3> ComboNumberSprites;
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
	uint32_t* GetJuiceMeter() { return &JuiceMeter; };
	uint32_t* GetSpeedPointer() { return &speed_; };
	int GetMaxLife() { return MaxLife_; };
	uint32_t* GetComboPointer() { return &Combo_; }
	float* GetComboTimerPtr() { return &ComboTimer_; }
	Score* GetScorePtr() { return scoreClass_.get(); };
	float GetTimer() { return GameTime_; };
	void SetIsUpdateComboTimer(const bool& isUpdateComboTimer) { isUpdateComboTimer_ = isUpdateComboTimer; };
};
