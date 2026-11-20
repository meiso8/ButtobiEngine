#pragma once

#include"Collider.h"
#include <memory>
#include"AnimationObject3d.h"

class PuzzleObj : public Collider {
public:
    PuzzleObj();
    ~PuzzleObj();
    
    void Initialize(const Vector3& pos, const float& rotY);
    void Update();
    void Draw();
    void OnCollision(Collider* collider) override;

    Vector3 GetWorldPosition() {
      return  aniObj_->GetWorldTransform().GetWorldPosition();
    }
    void SetIsOpen(const bool flag) { isOpen_ = flag; }
    void RegisterObj() { aniObj_->RegisterObject(); };
private:
    bool isOpen_ = false;
    std::unique_ptr<AnimationObject3d> aniObj_ = nullptr;

};
