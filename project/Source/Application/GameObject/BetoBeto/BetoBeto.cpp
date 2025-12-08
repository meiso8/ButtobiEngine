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

    position_.GetWaveData(0).amplitude = 0.1f;
    position_.GetWaveData(0).frequency = 30;
    position_.GetWaveData(0).direction = { 1.0f,0.0f,0.0f };

    position_.GetWaveData(1).amplitude = 0.2f;
    position_.GetWaveData(1).frequency = 20;
    position_.GetWaveData(1).direction = { 0.0f,0.0f,1.0f };
}

void BetoBeto::Initialize()
{
    position_.GetWaveData(0).time = 0.0f;
    position_.GetWaveData(1).time = 0.0f;
    position_.worldTransform_.translate_ = { 0.0f,-0.8f,0.0f };
}

void BetoBeto::Update()
{
    position_.Update();

    position_.GetWaveData(0).time += InverseFPS;
    position_.GetWaveData(1).time += InverseFPS;

    DebugUI::CheckObject3d(position_, "BetoBetoFloor");

}

void BetoBeto::Draw(Camera& camera)
{
    //position_.SetLightMode(kLightModeLReflectance);
    position_.Draw(camera, kBlendModeNormal, kCullModeNone);
}
