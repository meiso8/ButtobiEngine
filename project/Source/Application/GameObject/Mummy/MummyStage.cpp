#include "MummyStage.h"
#include"CollisionManager.h"

void MummyStage::Initialize() {
    mummy_ = std::make_unique<Mummy>();
    mummy_->Initialize();
}

void MummyStage::Update() {
    mummy_->Update();
}

bool MummyStage::IsColliderRay(RaySprite& raySprite)
{
    AABB aabb = GetAABBWorldPos(mummy_.get());

    if (raySprite.IntersectsAABB(aabb, mummy_->GetWorldPosition())) {
        return true;
    }

    return false;
}

void MummyStage::Draw(Camera& camera) {
    mummy_->Draw(camera);
}
