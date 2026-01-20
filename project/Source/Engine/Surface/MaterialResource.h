#pragma once

//ComPtr(コムポインタ)
#include<wrl.h>
#include<d3d12.h>
#include<cstdint>
#include"hlslTypeToCpp.h"

enum LightMode {
    kLightModeNone,
    kLightModeLReflectance,
    kLightModeHalfL,
};

struct Material
{
    float4 color;
    int32_t lightMode;
    float padding[3];
    float32_t4x4 uvTransform;
    float32_t shininess;
    float padding2[3];
};

class MaterialResource {

public:

    MaterialResource() = default;
    ~MaterialResource();
    void UnMap();

    void CreateMaterial(const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, uint32_t lightType = LightMode::kLightModeNone);
    Material* GetMaterial() {
        return material_;
    };
    Microsoft::WRL::ComPtr <ID3D12Resource>& GetMaterialResource() {
        return materialResource_
            ;
    };

    MaterialResource(const MaterialResource&) = delete;
    MaterialResource& operator=(const MaterialResource&) = delete;
    void SetColor(const Vector4& color);
    void SetUV(const Matrix4x4& transform);
    void SetLightMode(uint32_t lightType);
    void SetShininess(const float32_t& shininess);
private:
    Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_ = nullptr;
    Material* material_ = nullptr;
};