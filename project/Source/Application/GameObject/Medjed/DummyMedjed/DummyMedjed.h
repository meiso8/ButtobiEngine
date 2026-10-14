#pragma once

#include<memory>
#include"Object3d.h"
#include"Collider.h"
#include"AABB.h"

class Model;
class DummyMedjed :public Collider {


public:
    DummyMedjed();
    virtual void Init();
    virtual void Draw();
    virtual void Update();
    void OnCollision(Collider* collider)override;
    virtual void Look(const Vector3& target);

    virtual void SetTranslate(const Vector3& translate) { object3d_->SetTranslate(translate); }
    virtual void GoToTarget(const Vector3& target);
    virtual void Hide();
    const bool IsHide() { return hideTimer_ >= 1.0f; };
    virtual void SetColor(const Vector4& color) { object3d_->SetColor(color); };
    void SetModel(Model* model) { model_ = model; };
   const Model* GetModelPtr() { return model_; };
    void SetHideTimer(const float& timer) { hideTimer_ = timer; };
    void SetAniTimer(const float& timer) { aniTimer_ = timer; };
    const float& GetHideTimer() { return hideTimer_; }
    const float& GetAniTimer() { return aniTimer_; }
    const AABB& GetLocalAABB() { return localAABB_; }
private:
    std::unique_ptr < Object3d> object3d_ = nullptr;
    float rotateRange_ = 6.28f;
    float startRotateY_ = 0.0f;
    float startPosY_ = 0.0f;

    Model* model_ = nullptr;
    static inline const  AABB localAABB_ = { .min = {-0.2f,0.0f,-0.2f},.max = {0.2f,1.5f,0.2f} };
    float aniTimer_ = 0.0f;
    float hideTimer_ = 0.0f;
};

