#pragma once

#include<wrl.h>
#include<d3d12.h>
#include"Lights/Light.h"
#include<cstdint>
#include<cassert>
class PointLightManager
{
public:
    static void CreateData();
    static void SetGraphicsRootConstantBufferView();
    static void InitData(const uint32_t& index);
    static void InitDatas();
    static PointLight& GetPointLightData(const uint32_t& index) { assert(index < kMaxData_); return pointLightData_[index]; };
    static void Finalize();
private:
    static Microsoft::WRL::ComPtr <ID3D12Resource> pointLightResource_;
    static PointLight* pointLightData_;
    static const uint32_t kMaxData_ = 20;
};

