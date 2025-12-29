#pragma once

#include<memory>
#include"Object3d.h"
#include"AnimationObject3d.h"
#include"AABB.h"
#include"SkinningModel.h"
#include"Model.h"
#include"Collider.h"
#include"DummyMedjed.h"

class Medjed :public DummyMedjed
{
private:
    std::unique_ptr<AnimationObject3d> aniObj_ = nullptr;
    std::unique_ptr <SkinningModel> skinningModel = nullptr;
    bool isFind_ = false;
    bool isHit_ = false;
private:

public:
    Medjed();
    void LookTarget(const Vector3& target)override;
    void Init()override;
    void Update()override;
    void Draw(Camera& camera)override;
    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const override;
    WorldTransform& GetWorldTransform()override
    {
        return aniObj_->worldTransform_;
    }
    void MoveStart();
    const bool& GetIsFind()const { return isFind_; };
    void SetIsFind(const bool& f) { isFind_ = f; };
    const  bool& GetIsHit() const{ return isHit_; };
};

