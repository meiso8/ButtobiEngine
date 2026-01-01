#pragma once
#include "Collider.h"
#include <memory>
#include"CubeMesh.h"
#include"Texture.h"
class Platform : public Collider {
public:
    Platform();
    void SetTextureHandle(const Texture::TEXTURE_HANDLE& handle) {
        object_->SetTextureHandle(handle);
    }
    const Texture::TEXTURE_HANDLE GetTextureHandle() { return object_->GetTextureHandle(); }
    virtual void Initialize();
    virtual void Update();
    virtual void Draw(Camera& camera);
    Vector3 GetWorldPosition() const override;
    WorldTransform& GetWorldTransform() { return object_->worldTransform_; };
    void OnCollision(Collider* collider) override;
    std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
    void SetCubeAABB(const AABB& aabb) { SetAABB(aabb); cubeMesh_->SetMinMax(aabb); };
protected:
    std::unique_ptr<Object3d> object_;
};
