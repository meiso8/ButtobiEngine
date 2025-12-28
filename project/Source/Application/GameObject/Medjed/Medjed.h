#pragma once

#include<memory>
#include"Object3d.h"
#include"AnimationObject3d.h"
#include"AABB.h"
#include"SkinningModel.h"
#include"Model.h"
#include"Collider.h"

class Medjed :public Collider
{
private:
    Model* model_ = nullptr;
    Vector3* targetPos_ = nullptr;
    std::unique_ptr<AnimationObject3d> aniObj_ = nullptr;
    std::unique_ptr < SkinningModel> skinningModel = nullptr;
    AABB localAABB_ = { .min = {-0.5f,0.0f,-0.5f},.max = {0.5f,1.4f,0.5f} };

    bool* isEnemyApperPtr_ = nullptr;
private:
    void LookTarget();
public:

    Medjed();
    void SetIsEnemyApperPtr(bool* flag) { isEnemyApperPtr_ = flag; }
    void Init();
    void Update();
    void Draw(Camera& camera);
    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const override;

    WorldTransform& GetWorldTransform()
    {
        return aniObj_->worldTransform_;
    }

    void SetTarget(Vector3& target) {
        targetPos_ = &target;
    };

};

