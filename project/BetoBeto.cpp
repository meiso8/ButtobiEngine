#include "BetoBeto.h"

#include"ModelManager.h"
#include"Model.h"
#include"DebugUI.h"
#include"Easing.h"
#include"Input.h"
#include<algorithm>

BetoBeto::BetoBeto()
{
    position_.Create();
    model_ = ModelManager::GetModel(ModelManager::BETOBETO_FLOOR);
    position_.SetMesh(model_);
}

void BetoBeto::Initialize()
{
    position_.Initialize();
    position_.worldTransform_.translate_ = { 0.0f,-7.0f,-35.0f };
}

void BetoBeto::Update()
{
    position_.Update();

    DebugUI::CheckObject3d(position_, "BetoBetoFloor");

}

void BetoBeto::Draw(Camera& camera)
{
    position_.SetLightMode(kLightModeLReflectance);
    position_.Draw(camera, kBlendModeNormal, kCullModeNone);

}
