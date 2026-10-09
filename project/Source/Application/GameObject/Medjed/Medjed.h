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

public:
    Medjed();
    ~Medjed();
    void Look(const Vector3& target)override;
    void Init()override;
    void Update()override;
    void Draw()override;
    void OnCollision(Collider* collider)override;

    const WorldTransform& GetWorldTransform() { return aniObj_->GetWorldTransform(); }
    void SetTranslate(const Vector3& translate) { aniObj_->SetTranslate(translate); }
    Vector3 GetWorldPosition() const;
    void GoToTarget(const Vector3& target)override;

    void MoveStart();
    const bool& GetIsFind()const { return isFind_; };
    void SetIsFind(const bool& f) { isFind_ = f; };
    const  bool& GetIsHit() const { return isHit_; };
    void SetColor(const Vector4& color) { aniObj_->SetColor(color); };

private:
    std::unique_ptr<AnimationObject3d> aniObj_ = nullptr;
    Vector3 velocity_ = { 0.0f };

    bool isFind_ = false;
    bool isHit_ = false;

};

