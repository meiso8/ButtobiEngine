#pragma once
#include"BaseScene.h"
#include<memory>
#include"SkyBoxObject3d.h"
#include"LevelEditor/LevelEditor.h"
#include <vector>
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
    void PlayInterCome(const int32_t index);
    void SceneChange();
private:
    std::vector<std::unique_ptr<LevelEditor::ObjectSet>> objects_;
#pragma region//SkyBox
    std::unique_ptr<SkyboxObject3d>skyboxObject3d_ = nullptr;
#pragma endregion
    float cameraTimer_ = 0.0f;
    Vector3 middlePos_ = { 0.0f,0.6f,0.5f };
    float cameraFovAngle_ = 2.326f;
    std::array<bool,3> isInterComeRing_;
    bool isHorror_ = false;
    std::unique_ptr<Skip>skip_ = nullptr;
};

