#include "SceneManager.h"
#include"Input.h"

BaseScene::BaseScene()
{
    sceneChange_ = std::make_unique<SceneChange>();

    camera_ = std::make_unique<Camera>();
#ifdef _DEBUG
    // デバッグカメラの初期化
    debugCamera_ = std::make_unique<DebugCamera>();
#endif // _DEBUG
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
    isDebugCameraActive_ = isDebugCameraActive_ ? false : true;
    currentCamera_ = (isDebugCameraActive_) ? debugCamera_.get() : camera_.get();

}

// =========================================================================================

std::map < std::string, std::unique_ptr<BaseScene>> SceneManager::scenes_;
BaseScene* SceneManager::currentScene_ = nullptr;
std::map<std::string, std::unique_ptr<BaseScene>>::iterator SceneManager::currentIt_;

void SceneManager::Finalize()
{
    scenes_.clear();
    currentScene_ = nullptr;
}

void SceneManager::Update()
{

    currentScene_->SceneChangeUpdate();

    if (currentScene_->GetIsEndScene()) {

        ++currentIt_; // 次のシーンへ

        if (currentIt_ == scenes_.end()) {
            currentIt_ = scenes_.begin(); // 最初に戻る
        }

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
#ifdef _DEBUG

#ifdef USE_IMGUI
    for (const auto& [name, scene] : scenes_) {
        if (currentScene_ == scene.get()) {
            ImGui::Text("%s", name.c_str());
        }
    }

#endif // USE_IMGUI
    currentScene_->Debug();
#endif
}

void SceneManager::SetMap(const std::string& name, std::unique_ptr<BaseScene> scene)
{
    scenes_[name] = std::move(scene);
}

void SceneManager::SetItr(const std::string& name)
{
    //最初の位置を保持
    currentIt_ = scenes_.find(name);
}

void SceneManager::InitScene()
{
    currentScene_ = currentIt_->second.get();
    currentScene_->Initialize();
}
