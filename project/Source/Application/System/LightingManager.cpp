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
    DirectionalLightManager::GetDirectionalLightData()->intensity = -1.0f;

    PointLightManager::GetPointLightData(0).color = { 160.0f / 255.0f,1.0f,1.0f,1.0f };
    PointLightManager::GetPointLightData(0).intensity = 3.3f;
    PointLightManager::GetPointLightData(0).position = { 0.0f,15.0f,0.0f };
    PointLightManager::GetPointLightData(0).radius = 30.0f;
    PointLightManager::GetPointLightData(0).decay = 0.3f;

    SpotLightManager::GetData().color = { 1.0f,1.0f,1.0f,1.0f };

    playerHandPos_.Initialize();
    playerHandPos_.translate_ = { 0.0f,2.0f,3.0f };
    isPointLightOn_ = false;
}

void LightingManager::UpdatePointLight()
{
    DebugUI::CheckWorldTransform(playerHandPos_, "handPos");

    if (isPointLightOn_) {
        WorldTransformUpdate(playerHandPos_);
        SpotLightManager::GetData().color = { 1.0f,1.0f,1.0f,1.0f };
        SpotLightManager::GetData().position = playerHandPos_.GetWorldPosition();
        SpotLightManager::GetData().direction = player
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

