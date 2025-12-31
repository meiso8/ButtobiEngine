#pragma once
#include "Collider.h"
#include <memory>
#include"CubeMesh.h"
class Platform : public Collider {
public:
    Platform();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    Vector3 GetWorldPosition() const override;
    WorldTransform& GetWorldTransform() { return object_->worldTransform_; };
    void OnCollision(Collider* collider) override;
    std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
private:
    std::unique_ptr<Object3d> object_;
};
