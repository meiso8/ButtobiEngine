#include "TitleScene.h"
#include"Input.h"


TitleScene::TitleScene()
{
    sceneName_ = "Title";
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);
}

void TitleScene::Update()
{


}

void TitleScene::Draw()
{
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
    }

    sceneChange_->Update();
}
