#include "DebugStage.h"

#include"InputBind.h"
#include"Network/Network.h"

DebugStage::DebugStage()
{
    network_ = std::make_unique<Network>();
}

DebugStage::~DebugStage()
{
   /* network_->Finalize();*/
}

void DebugStage::Initialize()
{
    network_->Initialize();
}

void DebugStage::StageTransitionInitialize()
{

    GetMemoManager()->GenerateAllMemo();
    // ミイラ前に移動
    GetPlayer()->Init({ 3.25f, 0.0f, -3.25f });

    GetItemManager()->GenerateItems({});
}

void DebugStage::Update()
{
    GetMemoManager()->Debug();
    network_->Update();
    
}

void DebugStage::Draw()
{

}

void DebugStage::CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)
{

}

void DebugStage::DrawSprite()
{
  
}
