#pragma once
#include"WorldTransform.h"
class LightingManager
{
public:
    void Initialize();
    void UpdatePointLight();
    void DirectionalLightUpdate();
    WorldTransform playerHandPos_;
    static bool isPointLightOn_;
};

