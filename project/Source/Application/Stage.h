#pragma once
#include"WorldTransform.h"

class Camera;
class Model;

class Stage {

private:

    Model* model_ = nullptr;
    WorldTransform worldTransform_;
    Camera* camera_ = nullptr;

public:

    void Initialize(Camera* camera);
    void Update();
    void Draw();
    ~Stage();
};
