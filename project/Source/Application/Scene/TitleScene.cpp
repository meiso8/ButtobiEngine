#include "TitleScene.h"
#include"Input.h"
#include"SampleScene.h"
#include"MakeMatrix.h"
#include"DebugUI.h"
#include"Sound.h"
#include"Quaternion/Quaternion.h"
#include"CoordinateTransform.h"
TitleScene::TitleScene()
{
    puzzle_ = std::make_unique<Puzzle>();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);
    Sound::StopAllSound();
    puzzle_->Init();
}

void TitleScene::Update()
{
    puzzle_->Update();

    if (puzzle_->GetIsGameEnd()) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
        SceneManager::SetNestScene("Sample");
    }
}

void TitleScene::Draw()
{
    puzzle_->Draw();
    sceneChange_->Draw();
}

void TitleScene::Debug()
{

}

void TitleScene::SceneChangeUpdate()
{
    // 何かをしたらシーン遷移
    if (Input::IsTriggerKey(DIK_SPACE)) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
        SceneManager::SetNestScene("Sample");
    }

    sceneChange_->Update();
}
