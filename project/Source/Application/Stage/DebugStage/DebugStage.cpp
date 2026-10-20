#include "DebugStage.h"

#include"InputBind.h"

DebugStage::DebugStage()
{
}

void DebugStage::Initialize()
{


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
