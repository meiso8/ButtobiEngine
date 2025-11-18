#include "Collider.h"

Collider::Collider()
{
#ifdef _DEBUG
    sphereMesh_ = std::make_unique<SphereMesh>();
    sphereMesh_->Create(Texture::WHITE_1X1);
    object3d_.Create();
    object3d_.SetMesh(sphereMesh_.get());
#endif // _DEBUG
}

void Collider::ColliderUpdate()
{
#ifdef _DEBUG

    object3d_.SetColor({ 1.0f,1.0f,0.0f,0.5f });
    object3d_.worldTransform_.translate_ = GetWorldPosition();
    object3d_.Update();
#endif // _DEBUG
}

void Collider::ColliderDraw(Camera& camera)
{
#ifdef _DEBUG
    object3d_.SetLightMode(kLightModeNone);
    object3d_.Draw(camera);
#endif // _DEBUG
}

void Collider::OnCollisionCollider()
{
#ifdef _DEBUG
    object3d_.SetColor({ 1.0f,0.0f,0.0f,0.5f });
#endif // _DEBUG

}

