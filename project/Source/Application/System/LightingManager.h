#pragma once
#include"WorldTransform.h"
class LightingManager
{
public:

    static void NoonDirLightInit();
    static void InitializeDirectionalLight();
    static void InitializePointLight();
    void Initialize();
    void UpdateFlashlight();
    void UpdateDirectionalLight();
    void SetHandPosParent(const WorldTransform* parent);
    void SetDirection(const Vector3* direction) { direction_ = direction; }
    static bool GetIsPointLightOn() { return isSpotLightOn_; }
    static void setIsPointLightOn(const bool flag) { isSpotLightOn_ = flag; }
    void UpdateInformationStageLight();
    void SetPointLightIntensity(const float intensity);
private:
    enum SpotLightType {
        kFlashlight,
        kInformationLight
    };


    void InitializeSpotLight();
    void InitializeInformationSceneLight();

private:

    WorldTransform playerHandPos_{};
    const Vector3* direction_ = nullptr;
    static bool isSpotLightOn_;
};

