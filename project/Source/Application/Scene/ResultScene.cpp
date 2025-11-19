#include "ResultScene.h"
#include"Input.h"
ResultScene::ResultScene()
{

}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize()
{
    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);
}

void ResultScene::Update()
{
}

void ResultScene::Draw()
{
    sceneChange_->Draw();
}

void ResultScene::SceneChangeUpdate()
{
    // 何かをしたらシーン遷移
    if (Input::IsTriggerKey(DIK_SPACE)) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
    }    

    sceneChange_->Update();
}
