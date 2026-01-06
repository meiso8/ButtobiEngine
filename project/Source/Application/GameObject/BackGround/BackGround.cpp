#include "BackGround.h"

BackGround::BackGround()
{
    world_ = std::make_unique<World>();
    filed_ = std::make_unique<Field>();
    building_ = std::make_unique<Building>();

}

void BackGround::Initialize()
{
    world_->Init();
    filed_->Init();
    building_->Init();
}

void BackGround::Update()
{
    building_->Update();
    world_->Update();
}

void BackGround::Draw(Camera& camera)
{
    world_->Draw(camera);

    building_->Draw(camera);
}

void BackGround::DrawField(Camera& camera)
{
    filed_->Draw(camera);
}

void BackGround::UpdateApperMedjed()
{
    world_->UpdateColor();
    filed_->Update();
}
