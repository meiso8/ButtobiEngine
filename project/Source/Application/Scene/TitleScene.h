#pragma once
#include"SceneManager.h"
#include"WorldTransform.h"
#include "Sprite.h"
#include<memory>
#include<array>

class QuadMesh;
class Model;
class TitleScene :public SceneManager {

public:
    TitleScene();
    ~TitleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
};
