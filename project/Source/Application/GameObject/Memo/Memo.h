#pragma once

#include "Collider.h"
#include <memory>
#include"CubeMesh.h"
#include"Texture.h"

class Memo : public Collider {

private:
    std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
    std::unique_ptr<Object3d> object_;
public:
    Memo();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    Vector3 GetWorldPosition() const override;
    WorldTransform& GetWorldTransform() { return object_->worldTransform_; };
    void OnCollision(Collider* collider) override;
    void SetTexture(const TextureFactory::Handle& handle) {
        object_->SetTextureHandle(handle);
    };
    void SetTranslate(const Vector3& translate) {
        object_->worldTransform_.translate_ = translate
            ;
    }
    void SetCubeSize(const AABB& aabb) {
        SetAABB(aabb);
        cubeMesh_->SetMinMax(GetAABB()); 
        object_->SetMesh(cubeMesh_.get());
    };
    void SetColor(const Vector4& color) { object_->SetColor(color); }
};

