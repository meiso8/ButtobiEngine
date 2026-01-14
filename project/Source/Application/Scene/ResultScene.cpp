#include "ResultScene.h"
#include"Input.h"
#include"InputBind.h"
#include"Sound.h"
#include<algorithm>
#include"TimeManager.h"
ResultScene::~ResultScene() {}

ResultScene::ResultScene()
{

    skipSprite_ = std::make_unique<Sprite>();
    skipSprite_->Create(Texture::TEXTURE_HANDLE::SKIP, { 1280.0f-128.0f-64.0f, 720.0f-64.0f });
    skipSprite_->SetAnchorPoint({ 0.5f, 0.5f });

    symbolSprite_ = std::make_unique<Sprite>();
    symbolSprite_->Create(Texture::TEXTURE_HANDLE::PUZZLE, { 640.0f, 360.0f });
    symbolSprite_->SetAnchorPoint({ 0.5f, 0.5f });

    // 切り替えるテクスチャの順番
    textureSequence_ = {
        Texture::TEXTURE_HANDLE::ENDING2,
        Texture::TEXTURE_HANDLE::BD_HUNEFER,
        Texture::TEXTURE_HANDLE::ENDING1,
         Texture::TEXTURE_HANDLE::PUZZLE,
    };

    creditSprite_ = std::make_unique<Sprite>();
    creditSprite_->Create(Texture::TEXTURE_HANDLE::CREDIT, { 640.0f, 360.0f });
    creditSprite_->SetAnchorPoint({ 0.5f, 0.5f });

}


void ResultScene::Initialize()
{
    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);

    creditSprite_->SetUVTranslate({ 0.0f, 0.0f, 0.0f });
    timer_ = 0.0f;

    currentIndex_ = 0;
    timer_ = 0.0f;
    fadeSpeed_ = kInverseFPS * 2.0f;
    Sound::StopAllSound();
    lookTimer_ = 0.0f;

    isSkipDraw_ = false;
}

void ResultScene::Update()
{
    Sound::PlayBGM(Sound::BGM_SandCity);

    if (InputBind::IsClick()) {

        Sound::PlaySE(Sound::FALL);

        if (lookTimer_ >= 10.0f) {
            sceneChange_->SetState(SceneChange::kFadeIn, 30);
            SceneManager::SetNestScene("Title");
        } else {

            if (lookTimer_ <= 10.0f) {
                lookTimer_ = 10.0f;
                isSkipDraw_ = true;
            }
        }

    }


    symbolSprite_->Update();
    lookTimer_ +=kInverseFPS;
    timer_ += kInverseFPS;

    if (timer_ >= switchInterval_) {
        timer_ = 0.0f;
        currentIndex_ = (currentIndex_ + 1) % textureSequence_.size();
        symbolSprite_->SetTexture(textureSequence_[currentIndex_]);
        symbolSprite_->AdjustTextureSize();
    }

    if (timer_ <= 0.5f) {
        // αを徐々に増やす 
        alpha_ += fadeSpeed_;
    }

    if (timer_ >= 4.5f) {
        // αを徐々に 減らす
        alpha_ -= fadeSpeed_;
    }

    alpha_ = std::clamp(alpha_, 0.0f, 0.5f);
    // 色を設定（白＋α） 
    symbolSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });

    // UVのYを上にスクロール 
    creditSprite_->GetUVTranslate().y += scrollSpeed_;
    creditSprite_->Update();

#ifdef _DEBUG
    ImGui::SliderFloat("scrollSpeed", &scrollSpeed_, -1.0f, 1.0f);
    ImGui::SliderFloat("fadeSpeed_", &fadeSpeed_, -1.0f, 1.0f);
#endif // _DEBUG


}

void ResultScene::Draw()
{
    sceneChange_->Draw();
    Sprite::PreDraw();
    symbolSprite_->Draw();
    creditSprite_->Draw();

    if (isSkipDraw_) {
        skipSprite_->Draw();
    }
  
}

void ResultScene::SceneChangeUpdate()
{
    sceneChange_->Update();
}
