#pragma once

#include "Collider.h"
#include <memory>
#include"Texture.h"

class Memo : public Collider {

private:
    std::unique_ptr<Primitive>cubeMesh_ = nullptr;
    std::unique_ptr<Object3d> object_ = nullptr;
    const float kOffset_ = 0.25f;
    const float kThickness_ = 0.1f;
    //メモの文字列
    std::u32string string_ = {U""};
public:
    Memo();
    void Initialize();
    void Update();
    void Draw();

    void OnCollision(Collider* collider) override;
    void SetTexture(const uint32_t handle) {
        object_->SetSRVHandle(handle);
    };
    void SetTranslate(const Vector3& translate) {
        object_->SetTranslate(translate);
    }
    void SetMemoSize(const AABB& aabb) {
        AABB offset = { .min = {-kOffset_,-kThickness_,-kOffset_},.max = {kOffset_,kThickness_,kOffset_} };
        SetAABB(aabb+ offset);
        object_->SetScale(aabb.max - aabb.min);
        object_->SetMeshAndMaterial(cubeMesh_.get());
    };

    const AABB GetMemoSize() {
       Vector3 halfScale =  object_->GetScale()*0.5f;
       return AABB{ .min = -halfScale,.max = halfScale };
    }

    void SetColor(const Vector4& color) { object_->SetColor(color); }
    void SetString(const   std::u32string& string) {  string_ = string; }
    const std::u32string& GetString() { return string_; }

    WorldTransform& GetWorldTransform() { return object_->GetWorldTransform(); };
};

