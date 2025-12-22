#include "TitleScene.h"
#include"Input.h"
#include"SampleScene.h"
#include"MakeMatrix.h"
#include"DebugUI.h"
#include"Sound.h"
#include"Quaternion/Quaternion.h"
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
    Vector3 from0 = Normalize(Vector3{ 1.0f,0.7f,0.5f });
    Vector3 to0 = -from0;
    Vector3 from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
    Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });

    Matrix4x4 rotateMatrix0 = DirectionToDirection(
        Normalize(Vector3{ 1.0f,0.0f,0.0f }),
        Normalize(Vector3{ -1.0f,0.0f,0.0f })
    );

    Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
    Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

    Vector3 axis = { 1.0f,1.0f,1.0f };
    float angle = 0.44f;
    Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);
    Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
    Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
    Quaternion identity = IdentityQuaternion();
    Quaternion conj = Conjugate(q1);
    Quaternion inv = Inverse(q1);
    Quaternion normal = Normalize(q1);
    Quaternion mul1 = Multiply(q1, q2);
    Quaternion mul2 = Multiply(q2, q1);
    float norm = Norm(q1);

    puzzle_->Update();

#ifdef _DEBUG
    DebugUI::ShowMatrix4x4(rotateMatrix0, "rotateMatrix0");
    DebugUI::ShowMatrix4x4(rotateMatrix1, "rotateMatrix1");
    DebugUI::ShowMatrix4x4(rotateMatrix2, "rotateMatrix2");
    DebugUI::ShowMatrix4x4(rotateMatrix, "rotateMatrix");

    ImGui::SliderFloat4("Identity", &identity.x, -100.0f, 100.0f);
    ImGui::SliderFloat4("Conjugate", &conj.x, -100.0f, 100.0f);
    ImGui::SliderFloat4("Inverse", &inv.x, -100.0f, 100.0f);
    ImGui::SliderFloat4("Normalize", &normal.x, -100.0f, 100.0f);
    ImGui::SliderFloat4("Multiply(q1,q2)", &mul1.x,-100.0f,100.0f);
    ImGui::SliderFloat4("Multiply(q2,q1)", &mul2.x, -100.0f, 100.0f);
    ImGui::SliderFloat("Norm", &norm, -100.0f, 100.0f);
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
