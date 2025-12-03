#include "TitleScene.h"
#include"Input.h"
#include"SampleScene.h"
#include"MakeMatrix.h"
#include"DebugUI.h"
TitleScene::TitleScene()
{




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
    Vector3 from0 = Normalize(Vector3{ 1.0f,0.7f,0.5f });
    Vector3 to0 = -from0;
    Vector3 from1 = Normalize(Vector3{-0.6f,0.9f,0.2f });
    Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });

    Matrix4x4 rotateMatrix0 = DirectionToDirection(
        Normalize(Vector3{ 1.0f,0.0f,0.0f }), 
       Normalize(Vector3{-1.0f,0.0f,0.0f})
    );


    Matrix4x4 rotateMatrix1 = DirectionToDirection(from0,to0);
    Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

    Vector3 axis = Normalize({1.0f,1.0f,1.0f});
    float angle = 0.44f;
    Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);
#ifdef _DEBUG
    DebugUI::ShowMatrix4x4(rotateMatrix0,"rotateMatrix0");
    DebugUI::ShowMatrix4x4(rotateMatrix1,"rotateMatrix1");
    DebugUI::ShowMatrix4x4(rotateMatrix2, "rotateMatrix2");
    DebugUI::ShowMatrix4x4(rotateMatrix, "rotateMatrix");
#endif // _DEBUG


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
