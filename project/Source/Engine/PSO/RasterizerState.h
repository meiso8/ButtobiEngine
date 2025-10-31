#pragma once

#include<d3d12.h>
#include<stdint.h>

enum CullMode
{
    kCullModeNone,
    kCullModeFront,
    kCullModeBack,
    kCountOfCullMode
};

enum FillMode
{
    kFillModeWireframe,
    kFillModeSolid,
    kCountOfFillMode,
};

class RasterizerState {
public:
    void Create(uint32_t cullMode, uint32_t fillMode);
    D3D12_RASTERIZER_DESC& GetDesc()
    {
        return rasterizerDesc_;
    };
private:
    D3D12_RASTERIZER_DESC rasterizerDesc_{};
};