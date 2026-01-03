#include "TitleScene.h"
#include"Input.h"
#include"SampleScene.h"
#include"MakeMatrix.h"
#include"DebugUI.h"
#include"Sound.h"
#include"Quaternion/Quaternion.h"
#include"CoordinateTransform.h"
#include"InputBind.h"
#include"Window.h"

TitleScene::TitleScene()
{
    //タイトル画像 
    titleSprite_ = std::make_unique<Sprite>();
    titleSprite_->Create(Texture::TEXTURE_HANDLE::TITLE, { 640.0f, 256.0f });
    titleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
    // スタートボタン 
    startButton_ = std::make_unique<Sprite>();
    startButton_->Create(Texture::TEXTURE_HANDLE::BUTTON_START, { 640.0f,448.0f });
    startButton_->SetAnchorPoint({ 0.5f, 0.5f });
    // Exitボタン 
    exitButton_ = std::make_unique<Sprite>();
    exitButton_->Create(Texture::TEXTURE_HANDLE::BUTTON_EXIT, { 640.0f, 528.0f });
    exitButton_->SetAnchorPoint({ 0.5f, 0.5f });

    for (auto& sprite : test3Spritea_) {
        sprite = std::make_unique<Sprite>();
        sprite->Create(Texture::TEXTURE_HANDLE::TEST3, { 0.0f, 360.0f });
        sprite->SetSize({ static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()) *0.5f});
        sprite->SetUVScale({ 0.5f,1.0f,1.0f });
    }

    for (int i = 0; i < test3Spritea_.size(); ++i) {
        test3Spritea_[i]->SetUVTranslate({ 0.0f, 0.0f, 0.0f });
        test3Spritea_[i]->SetPosition({ 0.0f, test3Spritea_[i]->GetSize().y * i });

    }

}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

        for (int i = 0; i < test3Spritea_.size(); ++i) {
            test3Spritea_[i]->SetUVTranslate({ 0.0f, 0.0f, 0.0f });
        }

        isHoverStartButton_ = false;
        isHoverExitButton_ = false;
        sceneChange_->Initialize();
        sceneChange_->SetState(SceneChange::kFadeOut, 60);
        Sound::bgmVolume_ = 0.5f;
        Sound::StopAllSound();

    
}

void TitleScene::Update()
{
    Sound::PlayBGM(Sound::BGM_ArabRuins);
    startButton_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
    exitButton_->SetColor({ 1.0f,1.0f,1.0f,1.0f });


    test3Spritea_[0]->GetUVTranslate().x += 0.001f;
    test3Spritea_[1]->GetUVTranslate().x -= 0.001f;
    for (auto& sprite : test3Spritea_) {
        sprite->Update();
    }
    Vector2 mousePos = Input::GetCursorPosition();
    if (IsCollision(mousePos, *startButton_)) {
        isHoverExitButton_ = false;
        startButton_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
        if (!isHoverStartButton_) {
            Sound::PlaySE(Sound::SWITCH_ON);
            isHoverStartButton_ = true;

        }

        if (InputBind::IsClick()) {
            Sound::PlaySE(Sound::FALL);
            sceneChange_->SetState(SceneChange::kFadeIn, 30);
            SceneManager::SetNestScene("Sample");
        }

    }

    if (IsCollision(mousePos, *exitButton_)) {
        isHoverStartButton_ = false;
        exitButton_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
        if (!isHoverExitButton_) {
            Sound::PlaySE(Sound::SWITCH_ON);
            isHoverExitButton_ = true;

        }

        if (InputBind::IsClick()) {
            // アプリケーション終了 
            PostQuitMessage(0); // Windows APIでウィンドウを閉じる 
        }

    }
}

void TitleScene::Draw()
{
    Sprite::PreDraw();
    for (auto& sprite : test3Spritea_) {
        sprite->Draw();
    }
    titleSprite_->Draw();
    startButton_->Draw();
    exitButton_->Draw();

    sceneChange_->Draw();
}

void TitleScene::Debug()
{

}

void TitleScene::SceneChangeUpdate()
{


    sceneChange_->Update();
}
