#pragma once
#include"Vector3.h"
#include"WorldTransform.h"
class PlayerDirectionSpotLight
{
public:
     PlayerDirectionSpotLight();
    void Update();
    Vector3* dirPtr_ = nullptr;
    WorldTransform pos_;
};

