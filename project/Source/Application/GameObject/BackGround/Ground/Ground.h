#pragma once
#include<memory>
#include"Object3d.h"

class Camera;
class Model;

class Ground
{
private:
    std::unique_ptr <Object3d> object3d_;
    Model* model_ = nullptr;
public:
    Ground();
    void Init();
    void Update();
    void Draw(Camera& camera);
};

