#pragma once
#include"Collider.h"
#include <memory>
#include"AnimationObject3d.h"
#include"SkinningModel.h"
class Mummy : public Collider {
public:
    Mummy();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void OnCollision(Collider* collider) override;

    Vector3 GetWorldPosition() const override {
        return aniObj_->worldTransform_.GetWorldPosition();
    }

private:
    std::unique_ptr<Object3d> object_;
    std::unique_ptr<AnimationObject3d> aniObj_;
    Model* coffinModel_ = nullptr;
    std::unique_ptr<SkinningModel>skinningModel_ = nullptr;
};
