#pragma once
#include"WorldTransform.h"
class LightingManager
{
public:
    void Initialize();
    void UpdatePointLight();
    void DirectionalLightUpdate();
    WorldTransform playerHandPos_;
    Vector3* direction_;
    static bool isPointLightOn_;
};

