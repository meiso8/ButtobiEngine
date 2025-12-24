#pragma once

#include<memory>
#include"Object3d.h"
#include"AnimationObject3d.h"
#include"AABB.h"
class Model;
class Medjed
{
 
    std::unique_ptr < Object3d> object3d_;
    Model* model_ = nullptr;
    Vector3* targetPos_ = nullptr;
    std::unique_ptr<AnimationObject3d> aniObj_ = nullptr;
public:
    Medjed();
    void Init();
    AABB GetWorldAABB()
    {
        Vector3 pos = GetWorldPos();
        return AABB{
            .min = {pos - Vector3{0.2f,0.0f,0.2f}},
            .max = {pos + Vector3{0.2f,1.4f,0.2f}}
        };
    }
    Vector3 GetWorldPos()
    {
       return object3d_->worldTransform_.GetWorldPosition();

    }
    WorldTransform& GetWorldTransform()
    {
        return object3d_->worldTransform_;

    }

    void SetTarget(Vector3& target) {
        targetPos_ = &target
            ;
    };
    void LookTarget();
    void Draw(Camera& camera);
    void Update();
};

