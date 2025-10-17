#pragma once
#include"WorldTransform.h"

class Camera;
class Model;

class Stage {

private:
    Model* model_ = nullptr;
    WorldTransform worldTransform_;
public:
    Stage();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    ~Stage();
};
