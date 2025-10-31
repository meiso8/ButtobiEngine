#pragma once
#include"SceneManager.h"
#include"WorldTransform.h"
#include "Sprite.h"
#include<memory>
#include<array>

class QuadMesh;
class SphereMesh;
class Model;
class TitleScene :public SceneManager {

public:
    TitleScene();
    ~TitleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
    std::unique_ptr<QuadMesh> quadMesh_= nullptr;
    std::unique_ptr<SphereMesh> sphereMesh_ = nullptr;
};
