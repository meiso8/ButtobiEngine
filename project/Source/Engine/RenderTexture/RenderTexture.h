#pragma once
#include<wrl.h>
#include<d3d12.h>
#include"Vector4.h"
class RenderTexture

{
private:
    Vector4 kRenderTargetClearValue_;
    Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource_;
public:
    void Create();
    const Vector4& GetColor() {
        return kRenderTargetClearValue_;
    }
 
};

