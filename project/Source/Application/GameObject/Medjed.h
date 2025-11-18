#pragma once

#include<memory>
#include"Object3d.h"
class Model;
class Medjed
{
 
    std::unique_ptr < Object3d> object3d_;
    Model* model_ = nullptr;
    Matrix4x4* targetMatrix_ = nullptr;
public:
    Medjed();
    void Init();
    void SetTargetMatrix( Matrix4x4* target );
    void Draw(Camera& camera);
    void Update();
};

