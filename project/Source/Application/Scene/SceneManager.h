#pragma once
#include"Camera/Camera.h"
#include"Camera/DebugCamera.h"
#include<memory>
class SceneManager
{
protected:
    //カメラ
    bool isDebugCameraActive_ = false;
    std::unique_ptr<Camera> camera_ = nullptr;
    std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
    Camera* currentCamera_ = nullptr;
    bool isEndScene_ = false;
public:
    SceneManager() = default;
    ~SceneManager() = default;
    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void Debug();
    virtual bool GetIsEndScene() = 0;
    void SwitchCamera();

};