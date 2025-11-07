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

    SceneChange sceneChange_;

public:
    SceneManager() = default;
    virtual ~SceneManager() = default;
    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void Debug();
    virtual bool GetIsEndScene() {
        return sceneChange_.isEndScene_
            ;
    };
    void SetIsEndScene(bool isEndScene) { sceneChange_.isEndScene_ = isEndScene; };
    void SwitchCamera();

};