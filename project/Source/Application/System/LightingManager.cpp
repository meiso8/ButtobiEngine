#include "LightingManager.h"
#include"Lights/DirectionalLightManager.h"
#include"Lights/PointLightManager.h"
#include"Lights/SpotLightManager.h"
#include"Window.h"
#include"DebugUI.h"

bool LightingManager::isPointLightOn_ = false;

void LightingManager::Initialize()
{
    DirectionalLightManager::GetDirectionalLightData()->direction = { 0.0f,-1.0f,0.0f };
    DirectionalLightManager::GetDirectionalLightData()->intensity = 0.0f;

    PointLightManager::GetPointLightData(0).color = { 160.0f / 255.0f,1.0f,1.0f,1.0f };
    PointLightManager::GetPointLightData(0).intensity = 1.0f;
    PointLightManager::GetPointLightData(0).position = { 0.0f,15.0f,0.0f };
    PointLightManager::GetPointLightData(0).radius = 30.0f;
    PointLightManager::GetPointLightData(0).decay = 0.3f;

    SpotLightManager::GetData().intensity = 25.0f;
    SpotLightManager::GetData().color = { 1.0f,1.0f,1.0f,1.0f };
    SpotLightManager::GetData().distance =0.1f;
    SpotLightManager::GetData().decay = 0.5f;

    playerHandPos_.Initialize();
    isPointLightOn_ = true;
}

void LightingManager::UpdatePointLight()
{

    if (isPointLightOn_) {
        WorldTransformUpdate(playerHandPos_);
        SpotLightManager::GetData().color = { 1.0f,1.0f,1.0f,1.0f };
        SpotLightManager::GetData().position = playerHandPos_.GetWorldPosition();
        SpotLightManager::GetData().direction = *direction_;
    } else {
        SpotLightManager::GetData().color = { 0.0f,0.0f,0.0f,0.0f };
    }

}

void LightingManager::DirectionalLightUpdate()
{
    if (DirectionalLightManager::GetDirectionalLightData()->intensity < 1.0f) {
        DirectionalLightManager::GetDirectionalLightData()->intensity += InverseFPS * 2.0f;
    } else {
        DirectionalLightManager::GetDirectionalLightData()->intensity = 1.0f;
        isPointLightOn_ = false;
        PointLightManager::InitData(0);
    }

}

