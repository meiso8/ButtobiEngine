#pragma once
#include<memory>
#include"Object3d.h"

class Camera;
class Model;

class Nest
{
private:
    std::unique_ptr<Object3d> object3d_;
    Model* model_ = nullptr;
public:
    Nest();
    void Init();
    void Update();
    void Draw(Camera& camera);

    void SetPosition(const Vector3& position);
};

