#include "SceneManager.h"
#include"Input.h"

SceneManager::SceneManager()
{
    sceneChange_ = std::make_unique<SceneChange>();

    camera_ = std::make_unique<Camera>();
#ifdef _DEBUG
    // デバッグカメラの初期化
    debugCamera_ = std::make_unique<DebugCamera>();
#endif // _DEBUG
}

void SceneManager::Initialize()
{

}

void SceneManager::Update()
{
}

void SceneManager::Draw()
{
}

void SceneManager::Debug()
{

}

void SceneManager::SceneChangeUpdate()
{

    //// 何かをしたらシーン遷移
    //if (Input::IsTriggerKey(DIK_SPACE)) {
    //    sceneChange_->SetState(SceneChange::kFadeIn, 30);
    //}

#ifdef _DEBUG
    // 何かをしたらシーン遷移
    if (Input::IsTriggerKey(DIK_I)) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
    }
#endif
    sceneChange_->Update();
}

void SceneManager::SwitchCamera()
{
    isDebugCameraActive_ = isDebugCameraActive_ ? false : true;
    currentCamera_ = (isDebugCameraActive_) ? debugCamera_.get() : camera_.get();

}
