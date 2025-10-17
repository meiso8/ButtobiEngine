#include "UIManager.h"
#include"Texture.h"
#include"Sprite.h"

#include"DebugUI.h"

UIManager::UIManager()
{
    LifeTextureHandle_ = Texture::GetHandle(Texture::LIFE);
    scoreTextureHandle_ = Texture::GetHandle(Texture::SCORE);
    comboTextureHandle_ = Texture::GetHandle(Texture::COMBO);
    speedBonusTextureHandle_ = Texture::GetHandle(Texture::SPEED_BONUS);
    WASDTextureHandle_ = Texture::GetHandle(Texture::WASD);
    SpaceTextureHandle_ = Texture::GetHandle(Texture::SPACE);
    TimerTextureHandle_ = Texture::GetHandle(Texture::TIMER);
    JuiceTextureHandle_ = Texture::GetHandle(Texture::JUICE);
    NumbersTextureHandle_ = Texture::GetHandle(Texture::NUMBERS);

    scoreSprite = std::make_unique<Sprite>();
    scoreSprite->Create(scoreTextureHandle_, ScorePosition_, ScoreSize_, { 1, 1, 1, 1 });
    comboSprite = std::make_unique<Sprite>();
    comboSprite->Create(comboTextureHandle_, ComboPosition_, ComboSize_, { 1, 1, 1, 1 });
    speedBonusSprite = std::make_unique<Sprite>();
    speedBonusSprite->Create(speedBonusTextureHandle_, speedBonusPosition_, SpeedBonusSize_, { 1, 1, 1, 1 });
    WASDSprite = std::make_unique<Sprite>();
    WASDSprite->Create(WASDTextureHandle_, WASDPosition_, WASDSize_, { 1, 1, 1, 1 });
    SpaceSprite = std::make_unique<Sprite>();
    SpaceSprite->Create(SpaceTextureHandle_, SpacePosition_, SpaceSize_, { 1, 1, 1, 1 });
    TimerSprite = std::make_unique<Sprite>();
    TimerSprite->Create(TimerTextureHandle_, TimerPosition_, TimerSize_, { 1, 1, 1, 1 });
    JuiceSprite = std::make_unique<Sprite>();
    JuiceSprite->Create(JuiceTextureHandle_, JuicePosition_, JuiceSize_, { 1, 1, 1, 1 });
    numbersSprite = std::make_unique<Sprite>();
    numbersSprite->Create(NumbersTextureHandle_, { 0, 0 }, { 256, 32 }, { 1, 1, 1, 1 });

    for (int i = 0; i < MaxLife_; i++) {
        lifeSprites[i].Create(LifeTextureHandle_, LifeFirstPosition_, LifeSize_, { 1.0f, 1.0f, 1.0f, 1.0f });
    }

}

void UIManager::Initialize() {

    for (int i = 0; i < lifeSprites.size(); i++) {
        lifeSprites[i].SetPosition({ LifeFirstPosition_.x + LifePositionInterval_ * i, LifeFirstPosition_.y });
    }

}

void UIManager::Update() {


    AddFinalScore_ += static_cast<int>(AddBaseScore_ * speedBonus_);
    AddFinalScore_ += static_cast<int>(AddBaseScore_ * ComboBonus_);

    Score_ += AddFinalScore_; 
 
    AddFinalScore_ = 0;

    if (Score_> HighScore_) {
        HighScore_ = Score_;
	}
}

void UIManager::Draw() {

    Sprite::PreDraw(kBlendModeNormal);

    for (int i = 0; i < Life_; i++) {
        lifeSprites[i].Draw();
    }
    
    JuiceSprite->Draw();
    scoreSprite->Draw();
    comboSprite->Draw();
    speedBonusSprite->Draw();
    WASDSprite->Draw();
    SpaceSprite->Draw();
    TimerSprite->Draw();

}

void UIManager::Debug()
{
   
    DebugUI::CheckSprite(*JuiceSprite, "JuiceSprite");
    DebugUI::CheckSprite(*scoreSprite, "scoreSprite");
    DebugUI::CheckSprite(*comboSprite, "comboSprite");
    DebugUI::CheckSprite(*speedBonusSprite, "speedBonusSprite");
    DebugUI::CheckSprite(*WASDSprite, "WASDSprite");
    DebugUI::CheckSprite(*SpaceSprite, "SpaceSprite");
    DebugUI::CheckSprite(*TimerSprite, "TimerSprite");
 
    for (int i = 0; i < Life_; i++) {
        DebugUI::CheckSprite(lifeSprites[i], "lifeSprites");
    }


}

void UIManager::SetFruitSpeed(const float& speedA, const float& speedB)
{

}
