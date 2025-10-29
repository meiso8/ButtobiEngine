#pragma once
#include"Vector2.h"
class Sprite;
#include<cstdint>
#include<memory>
#include<array>

class Score
{

private:
	//--スコア
// 現在のスコア
	int Score_ = 0;
	// ハイスコア
	int HighScore_ = 0;

	bool isScoreUP_ = false;

	bool isHighScoreUpdate_ = false;

	// スコア加算値
	int AddBaseScore_ = 100;
	// スコアの最終加算値
	int AddFinalScore_ = 0;

	uint32_t juiceCount_;

	Vector2 ScoreSize_ = { 256, 64 };	
	Vector2 HighScoreSize_ = { 256.0f * 0.9f, 64 * 0.9f };
	Vector2 NumberSize_ = { 50, 50 };

	Vector2 ScorePosition_ = { 900, 70 };
	Vector2 ScoreNumbersPosition_{ 900, 120 };
	Vector2 HighScorePosition_ = { 900, 0 };
	Vector2 HighScoreNumbersPosition_{ 950, 40 };
	float scorePosInterval = 50;

	uint32_t scoreTextureHandle_ = 0;
	uint32_t NumbersTextureHandle_ = 0;
	uint32_t highScoreTextureHandle_ = 0;

	std::unique_ptr<Sprite> scoreSprite;
	std::array< std::unique_ptr<Sprite>, 7> NumbersSprite;
	std::unique_ptr<Sprite> highScoreSprite;
	std::array< std::unique_ptr<Sprite>, 7> HighScoreNumbersSprite;
public:
	Score();
	bool* GetIsScoreUPPtr() { return &isScoreUP_; }
	bool GetIsHighScoreUpdate() { return isHighScoreUpdate_; }
	void Initialize();
	void Update(const float& ComboBonus, const float& speedBonus, const float& JuiceBonus);
	void Calculation();
	void Draw();
	void SetScorePos();
	void ClearScorePos();
	void SetJuiceCount(uint32_t Count) { juiceCount_ = Count; };
	uint32_t GetJuiceCount() { return juiceCount_; };
	int GetScore() { return Score_; };
};

