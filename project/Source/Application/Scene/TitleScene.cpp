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

    Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
    Vector3 pointY = { 2.1f,-0.9f,1.3f };
    Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
    Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
    Vector3 rotateByMatrix = CoordinateTransform(pointY, rotateMatrix);

#ifdef _DEBUG


    ImGui::SliderFloat4("rotation", &rotation.x, -100.0f, 100.0f);
    DebugUI::ShowMatrix4x4(rotateMatrix, "rotateMatrix");
    ImGui::SliderFloat3("rotateByQuaternion", &rotateByQuaternion.x, -100.0f, 100.0f);
    ImGui::SliderFloat3("rotateByMatrix", &rotateByMatrix.x, -100.0f, 100.0f);


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
