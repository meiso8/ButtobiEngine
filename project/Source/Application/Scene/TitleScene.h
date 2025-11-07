#pragma once
#include"SceneManager.h"
#include"WorldTransform.h"
#include "Sprite.h"
#include<memory>
#include<array>
#include"Object3d.h"
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
    std::unique_ptr<Model> model_ = nullptr;

    std::array< std::unique_ptr < Object3d>, 3> object3ds_;
};
