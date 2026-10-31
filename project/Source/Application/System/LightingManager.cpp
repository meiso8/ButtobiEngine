#include "LightingManager.h"
#include"Lights/DirectionalLightManager.h"
#include"Lights/PointLightManager.h"
#include"Lights/SpotLightManager.h"
#include"TimeManager.h"
#include"DebugUI.h"

bool LightingManager::isSpotLightOn_ = false;

void LightingManager::NoonLightInit()
{
    auto* direLight = DirectionalLightManager::GetDirectionalLightData();
    direLight->color = { 1.0f,230.0f / 255.0f,200.0f / 255.0f,1.0f };
    direLight->direction = { 0.0f,-1.0f,0.0f };//向きは正規化する
    direLight->intensity = 1.0f;
}

void LightingManager::Initialize()
{

    InitializeDirectionalLight();

    InitializePointLight();

    InitializeSpotLight();

    InformationSceneLight();

    playerHandPos_.Initialize();
    isSpotLightOn_ = true;
}

void LightingManager::UpdateSpotLight()
{

    if (isSpotLightOn_) {
        WorldTransformUpdate(playerHandPos_);
        SpotLightManager::GetData(0).color = { 1.0f,1.0f,1.0f,1.0f };
        SpotLightManager::GetData(0).position = playerHandPos_.GetWorldPosition();
        SpotLightManager::GetData(0).direction = *direction_;
    } else {
        SpotLightManager::GetData(0).color = { 0.0f,0.0f,0.0f,0.0f };
    }

}

void LightingManager::DirectionalLightUpdate()
{
    if (DirectionalLightManager::GetDirectionalLightData()->intensity < 1.0f) {
        DirectionalLightManager::GetDirectionalLightData()->intensity += TimeManager::DeltaTime() * 2.0f;
    } else {
        DirectionalLightManager::GetDirectionalLightData()->intensity = 1.0f;
        isSpotLightOn_ = false;
        PointLightManager::InitData(0);
    }

}

void LightingManager::SetHandPosParent(const WorldTransform* parent)
{
    playerHandPos_.Parent(*parent);
}

void LightingManager::UpdateInformationSpotLight()
{
    SpotLightManager::GetData(1).intensity = { 100.0f };//輝度
}

void LightingManager::InitializeDirectionalLight()
{
    DirectionalLightManager::GetDirectionalLightData()->direction = { 0.0f,-1.0f,0.0f };
    DirectionalLightManager::GetDirectionalLightData()->intensity = 0.0f;

}

void LightingManager::InitializePointLight()
{
    PointLightManager::GetData(0).color = { 160.0f / 255.0f,1.0f,1.0f,1.0f };
    PointLightManager::GetData(0).intensity = 1.0f;
    PointLightManager::GetData(0).position = { 0.0f,15.0f,0.0f };
    PointLightManager::GetData(0).radius = 30.0f;
    PointLightManager::GetData(0).decay = 0.3f;
}

void LightingManager::InitializeSpotLight()
{
    SpotLightManager::GetData(0).color = { 1.0f,1.0f,1.0f,1.0f };
    SpotLightManager::GetData(0).position = { 0.0f,0.0f,0.0f };//位置
    SpotLightManager::GetData(0).intensity = { 25.0f };//輝度
    SpotLightManager::GetData(0).direction = { 0.0f,0.0f,1.0f };//スポットライトの方向
    SpotLightManager::GetData(0).distance = 0.1f;
    SpotLightManager::GetData(0).decay = 0.5f;
    SpotLightManager::GetData(0).cosAngle = cosf(Math::kPi / 8.0f);//スポットライトの余弦
    SpotLightManager::GetData(0).cosFalloffStart = 2.0f;
}

void LightingManager::InformationSceneLight()
{
    SpotLightManager::GetData(1).color = { 1.0f,1.0f,1.0f,1.0f };
    SpotLightManager::GetData(1).position = { 0.0f,0.0f,0.0f };//位置
    SpotLightManager::GetData(1).intensity = {0.0f };//輝度
    SpotLightManager::GetData(1).direction = { 0.0f,1.0f,0.0f };//スポットライトの方向
    SpotLightManager::GetData(1).distance = 0.1f;
    SpotLightManager::GetData(1).decay = 0.5f;
    SpotLightManager::GetData(1).cosAngle = cosf(Math::kPi / 8.0f);//スポットライトの余弦
    SpotLightManager::GetData(1).cosFalloffStart = 2.0f;
}

