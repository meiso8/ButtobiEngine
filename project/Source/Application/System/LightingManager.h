#pragma once
#include"WorldTransform.h"
class LightingManager
{
public:
    static void NoonLightInit();
    void Initialize();
    void UpdateSpotLight();
    void DirectionalLightUpdate();
    void SetHandPosParent(const WorldTransform* parent);
    void SetDirection(const Vector3* direction) { direction_ = direction; }
    static bool GetIsPointLightOn() { return isSpotLightOn_; }
    static void setIsPointLightOn(const bool flag) { isSpotLightOn_ = flag; }

    void UpdateInformationSpotLight();
private:
    void InitializeDirectionalLight();
    void InitializePointLight();
    void InitializeSpotLight();
    void InformationSceneLight();

private:

    WorldTransform playerHandPos_{};
    const Vector3* direction_ = nullptr;
    static bool isSpotLightOn_;
};

