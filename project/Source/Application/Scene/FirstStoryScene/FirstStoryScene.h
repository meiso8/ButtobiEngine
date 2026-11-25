#pragma once
#include"BaseScene.h"
#include<memory>
#include"SkyBoxObject3d.h"
#include"LevelEditor/LevelEditor.h"
#include <vector>
class FirstStoryZero;
class FirstStoryOne;

class Skip;
class FirstStoryScene :public BaseScene
{
public:
    FirstStoryScene();
    ~FirstStoryScene() override;
    void Initialize()override;
    void Update()override;
    void DrawModel()override;
    void DrawSprite()override;
    void SetSceneChange();
private:
    void Debug();
    void CameraUpdate();
    void SceneChange();
private:
    std::vector<std::unique_ptr<LevelEditor::ObjectSet>> objects_;
#pragma region//SkyBox
    std::unique_ptr<SkyboxObject3d>skyboxObject3d_ = nullptr;
#pragma endregion

    Vector3 middlePos_ = { 0.0f,0.6f,0.5f };
    float cameraFovAngle_ = 2.326f;

    std::unique_ptr<Skip>skip_ = nullptr;
    
    std::unique_ptr<FirstStoryZero>storyZero_ = nullptr;
    std::unique_ptr<FirstStoryOne>storyOne_ = nullptr;
};

