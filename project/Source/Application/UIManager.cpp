#define NOMINMAX
#include "UIManager.h"
#include"Texture.h"
#include"Sprite.h"
#include"Sound.h"
#include"DebugUI.h"
#include "Input.h"
#include"Score.h"
#include "Lerp.h"

#define InversFPS 1.0f/60.0f;

inline bool NearlyEqual(const Vector3& a, const Vector3& b, float epsilon = 0.001f) { return fabs(a.x - b.x) < epsilon && fabs(a.y - b.y) < epsilon && fabs(a.z - b.z) < epsilon; }

UIManager::UIManager()
{
	sparklesPosirion[0] = {1200,70};
	sparklesPosirion[1] = {ComboCountPosition_.x+50,ComboCountPosition_.y+20};
	sparklesPosirion[2] = { JuiceCountPosition_.x - 20, JuiceCountPosition_.y + 30};

    LifeTextureHandle_ = Texture::GetHandle(Texture::LIFE);

    comboTextureHandle_ = Texture::GetHandle(Texture::COMBO);
	ComboNumberTextureHandle = Texture::GetHandle(Texture::COMBONUMBERS);
    speedBonusTextureHandle_ = Texture::GetHandle(Texture::SPEED_BONUS);
	SpeedNumberTextureHandle = Texture::GetHandle(Texture::SPEED_BONUSNUMBERS);
    WASDTextureHandle_ = Texture::GetHandle(Texture::WASD);
    SpaceTextureHandle_ = Texture::GetHandle(Texture::SPACE);
    TimerTextureHandle_ = Texture::GetHandle(Texture::TIMER);
    JuiceTextureHandle_ = Texture::GetHandle(Texture::JUICE);
	JuiceCountTextureHandle_ = Texture::GetHandle(Texture::JUICECOUNT);
	JuiceNumberTextureHandle_ = Texture::GetHandle(Texture::JUICENUMBER);
	JuiceStringTextureHandle_ = Texture::GetHandle(Texture::JUICESTRING);
    timerNumbersTexturHandle = Texture::GetHandle(Texture::TIMERNUMBERS);
	sparkleTextureHandle_ = Texture::GetHandle(Texture::SPARKLE);
   
    comboSprite = std::make_unique<Sprite>();
    comboSprite->Create(comboTextureHandle_, ComboPosition_, ComboSize_, { 1, 1, 1, 1 });
    speedBonusSprite = std::make_unique<Sprite>();
    speedBonusSprite->Create(speedBonusTextureHandle_, speedBonusPosition_, SpeedBonusSize_, { 1, 1, 1, 1 });
    
    
    SpaceSprite = std::make_unique<Sprite>();
    SpaceSprite->Create(SpaceTextureHandle_, SpacePosition_, SpaceSize_, { 1, 1, 1, 1 });
    TimerSprite = std::make_unique<Sprite>();
    TimerSprite->Create(TimerTextureHandle_, TimerPosition_, TimerSize_, { 1, 1, 1, 1 });
    JuiceSprite = std::make_unique<Sprite>();
    JuiceSprite->Create(JuiceTextureHandle_, JuicePosition_, JuiceSize_, { 1, 1, 1, 1 });
	JuiceCountSprite = std::make_unique<Sprite>();
	JuiceCountSprite->Create(JuiceCountTextureHandle_, JuiceCountPosition_, JuiceCountSize, {1, 1, 1, 1});
	JuiceStringSprite = std::make_unique<Sprite>();
	JuiceStringSprite->Create(JuiceStringTextureHandle_, JuiceStringPosition_, JuiceStringSize, {1, 1, 1, 1});
    

	for (int i = 0; i < 5; i++) {
		JuiceNumberSprite[i].Create(JuiceNumberTextureHandle_, JuiceNumberPosition_, JuiceNumberSize, {1, 1, 1, 1});
	}


    for (int i = 0; i < 4; i++) {
        timerNumbersSprites[i].Create(timerNumbersTexturHandle, timerNumbersFirstPos, TimerNumbersSize, { 1, 1, 1, 1 });
		
		WASDSprite[i].Create(WASDTextureHandle_, WASDPosition_[i], WASDSize_, {1, 1, 1, 1});
		ComboNumberSprites[i].Create(ComboNumberTextureHandle, ComboNumberPosition, ComboNumberSize_, {1, 1, 1, 1});
		SpeedNumberSprites[i].Create(SpeedNumberTextureHandle, speedBonusNumberPosition, SpeedBonusNumberSize_, {1, 1, 1, 1});
    }
    

    for (int i = 0; i < MaxLife_; i++) {
        lifeSprites[i].Create(LifeTextureHandle_, LifeFirstPosition_, LifeSize_, { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    for (int i = 0; i < 3; i++) {
		
		sparkleSprites[i].Create(sparkleTextureHandle_, sparklesPosirion[i], {0, 0}, {1, 1, 1, 1});
    }

    for (int i = 0; i < 2; i++) {
		ComboCountSprites[i].Create(ComboNumberTextureHandle, ComboCountPosition_, ComboCountSize_, {1, 1, 1, 1});
    }

    scoreClass_ = std::make_unique<Score>();

}

void UIManager::Initialize() {

    for (int i = 0; i < lifeSprites.size(); i++) {
        lifeSprites[i].SetPosition({ LifeFirstPosition_.x + LifePositionInterval_ * i, LifeFirstPosition_.y });
    }

    scoreClass_->Initialize();

    for (int i = 0; i < 2; i++) {
		ComboCountSprites[i].SetPosition({ComboCountPosition_.x + (i * ComboCountPositionInterval), ComboCountPosition_.y});
		ComboCountSprites[i].SetTextureSize({124.0f, 124.0f});
		ComboCountSprites[i].Update();
    }

	for (int i = 0; i < 4; i++) {
		ComboNumberSprites[i].SetPosition({ComboNumberPosition.x + ComboNumberPositionInteval * i, ComboNumberPosition.y});
		ComboNumberSprites[i].SetTextureSize({124.0f, 124.0f});
		ComboNumberSprites[i].Update();
		SpeedNumberSprites[i].SetPosition({speedBonusNumberPosition.x + speedBonusNumberInterval * i, speedBonusNumberPosition.y});
		SpeedNumberSprites[i].SetTextureSize({124.0f, 124.0f});
		SpeedNumberSprites[i].SetTextureLeftTop({0, 0});
		SpeedNumberSprites[i].Update();
		timerNumbersSprites[i].SetPosition({timerNumbersFirstPos.x + i * timerNumbersPosInterval, timerNumbersFirstPos.y});
		timerNumbersSprites[i].SetTextureLeftTop({0, 0});
		timerNumbersSprites[i].SetTextureSize({100.0f, 100.0f});
		timerNumbersSprites[i].Update();
		WASDSprite[i].SetTextureSize({256, 256});
		WASDSprite[i].Update();
		JuiceNumberSprite[i].SetPosition({JuiceNumberPosition_.x + JuiceNummerIntervalPosition_*i, JuiceNumberPosition_.y});
		
    }

	JuiceNumberSprite[4].SetPosition({1080.0f, 380.0f});

    WASDSprite[0].SetTextureLeftTop({256, 128});
	WASDSprite[1].SetTextureLeftTop({0, 384});
	WASDSprite[2].SetTextureLeftTop({256, 384});
	WASDSprite[3].SetTextureLeftTop({512, 384});

	for (int i = 0; i < 5; i++) {
		JuiceNumberSprite[i].SetTextureSize({124.0f, 124.0f});
		JuiceNumberSprite[i].SetTextureLeftTop({0, 0});
		JuiceNumberSprite[i].Update();
	
	}
	for (int i = 0; i < 3; i++) {
		sparkleSprites[i].SetSize({0, 0});
		sparkleSprites[i].SetAnchorPoint({0.5f, 0.5f});
		sparkleSprites[i].Update();
	}
	JuiceSprite->SetTextureSize({768.0f, 1024.0f});
	JuiceSprite->SetTextureLeftTop({0.0f, 0.0f});
	JuiceSprite->Update();
	SpaceSprite->SetTextureLeftTop({0, 0});
	SpaceSprite->SetTextureSize({768, 768});
	SpaceSprite->Update();

	GameTime_ = MaxGameTime_;
	Life_ = MaxLife_;

	Combo_ = 0;
	ComboBonus_ = 1.0f;
	speed_ = 0;
	speedBonus_ = 1.0f;
    JuiceCount = 0;
	JuiceBonus_ = 0.0f;
	for (int i = 0; i < MaxLife_; i++) {
		lifeSprites[i].SetColor({1, 1, 1, 1});
    }
	for (int i = 0; i < 3; i++) {
		sparkleTime[i] = 0.0f;
		spakleAlpha[i] = 1.0f;
	}
}

void UIManager::Update() {


    GameTime_ -= 1 / 60.0f;
    int totalSeconds = static_cast<int>(GameTime_);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    UpdateCombo();
	if (Combo_ == 0) {
		ComboBonus_ = 1.0f;
	} else {
		ComboBonus_ = static_cast<int>(Combo_ /5.0f) + 1.1f;
    }

    if (ComboBonus_ > 10) {
		ComboBonus_ = 9.9f;
    }
    
    speedBonus_ = speed_ / 50.0f+1.0f;
	if (speedBonus_ > 10) {
		speedBonus_ = 9.9f;
    }

    if (JuiceCount >= 10) {
		JuiceCount = 9;
    }



    if (JuiceCount > 1) {
    JuiceBonus_ = JuiceCount * 0.1f + 1.0f;
	} else {
		JuiceBonus_ = 1.0f;
    }
	for (int i = 0; i < 3; i++) {
		if (isSparkleAlive[i]) {
			if (spakleAlpha[i]<=0.0f) {
				spakleAlpha[i] = 0.0f;
				isSparkleAlive[i] = false;
			} else {
				
				if (sparkleTime[i]>=1.0f) {
					
					spakleAlpha[i] -= 0.1f;
				} else {
					sparkleTime[i] += 0.01f;
					sparkleSprites[i].SetSize(Lerp({0, 0}, SparkleMaxSize, sparkleTime[i]));
					spakleAlpha[i] = 1.0f;
				}
			}
		sparkleSprites[i].SetColor({1, 1, 1, spakleAlpha[i]});
		} else {
			sparkleSprites[i].SetColor({1, 1, 1, 1});
			sparkleSprites[i].SetSize({0, 0});
			spakleAlpha[i] = 1.0f;
			sparkleTime[i] = 0.0f;
		}
	}
		 
    // =============================== Score==========================================
    
    scoreClass_->Update(ComboBonus_,speedBonus_,JuiceBonus_);
	
	if (scoreClass_->GetIsHighScoreUpdate()) {
		isSparkleAlive[0] = true;
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


	// ================= ComboBonus 表示 =================
	{
		int integerPart = static_cast<int>(ComboBonus_);
		int decimalPart = static_cast<int>((ComboBonus_ * 10)) % 10;

		
		ComboNumberSprites[0].SetTextureLeftTop({124.0f * 11, 0});
		// 整数部分
		ComboNumberSprites[1].SetTextureLeftTop({124.0f * integerPart, 0});

		// 小数点（固定でインデックス10）
		ComboNumberSprites[2].SetTextureLeftTop({124.0f * 10, 0});

		// 小数部分
		ComboNumberSprites[3].SetTextureLeftTop({124.0f * decimalPart, 0});

		for (int i = 0; i < 4; i++) {
			ComboNumberSprites[i].Update();
		}
	}

	// ================= SpeedBonus 表示 =================
	{
		int integerPart = static_cast<int>(speedBonus_);
		int decimalPart = static_cast<int>((speedBonus_ * 10)) % 10;

		SpeedNumberSprites[0].SetTextureLeftTop({124.0f * 11, 0});

		// 整数部分
		SpeedNumberSprites[1].SetTextureLeftTop({124.0f * integerPart, 0});

		// 小数点（固定でインデックス10）
		SpeedNumberSprites[2].SetTextureLeftTop({124.0f * 10, 0});

		// 小数部分
		SpeedNumberSprites[3].SetTextureLeftTop({124.0f * decimalPart, 0});

		for (int i = 0; i < 4; i++) {
			SpeedNumberSprites[i].Update();
		}
	}

    {        
        int tensPlace = static_cast<int> (Combo_ / 10.0f);
		int onesPlace = Combo_ % 10;

		ComboCountSprites[0].SetTextureLeftTop({124.0f * tensPlace,0.0f});
		ComboCountSprites[1].SetTextureLeftTop({124.0f * onesPlace, 0.0f});
		for (int i = 0; i < 2; i++) {
		
        ComboCountSprites[i].Update();

        }
    }

    {
		int integerPart = static_cast<int>(JuiceBonus_);
		int decimalPart = static_cast<int>((JuiceBonus_ * 10)) % 10;

		 JuiceNumberSprite[0].SetTextureLeftTop({124.0f * 11, 0});
		// 整数部分
	    JuiceNumberSprite[1].SetTextureLeftTop({124.0f * integerPart, 0});

		// 小数点（固定でインデックス10）
		JuiceNumberSprite[2].SetTextureLeftTop({124.0f * 10, 0});

		// 小数部分
		JuiceNumberSprite[3].SetTextureLeftTop({124.0f * decimalPart, 0});

		
    }
    {
		if (JuiceMeter >5) {
			JuiceMeter = 0;
			JuiceCount += 1;
			isSparkleAlive[2] = true;
			sparkleSprites[2].SetColor({1, 1, 1, 1});
            Sound::PlaySE(Sound::CHREERS);
        }

		JuiceSprite->SetTextureLeftTop({768.0f * JuiceMeter, 0.0f});
		JuiceSprite->Update();

        JuiceNumberSprite[4].SetTextureLeftTop({124.0f * (JuiceCount), 0.0f});
		
        for (int i = 0; i < 5; i++) {
			JuiceNumberSprite[i].Update();
		}
    }
	

	if (Input::IsPushKey(DIK_W)) {
		WASDSprite[0].SetTextureLeftTop({256 + 768, 128});
	} else {
		WASDSprite[0].SetTextureLeftTop({256, 128});
	}
	if (Input::IsPushKey(DIK_A)) {
		WASDSprite[1].SetTextureLeftTop({0 + 768, 384});
	} else {
		WASDSprite[1].SetTextureLeftTop({0, 384});
	}
	if (Input::IsPushKey(DIK_S)) {
		WASDSprite[2].SetTextureLeftTop({256 + 768, 384});
	} else {
		WASDSprite[2].SetTextureLeftTop({256, 384});
	}
	if (Input::IsPushKey(DIK_D)) {
		WASDSprite[3].SetTextureLeftTop({512 + 768, 384});
	} else {
		WASDSprite[3].SetTextureLeftTop({512, 384});
	}
	for (int i = 0; i < 4; i++) {
		WASDSprite[i].Update();
	}
	if (Input::IsPushKey(DIK_SPACE)) {
		SpaceSprite->SetTextureLeftTop({768, 0});
	} else {
		SpaceSprite->SetTextureLeftTop({0, 0});
	}
	SpaceSprite->Update();
}

void UIManager::Draw() {

    Sprite::PreDraw(kBlendModeNormal);

    for (int i = 0; i < MaxLife_; i++) {
        lifeSprites[i].Draw();
    }

    JuiceSprite->Draw();
	
	for (int i = 0; i < 3; i++) {
		if (isSparkleAlive[i]) {
		sparkleSprites[i].Draw();
		}
    }
	JuiceCountSprite->Draw();
	for (int i = 0; i < 5; i++) {
		JuiceNumberSprite[i].Draw();
    }
	/*if (JuiceCount >= 2) {*/
		JuiceStringSprite->Draw();
    /*}*/
	
        scoreClass_->Draw();
    

    comboSprite->Draw();
    speedBonusSprite->Draw();
	for (int i = 0; i < 2; i++) {
		ComboCountSprites[i].Draw();
    }
	for (int i = 0; i < 4; i++) {
		ComboNumberSprites[i].Draw();
		SpeedNumberSprites[i].Draw();
    }
	for (int i = 0; i < 4; i++) {
		WASDSprite[i].Draw();
	}
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
    ImGui::SliderFloat("GameTimer", &GameTime_,0.0f,120.0f);
    ImGui::Text("Combo : %d MaxCombo : %d ", Combo_, MaxCombo_);
	ImGui::InputScalar("juiceMeter", ImGuiDataType_U32, &JuiceMeter);
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

    //コンボ数補正
    Combo_ = std::min(Combo_, MaxCombo_);

    ComboTimer_ += InversFPS;

    if (ComboTimer_ < 19.0f) {
        return;
    }

    ComboTimer_ = 0.0f;
    Combo_ = 0;
    isUpdateComboTimer_ = false;


}

void UIManager::SetLife(int life) {
	if (life == Life_&&Life_!= MaxLife_) {
		lifeSprites[Life_].SetColor({0, 0, 0, 1});
    }
	Life_ = life;
	if (Life_ < 0) {
		Life_ = 0;
	}
	if (Life_ > MaxLife_) {
		Life_ = MaxLife_;
	}
};
