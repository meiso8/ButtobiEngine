#pragma once

#include"Collider.h"
#include <memory>
#include"Object3d.h"

class PuzzleObj : public Collider {
public:
    PuzzleObj();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void OnCollision(Collider* collider) override;
    Vector3 GetWorldPosition() const override {
        return object_->worldTransform_.GetWorldPosition();
    }
    void SetColor(const Vector4& color) { object_->SetColor(color); };
private:
    std::unique_ptr<Object3d> object_;

};
