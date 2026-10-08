#pragma once
#include"BaseScene.h"
#include<memory>
#include"SkyBoxObject3d.h"
#include"LevelEditor/LevelEditor.h"
#include <vector>
#include"AnimationObject3d.h"

class Skip;
class SecondStoryScene :public BaseScene
{
public:
    SecondStoryScene();
    ~SecondStoryScene() override;
    void Initialize()override;
    void Update()override;
    void DrawModel()override;
    void DrawSprite()override;
    void SetSceneChange();
private:
    void SceneChange();
    void Debug();
private:
    std::vector<std::unique_ptr<LevelEditor::ObjectSet>> objects_;
#pragma region//SkyBox
    std::unique_ptr<SkyboxObject3d>skyboxObject3d_ = nullptr;
#pragma endregion
    std::unique_ptr<Skip>skip_ = nullptr;
    //アニメーション付きオブジェクト
    std::unique_ptr<AnimationObject3d> aniObject_ = nullptr;

};

