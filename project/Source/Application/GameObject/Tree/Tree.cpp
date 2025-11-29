#include "Tree.h"

#include"ModelManager.h"
#include"Model.h"
#include"DebugUI.h"
#include"Easing.h"
#include"Input.h"
#include<algorithm>

Tree::Tree()
{
    position_.Create();
    position_.SetMesh(ModelManager::GetModel(ModelManager::TREE));
}

void Tree::Initialize()
{
    position_.Initialize();

    //position_.worldTransform_.scale_ = { 2.5f,2.5f,2.5f };
    //position_.worldTransform_.translate_.y = -0.45f;
}

void Tree::Update()
{

    position_.Update();
    DebugUI::CheckObject3d(position_, "TreePos");

}

void Tree::Draw(Camera& camera, const LightMode& lightType)
{
    position_.SetLightMode(lightType);

    position_.Draw(camera);

}
