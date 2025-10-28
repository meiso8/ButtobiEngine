#include "Score.h"
#include"Texture.h"
#include"Sprite.h"

Score::Score()
{
    scoreTextureHandle_ = Texture::GetHandle(Texture::SCORE);

    highScoreTextureHandle_ = Texture::GetHandle(Texture::HIGHSCORE);

    NumbersTextureHandle_ = Texture::GetHandle(Texture::NUMBERS);
    scoreSprite = std::make_unique<Sprite>();

    scoreSprite->Create(scoreTextureHandle_, ScorePosition_, ScoreSize_, { 1, 1, 1, 1 });
    highScoreSprite = std::make_unique<Sprite>();
    highScoreSprite->Create(highScoreTextureHandle_, HighScorePosition_, HighScoreSize_, { 1, 1, 1, 1 });

    for (int i = 0; i < 7; i++) {
        NumbersSprite[i] = std::make_unique<Sprite>();
        NumbersSprite[i]->Create(NumbersTextureHandle_, ScoreNumbersPosition_, NumberSize_, { 1, 1, 1, 1 });
        HighScoreNumbersSprite[i] = std::make_unique<Sprite>();
        HighScoreNumbersSprite[i]->Create(NumbersTextureHandle_, HighScoreNumbersPosition_, NumberSize_, { 1, 1, 1, 1 });
    }

}

void Score::Initialize()
{

    ScorePosition_ = { 900, 70 };
    ScoreNumbersPosition_ = { 900, 120 };
    HighScorePosition_ = { 900, 0 };
    HighScoreNumbersPosition_ = { 950, 40 };

    scoreSprite->SetPosition(ScorePosition_);
    highScoreSprite->SetPosition(HighScorePosition_);
    scorePosInterval = 50;

    for (int i = 0; i < 7; i++) {

		NumbersSprite[i]->SetPosition({ScoreNumbersPosition_.x + i * scorePosInterval, ScoreNumbersPosition_.y});
		NumbersSprite[i]->SetTextureLeftTop({0, 0});
		NumbersSprite[i]->SetTextureSize({80.0f, 80.0f});
		NumbersSprite[i]->Update();

		HighScoreNumbersSprite[i]->SetPosition({HighScoreNumbersPosition_.x + i * scorePosInterval * 0.8f, HighScoreNumbersPosition_.y});
		HighScoreNumbersSprite[i]->SetTextureLeftTop({0, 0});
		HighScoreNumbersSprite[i]->SetSize({40.0f, 40.0f});
		HighScoreNumbersSprite[i]->SetTextureSize({80.0f, 80.0f});
		HighScoreNumbersSprite[i]->Update();
	}
	isHighScoreUpdate_ = false;
	isScoreUP_ = false;
    Score_ = 0;
}

void Score::Update(const float& ComboBonus, const float& speedBonus, const float& JuiceBonus)
{
    if (isScoreUP_) {
        Score_ += static_cast<int>(AddBaseScore_ * ComboBonus * speedBonus * JuiceBonus);
        isScoreUP_ = false;
    }


    if (Score_ > HighScore_) {
        isHighScoreUpdate_ = true;
        HighScore_ = Score_;
    }

    if (isScoreUP_ || isHighScoreUpdate_) {
        Calculation();
        isHighScoreUpdate_ = false;
    }

}

void Score::Calculation()
{
    // スコア表示の更新
    NumbersSprite[0]->SetTextureLeftTop({ 80.0f * (Score_ / 1000000 % 10), 0 });
    NumbersSprite[1]->SetTextureLeftTop({ 80.0f * (Score_ / 100000 % 10), 0 });
    NumbersSprite[2]->SetTextureLeftTop({ 80.0f * (Score_ / 10000 % 10), 0 });
    NumbersSprite[3]->SetTextureLeftTop({ 80.0f * (Score_ / 1000 % 10), 0 });
    NumbersSprite[4]->SetTextureLeftTop({ 80.0f * (Score_ / 100 % 10), 0 });
    NumbersSprite[5]->SetTextureLeftTop({ 80.0f * (Score_ / 10 % 10), 0 });
    NumbersSprite[6]->SetTextureLeftTop({ 80.0f * (Score_ % 10), 0 });

    HighScoreNumbersSprite[0]->SetTextureLeftTop({ 80.0f * (HighScore_ / 1000000 % 10), 0 });
    HighScoreNumbersSprite[1]->SetTextureLeftTop({ 80.0f * (HighScore_ / 100000 % 10), 0 });
    HighScoreNumbersSprite[2]->SetTextureLeftTop({ 80.0f * (HighScore_ / 10000 % 10), 0 });
    HighScoreNumbersSprite[3]->SetTextureLeftTop({ 80.0f * (HighScore_ / 1000 % 10), 0 });
    HighScoreNumbersSprite[4]->SetTextureLeftTop({ 80.0f * (HighScore_ / 100 % 10), 0 });
    HighScoreNumbersSprite[5]->SetTextureLeftTop({ 80.0f * (HighScore_ / 10 % 10), 0 });
    HighScoreNumbersSprite[6]->SetTextureLeftTop({ 80.0f * (HighScore_ % 10), 0 });

    for (int i = 0; i < 7; i++) {
        NumbersSprite[i]->Update();
        HighScoreNumbersSprite[i]->Update();
    }

}

void Score::Draw()
{
    for (int i = 0; i < 7; i++) {
        NumbersSprite[i]->Draw();
        HighScoreNumbersSprite[i]->Draw();
    }
    scoreSprite->Draw();
    highScoreSprite->Draw();
}

void Score::SetScorePos()
{
    ScorePosition_ = { 320, 400 };
    ScoreNumbersPosition_ = { 640, 420 };
    HighScorePosition_ = { 320, 360 };
    HighScoreNumbersPosition_ = { 640, 360 };

    ScoreSize_ = { 256, 64 };
    HighScoreSize_ = { 256, 64 };

    scoreSprite->SetSize(ScoreSize_);
    highScoreSprite->SetSize(HighScoreSize_);
    scoreSprite->SetPosition(ScorePosition_);
    highScoreSprite->SetPosition(HighScorePosition_);

    scorePosInterval = 40;

    for (int i = 0; i < 7; i++) {

        NumbersSprite[i]->SetPosition({ ScoreNumbersPosition_.x + i * scorePosInterval, ScoreNumbersPosition_.y });
        NumbersSprite[i]->SetSize(NumberSize_);
        NumbersSprite[i]->Update();

        HighScoreNumbersSprite[i]->SetPosition({ HighScoreNumbersPosition_.x + i * scorePosInterval, HighScoreNumbersPosition_.y });
        HighScoreNumbersSprite[i]->SetSize(NumberSize_);
        HighScoreNumbersSprite[i]->Update();
    }

}
