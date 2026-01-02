#include "Memo.h"
#include"CollisionConfig.h"

Memo::Memo() {

    SetType(kAABB);
    SetCollisionAttribute(kCollisionWall);
    SetCollisionMask(kCollisionPlayer);

    // サイズ
    //SetAABB({ { -0.125f, -0.0625f, -0.125f },{  0.125f, 0.0625f, 0.125f } });

    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_->Create();
    cubeMesh_->SetMinMax(GetAABB());
    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMesh(cubeMesh_.get());
}

void Memo::Initialize() {
    object_->Initialize();
}

void Memo::Update() {
    object_->Update();
    object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

void Memo::Draw(Camera& camera) {
    object_->Draw(camera);
}

Vector3 Memo::GetWorldPosition() const {
    return object_->worldTransform_.GetWorldPosition();
}

void Memo::OnCollision(Collider* collider)
{

}

