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
public:
    SceneManager() = default;
    ~SceneManager() = default;
    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void Debug();

    void SwitchCamera();

};