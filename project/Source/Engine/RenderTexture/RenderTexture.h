#pragma once
#include<wrl.h>
#include<d3d12.h>
#include"Vector4.h"
#include<stdint.h>


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
    RenderTextureData renderTextureData_;
public:

    void Create();
    const Vector4& GetColor() {
        return kRenderTargetClearValue_;
    }
    RenderTextureData& GetRenderTextureData() {
        return renderTextureData_;
    }
    void Draw();
};

