#include "SceneManager.h"
#include"Input.h"

BaseScene::BaseScene()
{
    sceneChange_ = std::make_unique<SceneChange>();
    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);

    camera_ = std::make_unique<Camera>();
#ifdef _DEVELOP
    // デバッグカメラの初期化
    debugCamera_ = std::make_unique<DebugCamera>();
#endif //_DEVELOP
}

void BaseScene::Initialize()
{

}

void BaseScene::Update()
{
}

void BaseScene::Draw()
{
}

void BaseScene::Debug()
{
}

void BaseScene::SceneChangeUpdate()
{
#ifdef _DEBUG

    if (Input::IsTriggerKey(DIK_R)) { Initialize(); }

    // 何かをしたらシーン遷移
    if (Input::IsTriggerKey(DIK_I)) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
    }

#endif
    sceneChange_->Update();
}

void BaseScene::SwitchCamera()
{
    isDebugCameraActive_ = !isDebugCameraActive_;
    currentCamera_ = (isDebugCameraActive_) ? debugCamera_.get() : camera_.get();
}

// =========================================================================================

std::map < std::string, std::unique_ptr<BaseScene>> SceneManager::scenes_;
BaseScene* SceneManager::currentScene_ = nullptr;
BaseScene* SceneManager::nextScene_ = nullptr;

void SceneManager::Finalize()
{
    scenes_.clear();
    currentScene_ = nullptr;
    nextScene_ = nullptr;
}

void SceneManager::Update()
{

    currentScene_->SceneChangeUpdate();

    if (currentScene_->GetIsEndScene()) {

        InitScene();
    }

    // シーンの更新処理
    currentScene_->Update();
}

void SceneManager::Draw()
{
    currentScene_->Draw();
}

void SceneManager::Debug()
{

#ifdef USE_IMGUI
    for (const auto& [name, scene] : scenes_) {
        if (currentScene_ == scene.get()) {
            ImGui::Text("%s", name.c_str());
        }
    }

#endif // USE_IMGUI
    currentScene_->Debug();

}

void SceneManager::SetMap(const std::string& name, std::unique_ptr<BaseScene> scene)
{
    scenes_[name] = std::move(scene);
}

void SceneManager::SetNestScene(const std::string& name)
{
    //最初の位置を保持
    nextScene_ = scenes_[name].get();
}

void SceneManager::InitScene()
{
    //次のシーンがセットされていたら次のシーンにする
    if (nextScene_) {
        currentScene_ = nextScene_;
        nextScene_ = nullptr;
    }

    currentScene_->Initialize();
}
