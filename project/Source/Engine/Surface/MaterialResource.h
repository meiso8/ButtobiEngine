#pragma once

//ComPtr(コムポインタ)
#include<wrl.h>
#include<d3d12.h>
#include<cstdint>
#include"hlslTypeToCpp.h"

class MaterialResource {

public:

    struct Material
    {
        float32_t4 color;
        int32_t lightType;
        float padding[3];
        float32_t4x4 uvTransform;
        float32_t shininess;
        float padding2[3];
    };

    MaterialResource() = default;
    ~MaterialResource();
    void UnMap();
    enum LIGHTTYPE {
        NONE,
        L_REFLECTANCE,
        HALF_L,
    };

    void CreateMaterial(const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, uint32_t lightType = LIGHTTYPE::NONE);
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
    void SetLightType(uint32_t lightType);
    void SetShininess(const float32_t& shininess);
private:
    Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_ = nullptr;
    Material* material_ = nullptr;
};