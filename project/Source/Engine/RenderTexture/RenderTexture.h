#pragma once
#include<wrl.h>
#include<d3d12.h>
#include"Vector4.h"
#include<stdint.h>
#include<PSO.h>
#include"hlslTypeToCpp.h"

struct MaterialForRenderTexture {
    float4 color;
    bool useEffect = false;
    uint32_t type = false;
    bool padding[2] = { };
};

struct MaterialForVignette
{
    float correctVal;
    float viignetteVal;
    float padding[2];
};


struct MaterialForBoxFilter
{
    float kernel;
    float padding[3];
};

class RenderTexture

{
public:


    struct RenderTextureData {
        Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
        uint32_t srvIndex = 0;
        D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU = {};
        D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU = {};
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandleCPU = {};
    };
private:
    Vector4 kRenderTargetClearValue_;
    const Vector4 sepiaColor_ = {1.0f,74.0f/107.0f,43.0f/107.0f,1.0f};
    RenderTextureData renderTextureData_;
    std::array<Microsoft::WRL::ComPtr <ID3D12Resource>,PSO::kCountOfEffect> materialResource_;
    MaterialForRenderTexture* materialForGrayScale_ = nullptr;
    MaterialForVignette* materialForVignette_ = nullptr;
    MaterialForBoxFilter* materialForBoxFilter_ = nullptr;
public:

    void Create();
    const Vector4& GetColor() {
        return kRenderTargetClearValue_;
    }
    RenderTextureData& GetRenderTextureData() {
        return renderTextureData_;
    }
    void Draw(const PSO::EffectType& effectType);
    void Update();
private:
    void CreateMaterialBufferForGrayScale();
    void CreateMaterialBufferForVignette();
    void CreateMaterialBufferForBoxFilter();
};

