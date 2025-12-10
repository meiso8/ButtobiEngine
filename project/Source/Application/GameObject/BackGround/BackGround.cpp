#include "BackGround.h"

BackGround::BackGround()
{
    tree_ = std::make_unique<Tree>();
    ground_ = std::make_unique<Ground>();
    betoBeto_ = std::make_unique<BetoBeto>();
    skyDome_ = std::make_unique<SkyDome>();
}

void BackGround::Initialize()
{
    tree_->Initialize();
    ground_->Init();
    betoBeto_->Initialize();
    skyDome_->Init();
}

void BackGround::Update()
{
    tree_->Update();
    ground_->Update();
    betoBeto_->Update();
    skyDome_->Update();
}

void BackGround::Draw(Camera& camera)
{
    skyDome_->Draw(camera);
    tree_->Draw(camera);
    ground_->Draw(camera);
    tree_->Draw(camera);
    betoBeto_->Draw(camera);

}
