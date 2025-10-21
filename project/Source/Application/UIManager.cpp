#include "UIManager.h"
#include"Texture.h"
#include"Sprite.h"

#include"DebugUI.h"

#define InversFPS 1.0f/60.0f;

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
    timerNumbersTexturHandle = Texture::GetHandle(Texture::TIMERNUMBERS);
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
    
    for (int i = 0; i < 7; i++) {
        NumbersSprite[i].Create(NumbersTextureHandle_, ScoreNumbersPosition_, NumberSize_, { 1, 1, 1, 1 });
        HighScoreNumbersSprite[i].Create(NumbersTextureHandle_, HighScoreNumbersPosition_, NumberSize_, { 1, 1, 1, 1 });
    }
    for (int i = 0; i < 4; i++) {
        timerNumbersSprites[i].Create(timerNumbersTexturHandle, timerNumbersFirstPos, TimerNumbersSize, { 1, 1, 1, 1 });
    }
    

    for (int i = 0; i < MaxLife_; i++) {
        lifeSprites[i].Create(LifeTextureHandle_, LifeFirstPosition_, LifeSize_, { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    for (int i = 0; i < 4; i++) {

        timerNumbersSprites[i].SetPosition({ timerNumbersFirstPos.x + i * timerNumbersPosInterval, timerNumbersFirstPos.y });
        timerNumbersSprites[i].SetTextureLeftTop({ 0, 0 });
        timerNumbersSprites[i].SetTextureSize({ 100.0f, 100.0f });
        timerNumbersSprites[i].Update();
    }
    for (int i = 0; i < 7; i++) {

        NumbersSprite[i].SetPosition({ ScoreNumbersPosition_.x + i * scorePosInterval, ScoreNumbersPosition_.y });
        NumbersSprite[i].SetTextureLeftTop({ 0, 0 });
        NumbersSprite[i].SetTextureSize({ 80.0f, 80.0f });
        NumbersSprite[i].Update();

        HighScoreNumbersSprite[i].SetPosition({ ScoreNumbersPosition_.x + i * scorePosInterval * 0.8f, ScoreNumbersPosition_.y - 40.0f });
        HighScoreNumbersSprite[i].SetTextureLeftTop({ 0, 0 });
        HighScoreNumbersSprite[i].SetSize({ 40.0f, 40.0f });
        HighScoreNumbersSprite[i].SetTextureSize({ 80.0f, 80.0f });
        HighScoreNumbersSprite[i].Update();
    }

    scoreSprite->SetAnchorPoint({ 0.5f, 0.5f });
    scoreSprite->SetRotate(-0.1f);
    GameTime_ = MaxGameTime_;

}

void UIManager::Initialize() {

    for (int i = 0; i < lifeSprites.size(); i++) {
        lifeSprites[i].SetPosition({ LifeFirstPosition_.x + LifePositionInterval_ * i, LifeFirstPosition_.y });
    }

}

void UIManager::Update() {


    GameTime_ -= 1 / 60.0f;
    int totalSeconds = static_cast<int>(GameTime_);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    UpdateCombo();
    // =============================== Score==========================================
    AddFinalScore_ += static_cast<int>(AddBaseScore_ * speedBonus_);
    AddFinalScore_ += static_cast<int>(AddBaseScore_ * ComboBonus_);

    Score_ += AddFinalScore_;

    AddFinalScore_ = 0;

    if (Score_ > HighScore_) {
        HighScore_ = Score_;
    }
    if (GameTime_ <= 0.0f) {
        GameTime_ = 0.0f;

    }
    // 桁ごとに分割
    int minTens = minutes / 10;
    int minOnes = minutes % 10;
    int secTens = seconds / 10;
    int secOnes = seconds % 10;

    // 数字1文字の幅（1桁ぶんのテクスチャサイズ）
    const float digitWidth = 100.0f;

    // 各桁を更新
    timerNumbersSprites[0].SetTextureLeftTop({ minTens * digitWidth, 0 });
    timerNumbersSprites[1].SetTextureLeftTop({ minOnes * digitWidth, 0 });
    timerNumbersSprites[2].SetTextureLeftTop({ secTens * digitWidth, 0 });
    timerNumbersSprites[3].SetTextureLeftTop({ secOnes * digitWidth, 0 });


    for (int i = 0; i < 4; i++) {
        timerNumbersSprites[i].Update();
    }

    // スコア表示の更新
    NumbersSprite[0].SetTextureLeftTop({ 80.0f * (Score_ / 1000000 % 10), 0 });
    NumbersSprite[1].SetTextureLeftTop({ 80.0f * (Score_ / 100000 % 10), 0 });
    NumbersSprite[2].SetTextureLeftTop({ 80.0f * (Score_ / 10000 % 10), 0 });
    NumbersSprite[3].SetTextureLeftTop({ 80.0f * (Score_ / 1000 % 10), 0 });
    NumbersSprite[4].SetTextureLeftTop({ 80.0f * (Score_ / 100 % 10), 0 });
    NumbersSprite[5].SetTextureLeftTop({ 80.0f * (Score_ / 10 % 10), 0 });
    NumbersSprite[6].SetTextureLeftTop({ 80.0f * (Score_ % 10), 0 });

    HighScoreNumbersSprite[0].SetTextureLeftTop({ 80.0f * (HighScore_ / 1000000 % 10), 0 });
    HighScoreNumbersSprite[1].SetTextureLeftTop({ 80.0f * (HighScore_ / 100000 % 10), 0 });
    HighScoreNumbersSprite[2].SetTextureLeftTop({ 80.0f * (HighScore_ / 10000 % 10), 0 });
    HighScoreNumbersSprite[3].SetTextureLeftTop({ 80.0f * (HighScore_ / 1000 % 10), 0 });
    HighScoreNumbersSprite[4].SetTextureLeftTop({ 80.0f * (HighScore_ / 100 % 10), 0 });
    HighScoreNumbersSprite[5].SetTextureLeftTop({ 80.0f * (HighScore_ / 10 % 10), 0 });
    HighScoreNumbersSprite[6].SetTextureLeftTop({ 80.0f * (HighScore_ % 10), 0 });

    for (int i = 0; i < 7; i++) {
        NumbersSprite[i].Update();
        HighScoreNumbersSprite[i].Update();

    }
}

void UIManager::Draw() {

    Sprite::PreDraw(kBlendModeNormal);

    for (int i = 0; i < Life_; i++) {
        lifeSprites[i].Draw();
    }

    JuiceSprite->Draw();
    for (int i = 0; i < 7; i++) {
        NumbersSprite[i].Draw();
        HighScoreNumbersSprite[i].Draw();
    }
    scoreSprite->Draw();
    comboSprite->Draw();
    speedBonusSprite->Draw();
    WASDSprite->Draw();
    SpaceSprite->Draw();
    TimerSprite->Draw();
    for (int i = 0; i < 4; i++) {
        timerNumbersSprites[i].Draw();
    }


}

void UIManager::Debug()
{


    //DebugUI::CheckSprite(*JuiceSprite, "JuiceSprite");
    //DebugUI::CheckSprite(*scoreSprite, "scoreSprite");
    //DebugUI::CheckSprite(*comboSprite, "comboSprite");
    //DebugUI::CheckSprite(*speedBonusSprite, "speedBonusSprite");
    //DebugUI::CheckSprite(*WASDSprite, "WASDSprite");
    //DebugUI::CheckSprite(*SpaceSprite, "SpaceSprite");
    //DebugUI::CheckSprite(*TimerSprite, "TimerSprite");

    //for (int i = 0; i < Life_; i++) {
    //    DebugUI::CheckSprite(lifeSprites[i], "lifeSprites");
    //}

    ImGui::Begin("UI");
    ImGui::SliderFloat("ComboTimer", &ComboTimer_, 0.0f, 100.0f);
    ImGui::Checkbox("UpdateTimer", &isUpdateComboTimer_);
    ImGui::Text("Combo : %d MaxCombo : %d ", Combo_, MaxCombo_);

    ImGui::End();


}

void UIManager::UpdateCombo()
{

    if (!isUpdateComboTimer_) {
        return;
    }

    if (Combo_ < 1) {
        return;
    }

    ComboTimer_ += InversFPS;

    if (ComboTimer_ < 19.0f) {
        return;
    }

    ComboTimer_ = 0.0f;
    Combo_ = 0;
    isUpdateComboTimer_ = false;


}

