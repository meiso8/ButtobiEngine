#pragma once

#include"Collider.h"
#include <memory>
#include"Object3d.h"

class Water : public Collider {
public:
    Water();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void OnCollision(Collider* collider) override;
    Vector3 GetWorldPosition() const override {
        return object_->worldTransform_.GetWorldPosition();
    }
private:
    std::unique_ptr<Object3d> object_;

};
