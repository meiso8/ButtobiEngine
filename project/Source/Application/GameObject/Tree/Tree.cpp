#include "Tree.h"

#include"ModelManager.h"
#include"Model.h"
#include"DebugUI.h"
#include"Easing.h"
#include"Input.h"
#include<algorithm>

Tree::Tree()
{

    positions_[kTree].Create();
    positions_[kLeaves].Create();

    positions_[kTree].SetMesh(ModelManager::GetModel(ModelManager::TREE));
    positions_[kLeaves].SetMesh(ModelManager::GetModel(ModelManager::LEAVES));

    positions_[kLeaves].worldTransform_.Parent(positions_[kTree].worldTransform_);
}

void Tree::Initialize()
{
    positions_[kTree].Initialize();
    positions_[kLeaves].Initialize();
    positions_[kTree].worldTransform_.scale_ = { 50.0f,50.0f,50.0f };
    positions_[kTree].worldTransform_.translate_ = { 0.0f,-7.0f,-6.0f };
    //position_.worldTransform_.scale_ = { 2.5f,2.5f,2.5f };
    //position_.worldTransform_.translate_.y = -0.45f;
}

void Tree::Update()
{
    positions_[kTree].Update();
    positions_[kLeaves].Update();

    DebugUI::CheckObject3d(positions_[kTree], "TreePos");
    DebugUI::CheckObject3d(positions_[kLeaves], "LeavePos");
}

void Tree::Draw(Camera& camera, const LightMode& lightType)
{
    positions_[kTree].SetLightMode(lightType);
    positions_[kLeaves].SetLightMode(lightType);

    positions_[kTree].Draw(camera);
    positions_[kLeaves].Draw(camera,kBlendModeNormal, kCullModeNone);

}
