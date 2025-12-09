#include "BackGround.h"

BackGround::BackGround()
{
    tree_ = std::make_unique<Tree>();
    ground_ = std::make_unique<Ground>();
    betoBeto_ = std::make_unique<BetoBeto>();

}

void BackGround::Initialize()
{
    tree_->Initialize();
    ground_->Init();
    betoBeto_->Initialize();
}

void BackGround::Update()
{
    tree_->Update();
    ground_->Update();
    betoBeto_->Update();
}

void BackGround::Draw(Camera& camera)
{
    tree_->Draw(camera);
    ground_->Draw(camera);
    tree_->Draw(camera);
    betoBeto_->Draw(camera);
}
