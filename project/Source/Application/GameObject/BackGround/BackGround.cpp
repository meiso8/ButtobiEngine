#include "BackGround.h"

BackGround::BackGround()
{
    world_ = std::make_unique<World>();
    field_ = std::make_unique<Field>();
    building_ = std::make_unique<Building>();

}

void BackGround::Initialize()
{
    world_->Init();
    field_->Init();
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
    field_->Draw(camera);
}

void BackGround::UpdateApperMedjed()
{
    world_->UpdateColor();
    field_->Update();
}
