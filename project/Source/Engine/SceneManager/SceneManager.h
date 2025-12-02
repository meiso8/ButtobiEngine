#pragma once
#include"Camera.h"
#include"DebugCamera.h"
#include<memory>

#include"SceneChange.h"

class SceneManager
{
protected:
    //カメラ
    bool isDebugCameraActive_ = false;
    std::unique_ptr<Camera> camera_ = nullptr;
    std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
    Camera* currentCamera_ = nullptr;
    std::unique_ptr <SceneChange> sceneChange_ = nullptr;
public:
    SceneManager();
    virtual ~SceneManager() = default;
    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void Debug();
    virtual void SceneChangeUpdate();
    bool GetIsEndScene() {
        if (sceneChange_ == nullptr) { 
            return false;
        }
        // nullptr でない場合は IsEndScene() を呼び出す
        return sceneChange_->IsEndScene(); 
    };

    void SwitchCamera();
};