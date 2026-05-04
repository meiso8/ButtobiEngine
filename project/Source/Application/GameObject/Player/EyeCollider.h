#pragma once
#include"Collider.h"
#include<memory>
class EyeCollider:public Collider
{
private:
    std::unique_ptr <Object3d> object_;
    float walkingTheta_ = 0.0f;
    Vector3 prevPos_;
public:
    const float   kEyeDefaultPosY_ = 1.5f;
    const float   kEyeDefaultPosZ_ = 0.0f;

    void SavePosition() { prevPos_ = object_->worldTransform_.translate_; }
    void BackToPrevPosition() { object_->worldTransform_.translate_ = prevPos_; }

    EyeCollider();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void OnCollision(Collider* collider)override;
     Matrix4x4& GetWorldMatrix() {
        return object_->worldTransform_.matWorld_;
    }
     WorldTransform& GetWorldTransform() { return object_->worldTransform_; };
     void SetParent(WorldTransform& parent) { object_->worldTransform_.Parent(parent); }
     Vector3& GetForward();

     void Walk(const float& speed);
     void WalkStop();
     void MouseLook(const float& rotateX);
};
