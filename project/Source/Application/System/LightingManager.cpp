#include "LightingManager.h"
#include"Lights/DirectionalLightManager.h"
#include"Lights/PointLightManager.h"
#include"Lights/SpotLightManager.h"
#include"TimeManager.h"
#include"DebugUI.h"
#include"Random.h"

bool LightingManager::isSpotLightOn_ = false;

void LightingManager::NoonDirLightInit()
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

    InitializeInformationSceneLight();

    playerHandPos_.Initialize();
    isSpotLightOn_ = true;
}

void LightingManager::UpdateFlashlight()
{

    if (isSpotLightOn_) {
        WorldTransformUpdate(playerHandPos_);
        SpotLightManager::GetData(SpotLightType::kFlashlight).color = { 1.0f,1.0f,1.0f,1.0f };
        SpotLightManager::GetData(SpotLightType::kFlashlight).position = playerHandPos_.GetWorldPosition();
        SpotLightManager::GetData(SpotLightType::kFlashlight).direction = *direction_;
    } else {             
        SpotLightManager::GetData(SpotLightType::kFlashlight).color = { 0.0f,0.0f,0.0f,0.0f };
    }

}

void LightingManager::UpdateDirectionalLight()
{
    if (DirectionalLightManager::GetDirectionalLightData()->intensity < 1.0f) {
        DirectionalLightManager::GetDirectionalLightData()->intensity += TimeManager::DeltaTime() * 2.0f;
    } else {
        DirectionalLightManager::GetDirectionalLightData()->intensity = 1.0f;
        isSpotLightOn_ = false;
        PointLightManager::InitData(SpotLightType::kFlashlight);
    }

}

void LightingManager::SetHandPosParent(const WorldTransform* parent)
{
    playerHandPos_.Parent(*parent);
}

void LightingManager::UpdateInformationStageLight()
{
    Random random;
    
    float intensity = 40.0f;
    if (random.Get() <= 0.01f) {
        intensity = 10.0f;
    }

    SpotLightManager::GetData(SpotLightType::kInformationLight).intensity = intensity;//輝度
}

void LightingManager::SetPointLightIntensity(const float intensity)
{
    PointLightManager::GetData(0).intensity = intensity;
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
    SpotLightManager::GetData(SpotLightType::kFlashlight).color = { 1.0f,1.0f,1.0f,1.0f };
    SpotLightManager::GetData(SpotLightType::kFlashlight).position = { 0.0f,0.0f,0.0f };//位置
    SpotLightManager::GetData(SpotLightType::kFlashlight).intensity = { 25.0f };//輝度
    SpotLightManager::GetData(SpotLightType::kFlashlight).direction = { 0.0f,0.0f,1.0f };//スポットライトの方向
    SpotLightManager::GetData(SpotLightType::kFlashlight).distance = 0.1f;
    SpotLightManager::GetData(SpotLightType::kFlashlight).decay = 0.5f;
    SpotLightManager::GetData(SpotLightType::kFlashlight).cosAngle = cosf(Math::kPi / 8.0f);//スポットライトの余弦
    SpotLightManager::GetData(SpotLightType::kFlashlight).cosFalloffStart = 2.0f;
}

void LightingManager::InitializeInformationSceneLight()
{
    SpotLightManager::GetData(SpotLightType::kInformationLight).color = {0.0f,133.0f/256.0f,1.0f,1.0f };
    SpotLightManager::GetData(SpotLightType::kInformationLight).position = { -0.75f,4.0f,0.0f };//位置
    SpotLightManager::GetData(SpotLightType::kInformationLight).intensity = 0.0f;//輝度
    SpotLightManager::GetData(SpotLightType::kInformationLight).direction = { 0.0f,-1.0f,0.0f };//スポットライトの方向
    SpotLightManager::GetData(SpotLightType::kInformationLight).distance = 0.1f;
    SpotLightManager::GetData(SpotLightType::kInformationLight).decay = 0.5f;
    SpotLightManager::GetData(SpotLightType::kInformationLight).cosAngle = cosf(Math::kPi / 8.0f);//スポットライトの余弦
    SpotLightManager::GetData(SpotLightType::kInformationLight).cosFalloffStart = 2.0f;
}

