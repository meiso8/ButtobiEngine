#include "TitleScene.h"
#include"Input.h"
#include"SampleScene.h"
#include"MakeMatrix.h"
#include"DebugUI.h"
#include"Sound.h"
#include"Quaternion/Quaternion.h"
#include"CoordinateTransform.h"
#include"InputBind.h"
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
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
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

    Vector2 mousePos = Input::GetCursorPosition();
    if (IsCollision(mousePos, *startButton_)) {
        isHoverExitButton_ = false;
        startButton_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
        if (!isHoverStartButton_) {
            Sound::PlaySE(Sound::SWITCH_ON);
            isHoverStartButton_ = true;
      
        }
      
        if (InputBind::IsClick()) {

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
