#pragma once
#include"WorldTransform.h"
class LightingManager
{
public:
    static void NoonLightInit();
    void Initialize();
    void UpdatePointLight();
    void DirectionalLightUpdate();
    void SetHandPosParent(const WorldTransform* parent);
    void SetDirection(const Vector3* direction) { direction_ = direction; }
    static bool GetIsPointLightOn() { return isPointLightOn_; }
    static void setIsPointLightOn(const bool flag) { isPointLightOn_ = flag; }
private:
    WorldTransform playerHandPos_{};
    const Vector3* direction_ = nullptr;
    static bool isPointLightOn_;
};

