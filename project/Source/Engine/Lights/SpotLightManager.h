#pragma once
#include<d3d12.h>
#include<wrl.h>
#include"Lights/Light.h"

class SpotLightManager
{
public:
    static void Create();
    static void Finalize();
    static void SetGraphicsRootConstantBufferView();
    static SpotLight* GetData() { return spotLightData_; }
private:
    static Microsoft::WRL::ComPtr <ID3D12Resource> spotLightResource_;
    static SpotLight* spotLightData_;
};

