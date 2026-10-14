#pragma once
#include "Collider.h"
#include <unordered_map>
#include"LevelEditor/LevelEditor.h"

class StageChangeTrigger : public Collider {
public:
    StageChangeTrigger();
    ~StageChangeTrigger();
    void Create(const LevelData::StageChangeTriggerData& data
    );
    void Update();
    void Draw();
    // コライダーが何かに当たった時に呼ばれる
    void OnCollision(Collider* collider) override;
    const std::string GetObjectString();
    const Vector3 GetWorldPos() const{ return object_->GetWorldTransform().GetWorldPosition(); };
private:
    std::unique_ptr<Object3d> object_ = nullptr;
};

