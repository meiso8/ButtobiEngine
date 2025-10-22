#pragma once

#include"Material.h"

//ComPtr(コムポインタ)
#include<wrl.h>
#include<d3d12.h>

class MaterialResource {

public:

    ~MaterialResource();

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

    void SetColor(const Vector4& color);
    void SetMaterial(Material* material);
    void SetUV(const Matrix4x4& transform);
    void SetLightType(uint32_t lightType);
private:
    Microsoft::WRL::ComPtr <ID3D12Resource> materialResource_ = nullptr;
    Material* material_ = nullptr;
};