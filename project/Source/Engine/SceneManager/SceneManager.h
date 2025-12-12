#pragma once
#include"Camera.h"
#include"DebugCamera.h"
#include<memory>
#include<vector>
#include"SceneChange.h"

class BaseScene
{
protected:
    //カメラ
    bool isDebugCameraActive_ = false;
    std::unique_ptr<Camera> camera_ = nullptr;
    std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
    Camera* currentCamera_ = nullptr;
    std::unique_ptr <SceneChange> sceneChange_ = nullptr;
public:
    BaseScene();
    virtual ~BaseScene() = default;
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

#include<map>

class SceneManager {
public:
    static void Finalize();
    static void Update();
    static void Draw();
    static void Debug();
    static void SetMap(const std::string& name, std::unique_ptr<BaseScene> scene);
    static void SetNestScene(const std::string& name);
    static void InitScene();
private:

    static BaseScene* currentScene_;
    static BaseScene* nextScene_;
    static std::map < std::string, std::unique_ptr<BaseScene>> scenes_;
};

