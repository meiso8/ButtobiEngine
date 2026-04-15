#pragma once
#include "SceneManager.h"
#include"../Engine/FreeTypeManager/Text.h"
#include<string>
#include"Object3d/Object3d.h"
#include"Mesh/SkyBox/SkyBox.h"

class FreeTypeScene : public BaseScene
{
public:
    FreeTypeScene();
    void Initialize()override;
    void Update()override;
    void Draw()override;
private:
    std::u32string inputText_;
    Text text_;
    uint32_t handle_;
    Text pressSpaceText_;
    std::unique_ptr<Sprite>sprite_ = nullptr;
    std::unique_ptr<Object3d> object3d_ = nullptr;
    std::unique_ptr<SkyBox> skyBox_ = nullptr;
};

