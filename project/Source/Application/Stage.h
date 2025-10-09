#pragma once
#include"WorldTransform.h"
#include<memory>

class Model;
class Camera;

class Stage {

private:

    std::unique_ptr<Model>model_ = nullptr;
    WorldTransform worldTransform_;
    Camera* camera_ = nullptr;

public:

    void Initialize(Camera* camera);
    void Update();
    void Draw();

};
