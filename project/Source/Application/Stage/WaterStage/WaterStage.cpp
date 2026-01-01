#include "WaterStage.h"
WaterStage::WaterStage()
{
    water_ = std::make_unique<Water>();
    papyrusWall_ = std::make_unique<PapyrusWall>();
}

void WaterStage::Initialize()
{
    water_->Initialize();
    papyrusWall_->Init();
}

void WaterStage::Update()
{
    water_->Update();
    papyrusWall_->Update();

}

void WaterStage::Draw(Camera& camera)
{
    papyrusWall_->Draw(camera);
    water_->Draw(camera);
}

void WaterStage::CheckCollision(CollisionManager& collisionManager)
{
    //Waterのかべ
    for (auto& [type, object] : papyrusWall_.get()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }

    collisionManager.AddCollider(water_.get());
}

