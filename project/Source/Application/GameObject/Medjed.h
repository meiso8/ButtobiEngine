#pragma once

#include<memory>
#include"Object3d.h"
#include"AABB.h"
class Model;
class Medjed
{
 
    std::unique_ptr < Object3d> object3d_;
    Model* model_ = nullptr;
    Matrix4x4* targetMatrix_ = nullptr;
public:
    Medjed();
    void Init();
    AABB GetWorldAABB()
    {
        Vector3 pos = object3d_->worldTransform_.GetWorldPosition();
        return AABB{
            .min = {pos - Vector3{0.5f,0.75f,0.5f}},
            .max = {pos + Vector3{0.5f,0.75f,0.5f}}
        };
    }
    void SetTargetMatrix( Matrix4x4* target );
    void Draw(Camera& camera);
    void Update();
};

