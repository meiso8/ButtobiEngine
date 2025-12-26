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

    Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f,0.71f,0.0f }, 0.3f);
    Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f,0.0f,0.71f }, 3.141592f);

    Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
    Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
    Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
    Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
    Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);


#ifdef _DEBUG

    ImGui::SliderFloat4("interpolate0 , Slerp(q0,q1,0.0f)", &interpolate0.x, -100.0f, 100.0f);
    ImGui::SliderFloat4("interpolate1 , Slerp(q0,q1,0.3f)", &interpolate1.x, -100.0f, 100.0f);
    ImGui::SliderFloat4("interpolate2 , Slerp(q0,q1,0.5f)", &interpolate2.x, -100.0f, 100.0f);
    ImGui::SliderFloat4("interpolate3 , Slerp(q0,q1,0.7f)", &interpolate3.x, -100.0f, 100.0f);
    ImGui::SliderFloat4("interpolate4 , Slerp(q0,q1,1.0f)", &interpolate4.x, -100.0f, 100.0f);


#endif // _DEBUG

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
