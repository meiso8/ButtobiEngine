#pragma once

#include<memory>
#include"Object3d.h"
#include"Collider.h"
#include"AABB.h"
class Model;
class Locker :public Collider {

private:
    std::unique_ptr < Object3d> object3d_;
    Model* model_ = nullptr;
public:
    static inline bool isSetMesh_ = false;
    Locker();
    void Init();
    void SetPosX(const float& x) { object3d_->worldTransform_.translate_.x = x; };
    void Draw(Camera& camera);
    void Update();
    void UpdateSetMesh();

    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const override;
};

