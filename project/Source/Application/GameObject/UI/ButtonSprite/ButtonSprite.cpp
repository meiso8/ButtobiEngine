#include "ButtonSprite.h"
#include"TransformAni/TransformAni.h"
#include"TimeManager.h"
#include"DebugUI.h"
#include"InputBind.h"
#include"Easing.h"
#include"Sound.h"
//ステージ情報が欲しいよね
#include"../StageManager/StageManager.h"

namespace {
    const float kTimer_ = 10.0f;
    const float kScaleSpeed_ = 0.5f;
}
ButtonSprite::ButtonSprite()
{

    const float height = static_cast<float>(Window::GetClientHeight());
    const float whidth = static_cast<float>(Window::GetClientWidth());
    
    for (auto& sprite : sprites_) {
        sprite = std::make_unique<Sprite>();
        sprite->Create(TextureFactory::UV_CHECKER, { 0.0f,0.0f });
    }

    sprites_[kButton_UI_A]->SetTexture(TextureFactory::UI_A);
    sprites_[kButton_UI_L]->SetTexture(TextureFactory::UI_L);
    sprites_[kButton_UI_LB]->SetTexture(TextureFactory::UI_LB);
    sprites_[kButton_UI_R]->SetTexture(TextureFactory::UI_R);
    sprites_[kButton_UI_RB]->SetTexture(TextureFactory::UI_RB);
    sprites_[kButton_UI_X]->SetTexture(TextureFactory::UI_X);

    sprites_[kButton_UI_A]->SetPosition({ 1080.0f,640.0f });
    sprites_[kButton_UI_L]->SetPosition({ 108.0f,600.0f });
    sprites_[kButton_UI_LB]->SetPosition({ 64.0f,32.0f });
    sprites_[kButton_UI_R]->SetPosition({ 1080.0f,540.0f });
    sprites_[kButton_UI_RB]->SetPosition({ 1080.0f,48.0f });
    sprites_[kButton_UI_X]->SetPosition({ 1048.0f+60.0f+16.0f,592.0f+18.0f });

    sprites_[kButton_UI_RB]->SetAnchorPoint({ 0.5f,0.5f });
    sprites_[kButton_UI_X]->SetAnchorPoint({ 0.5f,0.5f });

    thermoGraphySprite_ = std::make_unique<Sprite>();
    thermoGraphySprite_->Create(TextureFactory::UI_THERMOGRAPHY, { whidth*0.5f,height*0.5f});
    thermoGraphySprite_->SetAnchorPoint({ 0.5f,0.5f });

    for (auto& sprite : sprites_) {
        sprite->AdjustTextureSize();
    }


}

ButtonSprite::~ButtonSprite()
{
   
}

void ButtonSprite::Initialize()
{
    isGetThermography_ = false;

    isGetThermographyFirst_ = false;
    
    isInformationStage_ = false;
    isPreInformationStage_ = false;
    jumpButtonAniTimer_ = 0.0f;

    timer_ = 0.0f;
    size_ = { 1.0f,1.0f };

}

void ButtonSprite::Update()
{
    if (isGetThermography_) {

        if (InputBind::IsClickR()) {
            if (!isGetThermographyFirst_) {
                isGetThermographyFirst_ = true;
            }
        }
        if (InputBind::IsClickPressR()) {
  
            scaleTimerDuration_ = 0.0f;
            timer_ = 0.0f;
        
            sprites_[kButton_UI_RB]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
            size_ = Lerp(size_, { 1.0f,1.0f }, kScaleSpeed_);
            sprites_[kButton_UI_RB]->SetScale(size_);
            thermoGraphySprite_->SetScale(size_);

        } else {
    
            sprites_[kButton_UI_RB]->SetColor({ 0.0f,1.0f,1.0f,1.0f });
            scaleTimerDuration_ += TimeManager::DeltaTime();
            scaleTimerDuration_ = std::fmod(scaleTimerDuration_, kTimer_);

            if (scaleTimerDuration_ <= kTimer_ * 0.5f) {
                timer_ += TimeManager::DeltaTime();
                timer_ = std::fmod(timer_, 1.0f);
                TransformAni::PoyoPoyo(size_, timer_, 1.0f, 0.0625f);
                sprites_[kButton_UI_RB]->SetScale(size_);
                thermoGraphySprite_->SetScale(size_);
            }
        }
    }

    if (!isGetThermographyFirst_) {
        //サーモ初めましての時のみ描画する
        thermoGraphySprite_->Update();
    }

    isInformationStage_ = StageManager::GetInstance()->GetCurrentStageName() == "InformationStage";
    //インフォメーションステージから抜けた瞬間に
    if (isPreInformationStage_ && !isInformationStage_) {
        Sound::PlaySE(SoundFactory::BELL);
        jumpButtonAniTimer_ = 0.0f;
    } 

    //インフォメーションステージから抜けたときにジャンプボタンのアニメーションを行う
    if (!isInformationStage_) {
        if (jumpButtonAniTimer_ <= 1.0f) {
            jumpButtonAniTimer_ += TimeManager::DeltaTime();
            sprites_[kButton_UI_X]->SetScale(Lerp(sprites_[kButton_UI_X]->GetScale(), { 1.5f ,1.5f }, 0.05f));

            sprites_[kButton_UI_X]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
        } else {
            const float tempNum = 1.0f / 256.0f;
            sprites_[kButton_UI_X]->SetScale(Lerp(sprites_[kButton_UI_X]->GetScale(), { 1.0f ,1.0f }, 0.05f));
            sprites_[kButton_UI_X]->SetColor({ 86.0f * tempNum,65.0f * tempNum,62.0f * tempNum ,1.0f });
        }
    }



    isPreInformationStage_ = isInformationStage_;


    for (int i = 0; i < kButtonMaxCount; ++i) {

        if (isInformationStage_ && i == kButton_UI_X) {
            continue;
        }
        sprites_[i]->Update();
#ifdef USE_IMGUI
        std::string name = "ButtonSprite" + std::to_string(i);
        DebugUI::CheckSprite(*sprites_[i], name.c_str());
#endif
    }

}

void ButtonSprite::Draw()
{
    for (int i = 0; i < kButtonMaxCount; ++i) {

        if (i == kButton_UI_RB) {
            continue;
        }

        if (isInformationStage_ && i == kButton_UI_X) {
            //最初のステージの時は描画しない
            continue;
        }

        sprites_[i]->Draw();
    }

    if (isGetThermography_) {


        if (!isGetThermographyFirst_) {
            //サーモ初めましての時のみ描画する
            thermoGraphySprite_->Draw();
        }

        Sprite::PreDraw(kBlendModeScreen);

  
        sprites_[kButton_UI_RB]->Draw();
    }

}
